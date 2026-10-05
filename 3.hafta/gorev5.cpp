%%bash
cat << 'EOF' > gorev5.cpp
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
    int gurultu_miktari = (resim.rows * resim.cols) * 0.10; 
    
    for (int i = 0; i < gurultu_miktari; i++) {
        int y = rand() % resim.rows;
        int x = rand() % resim.cols;
        int tip = rand() % 2;
        if (tip == 0) gurultulu.at<uchar>(y, x) = 0;
        else gurultulu.at<uchar>(y, x) = 255;
    }
    imwrite("sp_gurultulu.jpg", gurultulu);

    Mat medyan_hazir;
    medianBlur(gurultulu, medyan_hazir, 5);
    imwrite("medyan_hazir.jpg", medyan_hazir);

    Mat medyan_manuel = gurultulu.clone();
    
    for (int y = 2; y < gurultulu.rows - 2; y++) {
        for (int x = 2; x < gurultulu.cols - 2; x++) {
            int pencere[25];
            int sayac = 0;
            
            for (int ky = -2; ky <= 2; ky++) {
                for (int kx = -2; kx <= 2; kx++) {
                    pencere[sayac] = gurultulu.at<uchar>(y + ky, x + kx);
                    sayac++;
                }
            }
            
            for(int i = 0; i < 24; i++) {
                for(int j = 0; j < 24 - i; j++) {
                    if(pencere[j] > pencere[j+1]) {
                        int temp = pencere[j];
                        pencere[j] = pencere[j+1];
                        pencere[j+1] = temp;
                    }
                }
            }
            
            medyan_manuel.at<uchar>(y, x) = pencere[12];
        }
    }
    imwrite("medyan_manuel.jpg", medyan_manuel);

    cout << "Basarili!" << endl;
    return 0;
}
EOF

g++ -std=c++17 -O3 gorev5.cpp -o gorev5 \
    -I/content/opencv_install/include/opencv4 \
    -L/content/opencv_install/lib \
    -lopencv_core -lopencv_imgcodecs -lopencv_imgproc \
    -Wl,-rpath,/content/opencv_install/lib

./gorev5
