#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>
#include <random>

using namespace cv;
using namespace std;

void gurultuEkle(vector<double>& vektor, double gurultuOrani) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dist(-1.0, 1.0);

    double maxGenlik = 0.0;
    for (double val : vektor) {
        if (abs(val) > maxGenlik) {
            maxGenlik = abs(val);
        }
    }
    if (maxGenlik == 0.0) maxGenlik = 1.0;

    for (size_t i = 0; i < vektor.size(); i++) {
        double gurultuMiktari = dist(gen) * maxGenlik * gurultuOrani;
        vektor[i] += gurultuMiktari;
    }
}

int main() {
    int boyut = 512;
    vector<double> sinyal(boyut);

    for (int i = 0; i < boyut; i++) {
        double t = static_cast<double>(i) / boyut;
        sinyal[i] = 100.0 * sin(2.0 * CV_PI * 3.0 * t);
    }

    gurultuEkle(sinyal, 0.2);

    int imgGenislik = 800;
    int imgYukseklik = 400;
    Mat grafik(imgYukseklik, imgGenislik, CV_8UC3, Scalar(255, 255, 255));

    line(grafik, Point(50, imgYukseklik / 2), Point(imgGenislik - 50, imgYukseklik / 2), Scalar(200, 200, 200), 1);

    double adimX = static_cast<double>(imgGenislik - 100) / (boyut - 1);
    double merkezY = imgYukseklik / 2.0;

    for (int i = 0; i < boyut - 1; i++) {
        int x1 = 50 + cvRound(i * adimX);
        int y1 = cvRound(merkezY - sinyal[i]);
        int x2 = 50 + cvRound((i + 1) * adimX);
        int y2 = cvRound(merkezY - sinyal[i + 1]);

        line(grafik, Point(x1, y1), Point(x2, y2), Scalar(0, 0, 255), 2, LINE_AA);
    }

    imwrite("gurultulu_sinus_gorsel.jpg", grafik);
    cout << "Gurultu ekleme islemi basariyla tamamlandi." << endl;

    return 0;
}
