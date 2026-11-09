#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>

using namespace cv;
using namespace std;

int main() {
    string dosyaAdi = "pisa.jpg";
    Mat resim = imread(dosyaAdi, IMREAD_COLOR);

    if (resim.empty()) {
        resim = Mat(400, 400, CV_8UC3, Scalar(255, 255, 255));
        line(resim, Point(50, 50), Point(350, 350), Scalar(0, 0, 0), 3);
        line(resim, Point(100, 350), Point(300, 50), Scalar(0, 0, 0), 3);
    }

    Mat gri, cannyCiktisi;
    cvtColor(resim, gri, COLOR_BGR2GRAY);
    Canny(gri, cannyCiktisi, 100, 200);

    int genislik = cannyCiktisi.cols;
    int yukseklik = cannyCiktisi.rows;
    double diyagonal = sqrt(genislik * genislik + yukseklik * yukseklik);
    
    int num_rho = 2 * static_int_cast<int>(diyagonal) + 1;
    int num_theta = 180;

    vector<double> cos_table(num_theta);
    vector<double> sin_table(num_theta);
    for (int theta = 0; theta < num_theta; theta++) {
        double rad = theta * CV_PI / 180.0;
        cos_table[theta] = cos(rad);
        sin_table[theta] = sin(rad);
    }

    Mat accumulator = Mat::zeros(num_rho, num_theta, CV_32SC1);
    double rho_offset = diyagonal;

    for (int y = 0; y < yukseklik; y++) {
        for (int x = 0; x < genislik; x++) {
            if (cannyCiktisi.at<uchar>(y, x) > 0) {
                for (int theta = 0; theta < num_theta; theta++) {
                    double rho = x * cos_table[theta] + y * sin_table[theta];
                    int r_idx = cvRound(rho + rho_offset);
                    if (r_idx >= 0 && r_idx < num_rho) {
                        accumulator.at<int>(r_idx, theta)++;
                    }
                }
            }
        }
    }

    Mat acc_goruntu;
    double minVal, maxVal;
    minMaxLoc(accumulator, &minVal, &maxVal);
    accumulator.convertTo(acc_goruntu, CV_8UC1, 255.0 / maxVal);

    int esikDeger = 120;
    Mat sonucResmi = resim.clone();

    for (int r = 0; r < accumulator.rows; r++) {
        for (int t = 0; t < accumulator.cols; t++) {
            if (accumulator.at<int>(r, t) >= esikDeger) {
                double rho = r - rho_offset;
                double theta = t * CV_PI / 180.0;
                double a = cos_table[t], b = sin_table[t];
                double x0 = a * rho, y0 = b * rho;
                
                Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
                Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)));
                
                line(sonucResmi, pt1, pt2, Scalar(0, 0, 255), 2, LINE_AA);
            }
        }
    }

    imwrite("pisa_accumulator.jpg", acc_goruntu);
    imwrite("pisa_custom_hough_sonuc.jpg", sonucResmi);

    cout << "Ozel Hough Donusumu basariyla tamamlandi ve sonuclar kaydedildi." << endl;

    return 0;
}
