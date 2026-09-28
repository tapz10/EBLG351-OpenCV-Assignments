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
    parcalar[0] = resim(cv::Rect(0, 0, w, h));
    parcalar[1] = resim(cv::Rect(w, 0, resim.cols - w, h));
    parcalar[2] = resim(cv::Rect(0, h, w, resim.rows - h));
    parcalar[3] = resim(cv::Rect(w, h, resim.cols - w, resim.rows - h));

    std::string parcaisimleri[4] = { "Sol Ust", "Sag Ust", "Sol Alt", "Sag Alt" };

    int histSize = 256;
    float range[] = { 0, 256 };
    const float* histRange = { range };

    cv::Mat anaHist, parcaHist[4];

    cv::calcHist(&resim, 1, 0, cv::Mat(), anaHist, 1, &histSize, &histRange);

    for (int i = 0; i < 4; i++) {
        cv::calcHist(&parcalar[i], 1, 0, cv::Mat(), parcaHist[i], 1, &histSize, &histRange);

        float toplamPiksel = 0;
        for (int b = 0; b < 256; b++) {
            toplamPiksel += parcaHist[i].at<float>(b);
        }
        std::cout << parcaisimleri[i] << " Toplam Piksel: " << toplamPiksel << std::endl;

        cv::imshow(parcaisimleri[i], parcalar[i]);
    }

    float anaToplam = 0;
    for (int b = 0; b < 256; b++) {
        anaToplam += anaHist.at<float>(b);
    }
    std::cout << "Ana Resim Toplam Piksel: " << anaToplam << std::endl;

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
