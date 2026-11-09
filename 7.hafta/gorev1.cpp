#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>

using namespace cv;
using namespace std;

void sinuzoidalVektorUret(vector<double>& vektor, double genlik, double frekans, double faz) {
    int boyut = vektor.size();
    for (int i = 0; i < boyut; i++) {
        double t = static_cast<double>(i) / boyut;
        vektor[i] = genlik * sin(2.0 * CV_PI * frekans * t + faz);
    }
}

int main() {
    int boyut = 512;
    vector<double> sinyal(boyut);

    double genlik = 100.0;
    double frekans = 3.0;
    double faz = CV_PI / 4.0;

    sinuzoidalVektorUret(sinyal, genlik, frekans, faz);

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

    imwrite("sinus_gorsel.jpg", grafik);
    cout << "Islem basariyla tamamlandi." << endl;

    return 0;
}
