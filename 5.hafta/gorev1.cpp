#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

int main() {
    Mat resim = imread("/Users/tapz/Desktop/renkli.jpg", IMREAD_COLOR);
    if (resim.empty()) {
        cout << "HATA" << endl;
        return -1;
    }

    int hist_r[256] = {0};
    int hist_g[256] = {0};
    int hist_b[256] = {0};

    for (int y = 0; y < resim.rows; y++) {
        for (int x = 0; x < resim.cols; x++) {
            Vec3b pikseller = resim.at<Vec3b>(y, x);
            hist_b[pikseller[0]]++;
            hist_g[pikseller[1]]++;
            hist_r[pikseller[2]]++;
        }
    }

    int grafik_h = 512;
    int grafik_w = 512;

    int max_r = 0, max_g = 0, max_b = 0;
    for (int i = 0; i < 256; i++) {
        if (hist_r[i] > max_r) max_r = hist_r[i];
        if (hist_g[i] > max_g) max_g = hist_g[i];
        if (hist_b[i] > max_b) max_b = hist_b[i];
    }

    int bin_w = cvRound((double)grafik_w / 256);

    Mat hist_b_gorsel = Mat::zeros(grafik_h, grafik_w, CV_8UC3);
    Mat hist_g_gorsel = Mat::zeros(grafik_h, grafik_w, CV_8UC3);
    Mat hist_r_gorsel = Mat::zeros(grafik_h, grafik_w, CV_8UC3);

    for (int i = 0; i < 255; i++) {
        line(hist_b_gorsel, Point(i * bin_w, grafik_h - cvRound((double)hist_b[i] * grafik_h / max_b)),
              Point((i + 1) * bin_w, grafik_h - cvRound((double)hist_b[i + 1] * grafik_h / max_b)), Scalar(255, 0, 0), 2);

        line(hist_g_gorsel, Point(i * bin_w, grafik_h - cvRound((double)hist_g[i] * grafik_h / max_g)),
              Point((i + 1) * bin_w, grafik_h - cvRound((double)hist_g[i + 1] * grafik_h / max_g)), Scalar(0, 255, 0), 2);

        line(hist_r_gorsel, Point(i * bin_w, grafik_h - cvRound((double)hist_r[i] * grafik_h / max_r)),
              Point((i + 1) * bin_w, grafik_h - cvRound((double)hist_r[i + 1] * grafik_h / max_r)), Scalar(0, 0, 255), 2);
    }

    imwrite("histogram_mavi.jpg", hist_b_gorsel);
    imwrite("histogram_yesil.jpg", hist_g_gorsel);
    imwrite("histogram_kirmizi.jpg", hist_r_gorsel);

    cout << "Basarili!" << endl;

    imshow("Orijinal Resim", resim);
    imshow("Mavi Kanal Histogrami", hist_b_gorsel);
    imshow("Yesil Kanal Histogrami", hist_g_gorsel);
    imshow("Kirmizi Kanal Histogrami", hist_r_gorsel);
    waitKey(0);

    return 0;
}
