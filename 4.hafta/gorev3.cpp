#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace std;
using namespace cv;

int main() {
    Mat resim = imread("/Users/tapz/Desktop/jairzinho.jpg", IMREAD_GRAYSCALE);
    if (resim.empty()) {
        cout << "HATA" << endl;
        return -1;
    }

    Mat yumusatilmis = resim.clone();
    int gauss[3][3] = {
        {1, 2, 1},
        {2, 4, 2},
        {1, 2, 1}
    };

    for (int y = 1; y < resim.rows - 1; y++) {
        for (int x = 1; x < resim.cols - 1; x++) {
            int toplam = 0;
            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    toplam += resim.at<uchar>(y + ky, x + kx) * gauss[ky + 1][kx + 1];
                }
            }
            yumusatilmis.at<uchar>(y, x) = saturate_cast<uchar>(toplam / 16);
        }
    }

    Mat sobel_x = Mat::zeros(resim.size(), CV_8UC1);
    Mat sobel_y = Mat::zeros(resim.size(), CV_8UC1);

    int gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int gy[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    for (int y = 1; y < yumusatilmis.rows - 1; y++) {
        for (int x = 1; x < yumusatilmis.cols - 1; x++) {
            int toplam_x = 0;
            int toplam_y = 0;
            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    int piksel = yumusatilmis.at<uchar>(y + ky, x + kx);
                    toplam_x += piksel * gx[ky + 1][kx + 1];
                    toplam_y += piksel * gy[ky + 1][kx + 1];
                }
            }
            sobel_x.at<uchar>(y, x) = saturate_cast<uchar>(abs(toplam_x));
            sobel_y.at<uchar>(y, x) = saturate_cast<uchar>(abs(toplam_y));
        }
    }

    imwrite("sobel_x_sonuc.jpg", sobel_x);
    imwrite("sobel_y_sonuc.jpg", sobel_y);
    cout << "Basarili!" << endl;

    imshow("Orijinal", resim);
    imshow("Gauss Yumusatma", yumusatilmis);
    imshow("Sobel X (Dikey Kenar)", sobel_x);
    imshow("Sobel Y (Yatay Kenar)", sobel_y);
    waitKey(0);

    return 0;
}
