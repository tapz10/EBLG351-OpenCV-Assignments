#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>

using namespace cv;
using namespace std;

void frekansFiltrele(const Mat& giris, Mat& cikis, int filtreTuru, double D0, double W = 10.0) {
    Mat gri;
    if (giris.channels() == 3) cvtColor(giris, gri, COLOR_BGR2GRAY);
    else gri = giris.clone();
    
    gri.convertTo(gri, CV_64F);
    int M = gri.rows;
    int N = gri.cols;

    Mat harmonikReal = Mat::zeros(M, N, CV_64F);
    Mat harmonikImag = Mat::zeros(M, N, CV_64F);

    for (int u = 0; u < M; u++) {
        for (int v = 0; v < N; v++) {
            double rSum = 0.0, iSum = 0.0;
            for (int x = 0; x < M; x++) {
                for (int y = 0; y < N; y++) {
                    double aci = 2.0 * CV_PI * ((u * x) / (double)M + (v * y) / (double)N);
                    double pixel = gri.at<double>(x, y) * pow(-1, x + y);
                    rSum += pixel * cos(aci);
                    iSum -= pixel * sin(aci);
                }
            }
            harmonikReal.at<double>(u, v) = rSum;
            harmonikImag.at<double>(u, v) = iSum;
        }
    }

    int u0 = M / 2;
    int v0 = N / 2;
    for (int u = 0; u < M; u++) {
        for (int v = 0; v < N; v++) {
            double D = sqrt((u - u0) * (u - u0) + (v - v0) * (v - v0));
            double H = 1.0;

            if (filtreTuru == 1) {
                H = (D <= D0) ? 1.0 : 0.0;
            } else if (filtreTuru == 2) {
                H = (D > D0) ? 1.0 : 0.0;
            } else if (filtreTuru == 3) {
                H = (abs(D - D0) <= W / 2.0) ? 1.0 : 0.0;
            } else if (filtreTuru == 4) {
                H = (abs(D - D0) > W / 2.0) ? 1.0 : 0.0;
            }

            harmonikReal.at<double>(u, v) *= H;
            harmonikImag.at<double>(u, v) *= H;
        }
    }

    cikis = Mat::zeros(M, N, CV_64F);
    for (int x = 0; x < M; x++) {
        for (int y = 0; y < N; y++) {
            double toplam = 0.0;
            for (int u = 0; u < M; u++) {
                for (int v = 0; v < N; v++) {
                    double aci = 2.0 * CV_PI * ((u * x) / (double)M + (v * y) / (double)N);
                    toplam += harmonikReal.at<double>(u, v) * cos(aci) - harmonikImag.at<double>(u, v) * sin(aci);
                }
            }
            cikis.at<double>(x, y) = (toplam / (M * N)) * pow(-1, x + y);
        }
    }
    cikis.convertTo(cikis, CV_8UC1);
}

int main() {
    Mat resim = imread("pisa.jpg", IMREAD_GRAYSCALE);
    if (resim.empty()) {
        resim = Mat(64, 64, CV_8UC1, Scalar(128));
    }

    Mat kucukResim, lpfCikis, hpfCikis, bpfCikis, brfCikis;
    resize(resim, kucukResim, Size(64,
