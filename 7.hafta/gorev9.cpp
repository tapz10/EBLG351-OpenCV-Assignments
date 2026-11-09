#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>

using namespace cv;
using namespace std;

void idft2D(const Mat& realPart, const Mat& imagPart, Mat& cikis) {
    int M = realPart.rows;
    int N = realPart.cols;
    cikis = Mat::zeros(M, N, CV_64F);

    for (int x = 0; x < M; x++) {
        for (int y = 0; y < N; y++) {
            double toplam = 0.0;
            for (int u = 0; u < M; u++) {
                for (int v = 0; v < N; v++) {
                    double aci = 2.0 * CV_PI * ((u * x) / (double)M + (v * y) / (double)N);
                    toplam += realPart.at<double>(u, v) * cos(aci) - imagPart.at<double>(u, v) * sin(aci);
                }
            }
            cikis.at<double>(x, y) = toplam;
        }
    }
    cikis.convertTo(cikis, CV_8UC1);
}

double hesaplaZMNCC(const Mat& img1, const Mat& img2) {
    Mat m1, m2;
    img1.convertTo(m1, CV_64F);
    img2.convertTo(m2, CV_64F);

    Scalar mean1 = mean(m1);
    Scalar mean2 = mean(m2);

    double pay = 0.0, payda1 = 0.0, payda2 = 0.0;
    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m1.cols; j++) {
            double diff1 = m1.at<double>(i, j) - mean1[0];
            double diff2 = m2.at<double>(i, j) - mean2[0];
            pay += diff1 * diff2;
            payda1 += diff1 * diff1;
            payda2 += diff2 * diff2;
        }
    }
    if (payda1 == 0.0 || payda2 == 0.0) return 0.0;
    return pay / (sqrt(payda1) * sqrt(payda2));
}

int main() {
    Mat resim = imread("pisa.jpg", IMREAD_GRAYSCALE);
    if (resim.empty()) {
        resim = Mat(32, 32, CV_8UC1, Scalar(128));
    }

    Mat kucukResim;
    resize(resim, kucukResim, Size(32, 32));

    kucukResim.convertTo(kucukResim, CV_64F);
    int M = kucukResim.rows;
    int N = kucukResim.cols;

    Mat realPart = Mat::zeros(M, N, CV_64F);
    Mat imagPart = Mat::zeros(M, N, CV_64F);

    for (int u = 0; u < M; u++) {
        for (int v = 0; v < N; v++) {
            double rSum = 0.0, iSum = 0.0;
            for (int x = 0; x < M; x++) {
                for (int y = 0; y < N; y++) {
                    double aci = 2.0 * CV_PI * ((u * x) / (double)M + (v * y) / (double)N);
                    double pixel = kucukResim.at<double>(x, y);
                    rSum += pixel * cos(aci);
                    iSum -= pixel * sin(aci);
                }
            }
            realPart.at<double>(u, v) = rSum / (M * N);
            imagPart.at<double>(u, v) = iSum / (M * N);
        }
    }

    Mat yenidenUretilen;
    idft2D(realPart, imagPart, yenidenUretilen);

    Mat orijinal8U;
    kucukResim.convertTo(orijinal8U, CV_8UC1);
    double zmnccDegeri = hesaplaZMNCC(orijinal8U, yenidenUretilen);

    cout << "ZMNCC Sonucu: " << zmnccDegeri << endl;
    imwrite("yeniden_uretim_idft.jpg", yenidenUretilen);

    return 0;
}
