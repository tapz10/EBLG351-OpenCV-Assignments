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
        circle(resim, Point(200, 200), 80, Scalar(0, 0, 0), 3);
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
                    for (int theta = 0; theta < 360; theta += 5) {
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

    int esikDeger = 45;
    Mat sonucResmi = resim.clone();

    for (int y = 0; y < yukseklik; y++) {
        for (int x = 0; x < genislik; x++) {
            for (int r_idx = 0; r_idx < num_r; r_idx++) {
                int oy = accumulator.at<int>(y, x, r_idx);
                if (oy >= esikDeger) {
                    int r = minR + r_idx * r_step;
                    circle(sonucResmi, Point(x, y), r, Scalar(0, 0, 255), 2, LINE_AA);
                    circle(sonucResmi, Point(x, y), 3, Scalar(255, 0, 0), -1, LINE_AA);
                }
            }
        }
    }

    imwrite("pisa_custom_hough_circles_sonuc.jpg", sonucResmi);
    cout << "Ozel 3D Hough Çember Dönüşümü başarıyla tamamlandı ve sonuç kaydedildi." << endl;

    return 0;
}
