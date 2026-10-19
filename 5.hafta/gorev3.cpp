#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace std;
using namespace cv;

Mat ozelGriDonustur(const Mat& resim) {
    Mat gri_ozel = Mat::zeros(resim.size(), CV_8UC1);
    for (int y = 0; y < resim.rows; y++) {
        for (int x = 0; x < resim.cols; x++) {
            Vec3b bgr = resim.at<Vec3b>(y, x);
            uchar b = bgr[0];
            uchar g = bgr[1];
            uchar r = bgr[2];
            
            uchar gri = saturate_cast<uchar>(0.299 * r + 0.587 * g + 0.114 * b);
            gri_ozel.at<uchar>(y, x) = gri;
        }
    }
    return gri_ozel;
}

int main() {
    Mat resim = imread("/Users/tapz/Desktop/renkli.jpg", IMREAD_COLOR);
    if (resim.empty()) {
        cout << "HATA" << endl;
        return -1;
    }

    Mat gri_opencv;
    cvtColor(resim, gri_opencv, COLOR_BGR2GRAY);

    Mat gri_ozel = ozelGriDonustur(resim);

    long long toplam_hata = 0;
    for (int y = 0; y < resim.rows; y++) {
        for (int x = 0; x < resim.cols; x++) {
            int fark = abs((int)gri_opencv.at<uchar>(y, x) - (int)gri_ozel.at<uchar>(y, x));
            toplam_hata += fark;
        }
    }

    imwrite("gri_opencv.jpg", gri_opencv);
    imwrite("gri_ozel.jpg", gri_ozel);

    cout << "Basarili!" << endl;
    cout << "Toplam Mutlak Fark (Hata Degeri): " << toplam_hata << endl;

    imshow("Orijinal Resim", resim);
    imshow("OpenCV Gri", gri_opencv);
    imshow("Ozel Fonksiyon Gri", gri_ozel);
    waitKey(0);

    return 0;
}
