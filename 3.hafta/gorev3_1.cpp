%%bash
cat << 'EOF' > gorev3_1.cpp
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

    Ptr<CLAHE> clahe = createCLAHE();
    clahe->setClipLimit(2.0);
    clahe->setTilesGridSize(Size(8, 8));

    Mat clahe_resim;
    clahe->apply(resim, clahe_resim);

    imwrite("clahe_hazir.jpg", clahe_resim);

    cout << "Basarili!" << endl;
    return 0;
}
EOF

g++ -std=c++17 -O3 gorev3_1.cpp -o gorev3_1 \
    -I/content/opencv_install/include/opencv4 \
    -L/content/opencv_install/lib \
    -lopencv_core -lopencv_imgcodecs -lopencv_imgproc \
    -Wl,-rpath,/content/opencv_install/lib

./gorev3_1
