#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

using namespace cv;
using namespace std;

void bgrToGrayCustom(const Mat& bgr, Mat& gray) {
    gray.create(bgr.rows, bgr.cols, CV_8UC1);
    for (int y = 0; y < bgr.rows; y++) {
        for (int x = 0; x < bgr.cols; x++) {
            Vec3b p = bgr.at<Vec3b>(y, x);
            int Y = (p[0] * 1868 + p[1] * 9617 + p[2] * 4899 + (1 << 13)) >> 14;
            gray.at<uchar>(y, x) = saturate_cast<uchar>(Y);
        }
    }
}

void pseudoColorXRay(const Mat& gray, Mat& colored) {
    colored.create(gray.rows, gray.cols, CV_8UC3);

    Vec3b lut[256];
    for (int i = 0; i < 256; i++) {
        uchar b = 0, g = 0, r = 0;
        if (i < 60) {
            float t = i / 60.0f;
            b = saturate_cast<uchar>(180 + t * 75);
            g = saturate_cast<uchar>(t * 80);
            r = 0;
        } else if (i < 130) {
            float t = (i - 60) / 70.0f;
            b = saturate_cast<uchar>((1.0f - t) * 200);
            g = saturate_cast<uchar>(120 + t * 135);
            r = saturate_cast<uchar>(t * 100);
        } else if (i < 210) {
            float t = (i - 130) / 80.0f;
            b = 0;
            g = saturate_cast<uchar>(100 + t * 140);
            r = 255;
        } else {
            float t = (i - 210) / 45.0f;
            b = saturate_cast<uchar>(220 + t * 35);
            g = saturate_cast<uchar>(220 + t * 35);
            r = saturate_cast<uchar>(220 + t * 35);
        }
        lut[i] = Vec3b(b, g, r);
    }

    for (int y = 0; y < gray.rows; y++) {
        for (int x = 0; x < gray.cols; x++) {
            uchar val = gray.at<uchar>(y, x);
            colored.at<Vec3b>(y, x) = lut[val];
        }
    }
}

Mat sentetikXRayUret(int index) {
    Mat img(400, 400, CV_8UC3, Scalar(245, 245, 245));
    rectangle(img, Point(50, 70), Point(350, 330), Scalar(60, 140, 255), FILLED);
    circle(img, Point(130 + index * 20, 180), 45, Scalar(50, 180, 50), FILLED);
    rectangle(img, Point(180, 140 + index * 10), Point(290, 175 + index * 10), Scalar(180, 40, 10), FILLED);
    rectangle(img, Point(180, 175 + index * 10), Point(215, 240 + index * 10), Scalar(180, 40, 10), FILLED);
    GaussianBlur(img, img, Size(5, 5), 1.5);
    return img;
}

int main(int argc, char** argv) {
    string klasorYolu = (argc > 1) ? argv[1] : "ThreadsRGB";
    vector<String> dosyaYollari;
    
    try {
        glob(klasorYolu + "/*.*", dosyaYollari, false);
    } catch (...) {}

    int islenecekAdet = 5;

    for (int i = 0; i < islenecekAdet; i++) {
        Mat orijinal;
        string kaynakAdi;

        if (i < (int)dosyaYollari.size()) {
            orijinal = imread(dosyaYollari[i], IMREAD_COLOR);
            kaynakAdi = dosyaYollari[i];
        }

        if (orijinal.empty()) {
            orijinal = sentetikXRayUret(i);
            kaynakAdi = "Sentetik_XRay_" + to_string(i + 1);
        }

        Mat gri, renklendirilmis, fark;
        bgrToGrayCustom(orijinal, gri);
        pseudoColorXRay(gri, renklendirilmis);

        absdiff(orijinal, renklendirilmis, fark);
        Scalar s = sum(fark);
        double ortFark = (s[0] + s[1] + s[2]) / (orijinal.rows * orijinal.cols * 3.0);

        string OnEk = "xray_" + to_string(i + 1);
        imwrite(OnEk + "_1_orijinal.png", orijinal);
        imwrite(OnEk + "_2_gri.png", gri);
        imwrite(OnEk + "_3_renklendirilmis.png", renklendirilmis);

        cout << "[" << kaynakAdi << "]" << endl;
        cout << "Orijinal vs Sonradan Renklendirilmis Ortalama Piksel Farki: " << ortFark << "\n" << endl;
    }

    return 0;
}
