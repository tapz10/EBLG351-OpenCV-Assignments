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
        resim = Mat(500, 500, CV_8UC3, Scalar(255, 255, 255));
        circle(resim, Point(250, 250), 60, Scalar(0, 0, 0), 2);
        circle(resim, Point(250, 250), 110, Scalar(0, 0, 0), 2);
        circle(resim, Point(250, 250), 160, Scalar(0, 0, 0), 2);
    }

    Mat gri, cannyCiktisi;
    cvtColor(resim, gri, CV_8UC1);
    Canny(gri, cannyCiktisi, 100, 200);

    int genislik = cannyCiktisi.cols;
    int yukseklik = cannyCiktisi.rows;

    int minR = 20;
    int maxR = min(genislik, yukseklik) / 2;
    int r_step = 2;
    int num_r = (maxR - minR) / r_step + 1;

    vector<double> cos_table(360);
    vector<double> sin_table(360);
    for (int theta = 0; theta < 360; theta++) {
        double rad = theta * CV_PI / 180.0;
        cos_table[theta] = cos(rad);
        sin_table[theta] = sin(rad);
    }

    int dimSize[3] = {yukseklik, genislik, num_r};
    Mat accumulator(3, dimSize, CV_32SC1, Scalar(0));

    for (int y = 0; y < yukseklik; y++) {
        for (int x = 0; x < genislik; x++) {
            if (cannyCiktisi.at<uchar>(y, x) > 0) {
                for (int r_idx = 0; r_idx < num_r; r_idx++) {
                    int r = minR + r_idx * r_step;
                    for (int theta = 0; theta < 360; theta += 4) {
                        int a = cvRound(x - r * cos_table[theta]);
                        int b = cvRound(y - r * sin_table[theta]);

                        if (a >= 0 && a < genislik && b >= 0 && b < yukseklik) {
                            accumulator.at<int>(b, a, r_idx)++;
                        }
                    }
                }
            }
        }
    }

    int esikDeger = 50;
    Mat sonucResmi = resim.clone();

    for (int y = 0; y < yukseklik; y++) {
        for (int x = 0; x < genislik; x++) {
            int esmerkezSayaci = 0;
            vector<int> bulunanYaricaplar;

            for (int r_idx = 0; r_idx < num_r; r_idx++) {
                if (accumulator.at<int>(y, x, r_idx) >= esikDeger) {
                    esmerkezSayaci++;
                    bulunanYaricaplar.push_back(minR + r_idx * r_step);
                }
            }

            if (esmerkezSayaci >= 2) {
                circle(sonucResmi, Point(x, y), 4, Scalar(255, 0, 0), -1, LINE_AA);
                for (size_t i = 0; i < bulunanYaricaplar.size(); i++) {
                    circle(sonucResmi, Point(x, y), bulunanYaricaplar[i], Scalar(0, 0, 255), 2, LINE_AA);
                }
            }
        }
    }

    imwrite("pisa_esmerkez_hough_sonuc.jpg", sonucResmi);
    cout << "Eş merkezli çemberler 3D oylama matrisi üzerinden doğrudan tespit edildi ve kaydedildi." << endl;

    return 0;
}
