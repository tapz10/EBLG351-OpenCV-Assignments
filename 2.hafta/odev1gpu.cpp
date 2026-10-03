#include <opencv2/opencv.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <iostream>
#include <string>

void analizYap(std::string dosyaAdi) {
    cv::Mat resimCPU = cv::imread(dosyaAdi, cv::IMREAD_GRAYSCALE);

    if (resimCPU.empty()) {
        std::cout << dosyaAdi << " bulunamadi" << std::endl;
        return;
    }

    std::cout << dosyaAdi << std::endl;

    int64 baslangic = cv::getTickCount();

    cv::cuda::GpuMat resimGPU;
    resimGPU.upload(resimCPU);

    cv::cuda::Stream stream;
    stream.waitForCompletion();

    cv::Scalar ortalamaVal = cv::mean(resimCPU);
    cv::Mat meanMat, stddevMat;
    cv::meanStdDev(resimCPU, meanMat, stddevMat);

    int64 bitis = cv::getTickCount();
    double sure = (bitis - baslangic) / cv::getTickFrequency();

    std::cout << "Ortalama: " << ortalamaVal[0] << std::endl;
    std::cout << "Standart Sapma: " << stddevMat.at<double>(0, 0) << std::endl;
    std::cout << "Sure: " << sure << " saniye" << std::endl;
}

int main() {
    analizYap("image.jpg.jpeg");
    analizYap("image1.jpg.jpeg");

    return 0;
}
