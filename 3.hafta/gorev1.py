%%bash
cat << 'EOF' > kontrast.cpp
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
    Mat resim = imread("socrates.jpg", IMREAD_GRAYSCALE);
    if (resim.empty()) return -1;

    int min_val = 255;
    int max_val = 0;

    for (int y = 0; y < resim.rows; y++) {
        for (int x = 0; x < resim.cols; x++) {
            int piksel = resim.at<uchar>(y, x);
            if (piksel < min_val) min_val = piksel;
            if (piksel > max_val) max_val = piksel;
        }
    }

    Mat yeni_resim = resim.clone();

    for (int y = 0; y < resim.rows; y++) {
        for (int x = 0; x < resim.cols; x++) {
            int piksel = resim.at<uchar>(y, x);
            int yeni_piksel = ((piksel - min_val) * 255) / (max_val - min_val);
            yeni_resim.at<uchar>(y, x) = saturate_cast<uchar>(yeni_piksel);
        }
    }

    imwrite("kontrast_germe.jpg", yeni_resim);
    return 0;
}
EOF

g++ -std=c++17 -O3 kontrast.cpp -o kontrast \
    -I/content/opencv_install/include/opencv4 \
    -L/content/opencv_install/lib \
    -lopencv_core -lopencv_imgcodecs -lopencv_imgproc \
    -Wl,-rpath,/content/opencv_install/lib

./kontrast
