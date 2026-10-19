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

    Mat laplacian_resim = Mat::zeros(resim.size(), CV_8UC1);
    int laplacian[3][3] = {
        {0,  1, 0},
        {1, -4, 1},
        {0,  1, 0}
    };

    for (int y = 1; y < yumusatilmis.rows - 1; y++) {
        for (int x = 1; x < yumusatilmis.cols - 1; x++) {
            int toplam = 0;
            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    toplam += yumusatilmis.at<uchar>(y + ky, x + kx) * laplacian[ky + 1][kx + 1];
                }
            }
            laplacian_resim.at<uchar>(y, x) = saturate_cast<uchar>(abs(toplam));
        }
    }

    imwrite("laplacian_sonuc.jpg", laplacian_resim);
    cout << "Basarili!" << endl;

    imshow("Orijinal", resim);
    imshow("Gauss Yumusatma", yumusatilmis);
    imshow("Laplacian Sonuc", laplacian_resim);
    waitKey(0);

    return 0;
}
