%%bash
cat << 'EOF' > gorev4.cpp
#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace cv;

int main() {
    Mat resim = imread("socrates.jpg", IMREAD_GRAYSCALE);
    if (resim.empty()) {
        cout << "HATA" << endl;
        return -1;
    }

    Mat gurultulu = resim.clone();
    srand(time(0));
    int gurultu_miktari = (resim.rows * resim.cols) * 0.05; 
    
    for (int i = 0; i < gurultu_miktari; i++) {
        int y = rand() % resim.rows;
        int x = rand() % resim.cols;
        int tip = rand() % 2;
        if (tip == 0) {
            gurultulu.at<uchar>(y, x) = 0;
        } else {
            gurultulu.at<uchar>(y, x) = 255;
        }
    }
    imwrite("gurultulu_socrates.jpg", gurultulu);

    Mat filtrelenmis = gurultulu.clone();

    for (int y = 1; y < gurultulu.rows - 1; y++) {
        for (int x = 1; x < gurultulu.cols - 1; x++) {
            int toplam = 0;
            
            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    toplam += gurultulu.at<uchar>(y + ky, x + kx);
                }
            }
            
            filtrelenmis.at<uchar>(y, x) = toplam / 9;
        }
    }

    imwrite("filtrelenmis_socrates.jpg", filtrelenmis);
    cout << "Basarili! Gorseller uretildi." << endl;

    return 0;
}
EOF

g++ -std=c++17 -O3 gorev4.cpp -o gorev4 \
    -I/content/opencv_install/include/opencv4 \
    -L/content/opencv_install/lib \
    -lopencv_core -lopencv_imgcodecs -lopencv_imgproc \
    -Wl,-rpath,/content/opencv_install/lib

./gorev4
