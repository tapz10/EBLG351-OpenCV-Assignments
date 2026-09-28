#include <opencv2/opencv.hpp>

int main() {
    cv::Mat renkliResim = cv::imread("gorev1resmi.jpeg");

    if (renkliResim.empty()) {
        return -1;
    }

    cv::Mat griResim;
    cv::cvtColor(renkliResim, griResim, cv::COLOR_BGR2GRAY);

    int genislik = griResim.cols;
    int yukseklik = griResim.rows;

    cv::Mat resim_x2, resim_x4, resim_x0_5, resim_x0_25;

    cv::resize(griResim, resim_x2, cv::Size(genislik * 2, yukseklik * 2));
    cv::resize(griResim, resim_x4, cv::Size(genislik * 4, yukseklik * 4));
    cv::resize(griResim, resim_x0_5, cv::Size(genislik / 2, yukseklik / 2));
    cv::resize(griResim, resim_x0_25, cv::Size(genislik / 4, yukseklik / 4));

    cv::imshow("1 - Orijinal Siyah-Beyaz", griResim);
    cv::imshow("2 - Ceyrek Boyut (x0.25)", resim_x0_25);
    cv::imshow("3 - Yarim Boyut (x0.5)", resim_x0_5);
    cv::imshow("4 - Dört Kat Büyütm (x4)", resim_x4);
    cv::imshow("5 - İki Kat Büyütme (x2)", resim_x2);

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
