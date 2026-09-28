#include <opencv2/opencv.hpp>

int main() {
    cv::Mat insanresmi = cv::imread("vangogh.jpg", cv::IMREAD_GRAYSCALE);

    if (insanresmi.empty()) {
        return -1;
    }

    cv::Mat bolunen = insanresmi.clone();
    cv::Mat carpilan = insanresmi.clone();

    for (int y = 0; y < insanresmi.rows; y++) {
        for (int x = 0; x < insanresmi.cols; x++) {
            uchar piksel = insanresmi.at<uchar>(y, x);

            bolunen.at<uchar>(y, x) = piksel / 4;
            carpilan.at<uchar>(y, x) = cv::saturate_cast<uchar>(piksel * 4);
        }
    }

    cv::imshow("Orijinal Resim", insanresmi);
    cv::imshow("4ilebolunen", bolunen);
    cv::imshow("4ilecarpilan", carpilan);

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
