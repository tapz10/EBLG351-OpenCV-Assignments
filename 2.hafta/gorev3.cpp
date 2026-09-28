#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

int main() {
    cv::Mat resim = cv::imread("vangogh.jpg", cv::IMREAD_GRAYSCALE);

    if (resim.empty()) {
        return -1;
    }

    int w = resim.cols / 2;
    int h = resim.rows / 2;

    cv::Mat parcalar[4];
    parcalar[0] = resim(cv::Rect(0, 0, w, h)).clone();
    parcalar[1] = resim(cv::Rect(w, 0, resim.cols - w, h)).clone();
    parcalar[2] = resim(cv::Rect(0, h, w, resim.rows - h)).clone();
    parcalar[3] = resim(cv::Rect(w, h, resim.cols - w, resim.rows - h)).clone();

    cv::Mat bolunenler[4], carpilanlar[4];
    std::string parcaisimleri[4] = { "Sol Ust", "Sag Ust", "Sol Alt", "Sag Alt" };

    int64 baslangic = cv::getTickCount();

    for (int i = 0; i < 4; i++) {
        bolunenler[i] = parcalar[i].clone();
        carpilanlar[i] = parcalar[i].clone();

        for (int y = 0; y < parcalar[i].rows; y++) {
            for (int x = 0; x < parcalar[i].cols; x++) {
                uchar piksel = parcalar[i].at<uchar>(y, x);
                bolunenler[i].at<uchar>(y, x) = piksel / 4;
                carpilanlar[i].at<uchar>(y, x) = cv::saturate_cast<uchar>(piksel * 4);
            }
        }
    }

    int64 bitis = cv::getTickCount();
    double sure = (bitis - baslangic) / cv::getTickFrequency();
    std::cout << "4 parcali isleme suresi: " << sure << " saniye" << std::endl;

    for (int i = 0; i < 4; i++) {
        cv::imshow(parcaisimleri[i], parcalar[i]);
        cv::imshow(parcaisimleri[i] + " / 4", bolunenler[i]);
        cv::imshow(parcaisimleri[i] + " * 4", carpilanlar[i]);
    }

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
