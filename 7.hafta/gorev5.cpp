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
        double real = 0.0, imag = 0.0;
        for (int n = 0; n < N; n++) {
            double aci = 2.0 * CV_PI * k * n / N;
            real += giris[n] * cos(aci);
            imag -= giris[n] * sin(aci);
        }
        harmonik[k] = sqrt(real * real + imag * imag) / (N / 2);
    }
}

int main() {
    int boyut = 256;
    vector<double> toplamSinyal(boyut, 0.0);
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dist(-1.0, 1.0);

    for (int i = 0; i < boyut; i++) {
        double t = static_cast<double>(i) / boyut;
        double s1 = 12.0 * sin(2.0 * CV_PI * 50.0 * t) + dist(gen) * 2.0;
        double s2 = 5.0 * sin(2.0 * CV_PI * 30.0 * t) + dist(gen) * 2.0;
        toplamSinyal[i] = s1 + s2;
    }

    vector<double> harmonik;
    dftHarmonikHesapla(toplamSinyal, harmonik);

    Mat grafik(400, 800, CV_8UC3, Scalar(255, 255, 255));
    double adim = 700.0 / (harmonik.size() - 1);
    for (size_t i = 0; i < harmonik.size() - 1; i++) {
        int x1 = 50 + cvRound(i * adim);
        int y1 = cvRound(350 - harmonik[i] * 5.0);
        int x2 = 50 + cvRound((i + 1) * adim);
        int y2 = cvRound(350 - harmonik[i + 1] * 5.0);
        line(grafik, Point(x1, y1), Point(x2, y2), Scalar(255, 0, 0), 2, LINE_AA);
    }

    imwrite("dft_kisa_sonuc.jpg", grafik);
    cout << "Islem tamamlandi." << endl;

    return 0;
}
