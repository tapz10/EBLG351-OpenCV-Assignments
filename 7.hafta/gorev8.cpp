#include <iostream>
#include <opencv2/opencv.hpp>
#include <random>

using namespace cv;
using namespace std;

void gaussGurultuEkle(const Mat& giris, Mat& cikis, double ortalama, double sapma) {
    cikis = giris.clone();
    cikis.convertTo(cikis, CV_64F);
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<double> dist(ortalama, sapma);

    for (int i = 0; i < cikis.rows; i++) {
        for (int j = 0; j < cikis.cols; j++) {
            double val = cikis.at<double>(i, j) + dist(gen);
            cikis.at<double>(i, j) = max(0.0, min(255.0, val));
        }
    }
    cikis.convertTo(cikis, CV_8UC1);
}

void saltAndPepperEkle(const Mat& giris, Mat& cikis, double oran) {
    cikis = giris.clone();
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dist(0.0, 1.0);

    int toplamPiksel = cikis.rows * cikis.cols;
    int gurultuSayisi = static_cast<int>(toplamPiksel * oran);

    for (int k = 0; k < gurultuSayisi; k++) {
        int r = gen() % cikis.rows;
        int c = gen() % cikis.cols;
        if (dist(gen) < 0.5) {
            cikis.at<uchar>(r, c) = 0;
        } else {
            cikis.at<uchar>(r, c) = 255;
        }
    }
}

Mat dft2D(const Mat& giris) {
    Mat gri;
    if (giris.channels() == 3) cvtColor(giris, gri, COLOR_BGR2GRAY);
    else gri = giris.clone();
    
    gri.convertTo(gri, CV_64F);
    int M = gri.rows;
    int N = gri.cols;
    Mat harmonik = Mat::zeros(M, N, CV_64F);

    for (int u = 0; u < M; u++) {
        for (int v = 0; v < N; v++) {
            double realSum = 0.0;
            double imagSum = 0.0;
            for (int x = 0; x < M; x++) {
                for (int y = 0; y < N; y++) {
                    double aci = 2.0 * CV_PI * ((u * x) / (double)M + (v * y) / (double)N);
                    double pixel = gri.at<double>(x, y);
                    realSum += pixel * cos(aci);
                    imagSum -= pixel * sin(aci);
                }
            }
            harmonik.at<double>(u, v) = sqrt(realSum * realSum + imagSum * imagSum) / (M * N);
        }
    }
    return harmonik;
}

int main() {
    Mat resim = imread("pisa.jpg", IMREAD_GRAYSCALE);
    if (resim.empty()) {
        resim = Mat(64, 64, CV_8UC1, Scalar(1
