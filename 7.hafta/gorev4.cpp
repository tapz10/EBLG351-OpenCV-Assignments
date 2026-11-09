#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>
#include <random>

using namespace cv;
using namespace std;

void dftHarmonikHesapla(const vector<double>& giris, vector<double>& harmonik) {
    int N = giris.size();
    harmonik.resize(N / 2);
    
    for (int k = 0; k < N / 2; k++) {
        double realSum = 0.0;
        double imagSum = 0.0;
        for (int n = 0; n < N; n++) {
            double aci = 2.0 * CV_PI * k * n / N;
            realSum += giris[n] * cos(aci);
            imagSum -= giris[n] * sin(aci);
        }
        harmonik[k] = sqrt(realSum * realSum + imagSum * imagSum) / (N / 2);
    }
}

int main() {
    int boyut = 256;
    vector<double> temizSinyal(boyut);
    vector<double> gurultuluSinyal(boyut);

    for (int i = 0; i < boyut; i++) {
        double deger = 100.0 * sin(2.0 * CV_PI * 5.0 * i / boyut);
        temizSinyal[i] = deger;
        gurultuluSinyal[i] = deger;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dist(-1.0, 1.0);
    for (double& val : gurultuluSinyal) {
        val += dist(gen) * 30.0;
    }

    vector<double> harmonikTemiz, harmonikGurultulu;
    dftHarmonikHesapla(temizSinyal, harmonikTemiz);
    dftHarmonikHesapla(gurultuluSinyal, harmonikGurultulu);

    Mat grafik(400, 800, CV_8UC3, Scalar(255, 255, 255));
    line(grafik, Point(50, 350), Point(750, 350), Scalar(200, 200, 200), 1);

    double adimX = 700.0 / (harmonikTemiz.size() - 1);
    for (size_t i = 0; i < harmonikTemiz.size() - 1; i++) {
        int x1 = 50 + cvRound(i * adimX);
        int y1 = cvRound(350 - harmonikTemiz[i]);
        int x2 = 50 + cvRound((i + 1) * adimX);
        int y2 = cvRound(350 - harmonikTemiz[i + 1]);
        line(grafik, Point(x1, y1), Point(x2, y2), Scalar(0, 255, 0), 2, LINE_AA);
    }

    for (size_t i = 0; i < harmonikGurultulu.size() - 1; i++) {
        int x1 = 50 + cvRound(i * adimX);
        int y1 = cvRound(350 - harmonikGurultulu[i]);
        int x2 = 50 + cvRound((i + 1) * adimX);
        int y2 = cvRound(350 - harmonikGurultulu[i + 1]);
        line(grafik, Point(x1, y1), Point(x2, y2), Scalar(0, 0, 255), 1, LINE_AA);
    }

    imwrite("dft_harmonik_sonuc.jpg", grafik);
    cout << "DFT ve harmonik analizi tamamlandi, sonuc kaydedildi." << endl;

    return 0;
}
