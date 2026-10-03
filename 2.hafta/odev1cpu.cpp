#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

void analizYap(std::string dosyaAdi) {
    cv::Mat resim = cv::imread(dosyaAdi, cv::IMREAD_GRAYSCALE);
    
    if (resim.empty()) {
        std::cout << dosyaAdi << " bulunamadi" << std::endl;
        return;
    }

    std::cout << dosyaAdi << std::endl;

    cv::Mat mean, stddev;
    int64 baslangic = cv::getTickCount();
    
    cv::meanStdDev(resim, mean, stddev);
    
    int64 bitis = cv::getTickCount();
    double sure = (bitis - baslangic) / cv::getTickFrequency();

    std::cout << "Ortalama: " << mean.at<double>(0, 0) << std::endl;
    std::cout << "Standart Sapma: " << stddev.at<double>(0, 0) << std::endl;
    std::cout << "Sure: " << sure << " saniye" << std::endl;
}

int main() {
    analizYap("Image.jpg.jpeg");
    analizYap("Image1.jpg.jpeg");

    return 0;
}
