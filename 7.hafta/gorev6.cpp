#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>
#include <random>

using namespace cv;
using namespace std;

void dftIslemi(const vector<double>& giris, vector<double>& realPart, vector<double>& imagPart) {
    int N = giris.size();
    realPart.assign(N, 0.0);
    imagPart.assign(N, 0.0);
    for (int k = 0; k < N; k++) {
        for (int n = 0; n < N; n++) {
            double aci = 2.0 * CV_PI * k * n / N;
            realPart[k] += giris[n] * cos(aci);
            imagPart[k] -= giris[n] * sin(aci);
        }
    }
}

void idftIslemi(const vector<double>& realPart, const vector<double>& imagPart, vector<double>& cikis) {
    int N = realPart.size();
    cikis.resize(N);
    for (int n = 0; n < N; n++) {
        double toplam = 0.0;
        for (int k = 0; k < N; k++) {
            double aci = 2.0 * CV_PI * k * n / N;
            toplam += realPart[k] * cos(aci) - imagPart[k] * sin(aci);
        }
        cikis[n] = toplam / N;
    }
}

int main() {
    int boyut = 256;
    vector<double> toplamSinyal(boyut);
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dist(-1.0, 1.0);

    for (int i = 0; i < boyut; i++) {
        double t = static_cast<double>(i) / boyut;
        double s1 = 12.0 * sin(2.0 * CV_PI * 50.0 * t) + dist(gen) * 2.0;
        double s2 = 5.0 * sin(2.0 * CV_PI * 30.0 * t) + dist(gen) * 2.0;
        toplamSinyal[i] = s1 + s2;
    }

    vector<double> realPart, imagPart;
    dftIslemi(toplamSinyal, realPart, imagPart);

    double esikDeger = 500.0; 
    for (size_t k = 0; k < realPart.size(); k++) {
        double genlik = sqrt(realPart[k] * realPart[k] + imagPart[k] * imagPart[k]);
        if (genlik < esikDeger) {
            realPart[k] = 0.0;
            imagPart[k] = 0.0;
        }
    }

    vector<double> yenidenUretilen;
    idftIslemi(realPart, imagPart, yenidenUretilen);

    Mat grafik(400, 800, CV_8UC3, Scalar(255, 255, 255));
    double adim = 700.0 / (boyut - 1);
    for (int i = 0; i < boyut - 1; i++) {
        int x1 = 50 + cvRound(i * adim);
        int y1 = cvRound(200 - yenidenUretilen[i] * 5.0);
        int x2 = 50 + cvRound((i + 1) * adim);
        int y2 = cvRound(200 - yenidenUretilen[i + 1] * 5.0);
        line(grafik, Point(x1, y1), Point(x2, y2), Scalar(0, 150, 0), 2, LINE_AA);
    }

    imwrite("frekans_filtre_sonuc.jpg", grafik);
    cout << "Frekans filtresi ve IDFT ile yeniden uretim tamamlandi." << endl;

    return 0;
}
