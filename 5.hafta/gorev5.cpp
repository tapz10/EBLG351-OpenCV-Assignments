#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;
using namespace std;

void bgrToGrayCustom(const Mat& bgr, Mat& gray) {
    gray.create(bgr.rows, bgr.cols, CV_8UC1);
    for (int y = 0; y < bgr.rows; y++) {
        for (int x = 0; x < bgr.cols; x++) {
            Vec3b p = bgr.at<Vec3b>(y, x);
            int b = p[0], g = p[1], r = p[2];
            int gri = saturate_cast<uchar>(0.114 * b + 0.587 * g + 0.299 * r);
            gray.at<uchar>(y, x) = gri;
        }
    }
}

void bgrToYCrCbCustom(const Mat& bgr, Mat& ycrcb) {
    ycrcb.create(bgr.rows, bgr.cols, CV_8UC3);
    for (int y = 0; y < bgr.rows; y++) {
        for (int x = 0; x < bgr.cols; x++) {
            Vec3b p = bgr.at<Vec3b>(y, x);
            double b = p[0], g = p[1], r = p[2];

            uchar Y  = saturate_cast<uchar>(0.299 * r + 0.587 * g + 0.114 * b);
            uchar Cr = saturate_cast<uchar>((r - Y) * 0.713 + 128);
            uchar Cb = saturate_cast<uchar>((b - Y) * 0.564 + 128);

            ycrcb.at<Vec3b>(y, x) = Vec3b(Y, Cr, Cb);
        }
    }
}

int main() {
    Mat resim(512, 512, CV_8UC3);
    randu(resim, Scalar(0, 0, 0), Scalar(256, 256, 256));

    Mat bizimGray, opencvGray, fark;
    
    bgrToGrayCustom(resim, bizimGray);
    cvtColor(resim, opencvGray, COLOR_BGR2GRAY);

    absdiff(bizimGray, opencvGray, fark);
    double toplamHata = sum(fark)[0];

    cout << "Toplam Sayisal Hata: " << toplamHata << endl;
    cout << "Kod basariyla calisti!" << endl;

    return 0;
}
