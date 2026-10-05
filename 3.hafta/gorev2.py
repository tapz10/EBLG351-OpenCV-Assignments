%%bash
cat << 'EOF' > gorev2.cpp
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
    Mat resim = imread("socrates.jpg", IMREAD_GRAYSCALE);
    if (resim.empty()) {
        cout << "HATA" << endl;
        return -1;
    }

    int toplam_piksel = resim.rows * resim.cols;

    int hist[256] = {0};
    for (int y = 0; y < resim.rows; y++) {
        for (int x = 0; x < resim.cols; x++) {
            int piksel = resim.at<uchar>(y, x);
            hist[piksel]++;
        }
    }

    int cdf[256] = {0};
    cdf[0] = hist[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + hist[i];
    }

    int cdf_min = 0;
    for (int i = 0; i < 256; i++) {
        if (cdf[i] > 0) {
            cdf_min = cdf[i];
            break;
        }
    }

    Mat yeni_resim = resim.clone();
    for (int y = 0; y < resim.rows; y++) {
        for (int x = 0; x < resim.cols; x++) {
            int piksel = resim.at<uchar>(y, x);
            
            int pay = cdf[piksel] - cdf_min;
            int payda = toplam_piksel - cdf_min;
            int yeni_piksel = (pay * 255) / payda;
            
            yeni_resim.at<uchar>(y, x) = saturate_cast<uchar>(yeni_piksel);
        }
    }

    imwrite("esitlenmis_socrates.jpg", yeni_resim);

    int hist_yeni[256] = {0};
    for (int y = 0; y < resim.rows; y++) {
        for (int x = 0; x < resim.cols; x++) {
            int piksel = yeni_resim.at<uchar>(y, x);
            hist_yeni[piksel]++;
        }
    }

    int max_hist = 0;
    int max_hist_yeni = 0;
    for (int i = 0; i < 256; i++) {
        if (hist[i] > max_hist) max_hist = hist[i];
        if (hist_yeni[i] > max_hist_yeni) max_hist_yeni = hist_yeni[i];
    }

    Mat hist_resmi(resim.rows, 512, CV_8UC1, Scalar(255));
    Mat hist_yeni_resmi(resim.rows, 512, CV_8UC1, Scalar(255));

    for (int i = 0; i < 256; i++) {
        int uzunluk1 = (hist[i] * resim.rows) / max_hist;
        line(hist_resmi, Point(i * 2, resim.rows), Point(i * 2, resim.rows - uzunluk1), Scalar(0), 2);

        int uzunluk2 = (hist_yeni[i] * resim.rows) / max_hist_yeni;
        line(hist_yeni_resmi, Point(i * 2, resim.rows), Point(i * 2, resim.rows - uzunluk2), Scalar(0), 2);
    }

    Mat ust_kisim, alt_kisim, son_gorsel;
    hconcat(resim, hist_resmi, ust_kisim);
    hconcat(yeni_resim, hist_yeni_resmi, alt_kisim);
    vconcat(ust_kisim, alt_kisim, son_gorsel);

    imwrite("tum_sonuclar.jpg", son_gorsel);
    
    cout << "Basarili!" << endl;
    return 0;
}
EOF

g++ -std=c++17 -O3 gorev2.cpp -o gorev2 \
    -I/content/opencv_install/include/opencv4 \
    -L/content/opencv_install/lib \
    -lopencv_core -lopencv_imgcodecs -lopencv_imgproc \
    -Wl,-rpath,/content/opencv_install/lib

./gorev2
