#include <opencv2/opencv.hpp>

int main() {
    cv::Mat resim = cv::imread("vangogh.jpg", cv::IMREAD_GRAYSCALE);

    if (resim.empty()) {
        return -1;
    }

    cv::Mat carpilan = resim.clone();
    cv::Mat eklenen = resim.clone();

    for (int y = 0; y < resim.rows; y++) {
        for (int x = 0; x < resim.cols; x++) {
            uchar piksel = resim.at<uchar>(y, x);

            carpilan.at<uchar>(y, x) = cv::saturate_cast<uchar>(piksel * 0.75);
            eklenen.at<uchar>(y, x) = cv::saturate_cast<uchar>(piksel + 20);
        }
    }

    cv::imshow("Orijinal", resim);
    cv::imshow("0.75 ile Carpilmis", carpilan);
    cv::imshow("+20 Eklenmis", eklenen);

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
