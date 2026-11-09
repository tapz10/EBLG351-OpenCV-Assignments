#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

using namespace cv;
using namespace std;

void vektorGaussFiltrele(const vector<double>& giris, vector<double>& cikis, int r, double sigma) {
    int boyut = giris.size();
    cikis.resize(boyut);
    vector<double> kernel(2 * r + 1);
    double toplam = 0.0;
    
    for (int i = -r; i <= r; i++) {
        kernel[i + r] = exp(-(i * i) / (2.0 * sigma * sigma));
        toplam += kernel[i + r];
    }
    for (double& val : kernel) val /= toplam;

    for (int i = 0; i < boyut; i++) {
        double val = 0.0;
        for (int j = -r; j <= r; j++) {
            int idx = i + j;
            if (idx >= 0 && idx < boyut) val += giris[idx] * kernel[j + r];
        }
        cikis[i] = val;
    }
}

void vektorMedianFiltrele(const vector<double>& giris, vector<double>& cikis, int r) {
    int boyut = giris.size();
    cikis.resize(boyut);
    for (int i = 0; i < boyut; i++) {
        vector<double> pencere;
        for (int j = -r; j <= r; j++) {
            int idx = i + j;
            if (idx >= 0 && idx < boyut) pencere.push_back(giris[idx]);
        }
        sort(pencere.begin(), pencere.end());
        cikis[i] = pencere[pencere.size() / 2];
    }
}

int main() {
    int boyut = 512;
    vector<double> sinyal(boyut);
    for (int i = 0; i < boyut; i++) sinyal[i] = 100.0 * sin(2.0 * CV_PI * 3.0 * i / boyut);

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dist(-1.0, 1.0);
    for (double& val : sinyal) val += dist(gen) * 20.0;

    vector<double> gaussCikis, medianCikis;
    vektorGaussFiltrele(sinyal, gaussCikis, 3, 2.0);
    vektorMedianFiltrele(sinyal, medianCikis, 3);

    Mat grafik(400, 800, CV_8UC3, Scalar(255, 255, 255));
    line(grafik, Point(50, 200), Point(750, 200), Scalar(200, 200, 200), 1);

    double adimX = 700.0 / (boyut - 1);
    for (int i = 0; i < boyut - 1; i++) {
        int x1 = 50 + cvRound(i * adimX);
        int y1 = cvRound(200 - gaussCikis[i]);
        int x2 = 50 + cvRound((i + 1) * adimX);
        int y2 = cvRound(200 - gaussCikis[i + 1]);
        line(grafik, Point(x1, y1), Point(x2, y2), Scalar(255, 0, 0), 2, LINE_AA);
    }

    imwrite("filtre_sonuc.jpg", grafik);
    cout << "Filtreleme tamamlandi." << endl;

    return 0;
}
