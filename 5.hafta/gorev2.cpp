#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

void ozelCalcHist(const Mat& kanal, vector<int>& hist) {
    fill(hist.begin(), hist.end(), 0);
    for (int y = 0; y < kanal.rows; y++) {
        for (int x = 0; x < kanal.cols; x++) {
            uchar piksel = kanal.at<uchar>(y, x);
            hist[piksel]++;
        }
    }
}

int main() {
    Mat resim = imread("socrates.jpg", IMREAD_COLOR);
    if (resim.empty()) {
        cout << "HATA" << endl;
        return -1;
    }

    vector<Mat> kanallar;
    split(resim, kanallar);

    vector<int> hist_b(256, 0);
    vector<int> hist_g(256, 0);
    vector<int> hist_r(256, 0);

    ozelCalcHist(kanallar[0], hist_b);
    ozelCalcHist(kanallar[1], hist_g);
    ozelCalcHist(kanallar[2], hist_r);

    int grafik_h = 400;
    int grafik_w = 400;

    int max_b = *max_element(hist_b.begin(), hist_b.end());
    int max_g = *max_element(hist_g.begin(), hist_g.end());
    int max_r = *max_element(hist_r.begin(), hist_r.end());

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

    imwrite("ozel_histogram_mavi.jpg", hist_b_gorsel);
    imwrite("ozel_histogram_yesil.jpg", hist_g_gorsel);
    imwrite("ozel_histogram_kirmizi.jpg", hist_r_gorsel);

    cout << "Basarili!" << endl;

    return 0;
}
