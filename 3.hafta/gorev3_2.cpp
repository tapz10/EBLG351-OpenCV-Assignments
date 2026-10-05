%%bash
cat << 'EOF' > gorev3_2.cpp
#include <iostream>
#include <opencv2/opencv.hpp>
#include <algorithm>

using namespace std;
using namespace cv;

int main() {
    Mat resim = imread("socrates.jpg", IMREAD_GRAYSCALE);
    if (resim.empty()) {
        cout << "HATA" << endl;
        return -1;
    }

    int grid_x = 8;
    int grid_y = 8;
    int tile_w = resim.cols / grid_x;
    int tile_h = resim.rows / grid_y;
    int toplam_piksel = tile_w * tile_h;
    
    int clip_limit = max(1, (toplam_piksel * 2) / 256); 

    int mappings[8][8][256] = {0};

    for (int i = 0; i < grid_y; i++) {
        for (int j = 0; j < grid_x; j++) {
            int hist[256] = {0};

            for (int y = i * tile_h; y < (i + 1) * tile_h; y++) {
                for (int x = j * tile_w; x < (j + 1) * tile_w; x++) {
                    int piksel = resim.at<uchar>(y, x);
                    hist[piksel]++;
                }
            }

            int fazlalik = 0;
            for (int k = 0; k < 256; k++) {
                if (hist[k] > clip_limit) {
                    fazlalik += hist[k] - clip_limit;
                    hist[k] = clip_limit;
                }
            }

            int dagitim = fazlalik / 256;
            int kalan = fazlalik % 256;
            for (int k = 0; k < 256; k++) {
                hist[k] += dagitim;
                if (k < kalan) hist[k]++;
            }

            int cdf = 0;
            for (int k = 0; k < 256; k++) {
                cdf += hist[k];
                mappings[i][j][k] = (cdf * 255) / toplam_piksel;
            }
        }
    }

    Mat yeni_resim = resim.clone();
    for (int y = 0; y < resim.rows; y++) {
        for (int x = 0; x < resim.cols; x++) {
            int piksel = resim.at<uchar>(y, x);

            float ty = (float)y / tile_h - 0.5f;
            float tx = (float)x / tile_w - 0.5f;

            int y1 = max(0, (int)ty);
            int x1 = max(0, (int)tx);
            int y2 = min(grid_y - 1, y1 + 1);
            int x2 = min(grid_x - 1, x1 + 1);

            float dy = ty - y1;
            float dx = tx - x1;

            if (ty < 0) { y1 = 0; y2 = 0; dy = 0; }
            if (tx < 0) { x1 = 0; x2 = 0; dx = 0; }

            int p1 = mappings[y1][x1][piksel];
            int p2 = mappings[y1][x2][piksel];
            int p3 = mappings[y2][x1][piksel];
            int p4 = mappings[y2][x2][piksel];

            float ust = p1 * (1.0f - dx) + p2 * dx;
            float alt = p3 * (1.0f - dx) + p4 * dx;
            int yeni_piksel = ust * (1.0f - dy) + alt * dy;

            yeni_resim.at<uchar>(y, x) = saturate_cast<uchar>(yeni_piksel);
        }
    }

    imwrite("clahe_manuel.jpg", yeni_resim);
    cout << "Basarili! 'clahe_manuel.jpg' uretildi." << endl;

    return 0;
}
EOF

g++ -std=c++17 -O3 gorev3_2.cpp -o gorev3_2 \
    -I/content/opencv_install/include/opencv4 \
    -L/content/opencv_install/lib \
    -lopencv_core -lopencv_imgcodecs -lopencv_imgproc \
    -Wl,-rpath,/content/opencv_install/lib

./gorev3_2
