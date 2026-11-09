#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>
#include <cuda_runtime.h>

using namespace cv;
using namespace std;

__global__ void houghLinesKernel(const unsigned char* d_canny, int width, int height, 
                                 int* d_accumulator, int num_rho, int num_theta, 
                                 double rho_offset, const double* d_cosTable, const double* d_sinTable) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x < width && y < height) {
        if (d_canny[y * width + x] > 0) {
            for (int t = 0; t < num_theta; t++) {
                double rho = x * d_cosTable[t] + y * d_sinTable[t];
                int r_idx = __double2int_rn(rho + rho_offset);
                if (r_idx >= 0 && r_idx < num_rho) {
                    atomicAdd(&d_accumulator[r_idx * num_theta + t], 1);
                }
            }
        }
    }
}

int main() {
    string dosyaAdi = "pisa.jpg";
    Mat resim = imread(dosyaAdi, IMREAD_COLOR);

    if (resim.empty()) {
        resim = Mat(500, 500, CV_8UC3, Scalar(255, 255, 255));
        line(resim, Point(100, 50), Point(100, 450), Scalar(0, 0, 0), 4);
        line(resim, Point(200, 50), Point(250, 450), Scalar(0, 0, 0), 4);
    }

    Mat gri, cannyCiktisi;
    cvtColor(resim, gri, COLOR_BGR2GRAY);
    Canny(gri, cannyCiktisi, 100, 200);

    int width = cannyCiktisi.cols;
    int height = cannyCiktisi.rows;
    double diagonal = sqrt(width * width + height * height);
    
    int num_rho = 2 * static_cast<int>(diagonal) + 1;
    int num_theta = 180;
    double rho_offset = diagonal;

    vector<double> h_cosTable(num_theta);
    vector<double> h_sinTable(num_theta);
    for (int t = 0; t < num_theta; t++) {
        double rad = t * CV_PI / 180.0;
        h_cosTable[t] = cos(rad);
        h_sinTable[t] = sin(rad);
    }

    unsigned char* d_canny = nullptr;
    int* d_accumulator = nullptr;
    double* d_cosTable = nullptr;
    double* d_sinTable = nullptr;

    cudaMalloc(&d_canny, width * height * sizeof(unsigned char));
    cudaMalloc(&d_accumulator, num_rho * num_theta * sizeof(int));
    cudaMalloc(&d_cosTable, num_theta * sizeof(double));
    cudaMalloc(&d_sinTable, num_theta * sizeof(double));

    cudaMemcpy(d_canny, cannyCiktisi.data, width * height * sizeof(unsigned char), cudaMemcpyHostToDevice);
    cudaMemset(d_accumulator, 0, num_rho * num_theta * sizeof(int));
    cudaMemcpy(d_cosTable, h_cosTable.data(), num_theta * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_sinTable, h_sinTable.data(), num_theta * sizeof(double), cudaMemcpyHostToDevice);

    dim3 blockSize(16, 16);
    dim3 gridSize((width + blockSize.x - 1) / blockSize.x, (height + blockSize.y - 1) / blockSize.y);

    houghLinesKernel<<<gridSize, blockSize>>>(d_canny, width, height, d_accumulator, num_rho, num_theta, rho_offset, d_cosTable, d_sinTable);
    cudaDeviceSynchronize();

    vector<int> h_accumulator(num_rho * num_theta);
    cudaMemcpy(h_accumulator.data(), d_accumulator, num_rho * num_theta * sizeof(int), cudaMemcpyDeviceToHost);

    Mat accumulator(num_rho, num_theta, CV_32SC1, h_accumulator.data());
    int esikDeger = 100;
    Mat sonucResmi = resim.clone();

    for (int r = 0; r < num_rho; r++) {
        for (int t = 0; t < num_theta; t++) {
            if (accumulator.at<int>(r, t) >= esikDeger) {
                double rho = r - rho_offset;
                double theta = t * CV_PI / 180.0;
                double a = h_cosTable[t], b = h_sinTable[t];
                double x0 = a * rho, y0 = b * rho;
                
                Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
                Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)));
                
                line(sonucResmi, pt1, pt2, Scalar(0, 0, 255), 2, LINE_AA);
            }
        }
    }

    imwrite("pisa_cuda_hough_sonuc.jpg", sonucResmi);
    cout << "CUDA tabanlı paralel Hough Dönüşümü başarıyla tamamlandı ve sonuç kaydedildi." << endl;

    cudaFree(d_canny);
    cudaFree(d_accumulator);
    cudaFree(d_cosTable);
    cudaFree(d_sinTable);

    return 0;
}
