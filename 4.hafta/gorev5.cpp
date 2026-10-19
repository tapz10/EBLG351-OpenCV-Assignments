#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace std;
using namespace cv;

int main() {
    Mat resim = imread("/Users/tapz/Desktop/jairzinho.jpg", IMREAD_GRAYSCALE);
    if (resim.empty()) {
        cout << "HATA" << endl;
        return -1;
    }

    Mat log_resim = Mat::zeros(resim.size(), CV_8UC1);
    float log_kernel[7][7] = {
        {  0,   0,   1,   2,   1,   0,   0 },
        {  0,   3,  13,  22,  13,   3,   0 },
        {  1,  13,  59,  97,  59,  13,   1 },
        {  2,  22,  97, -152,  97,  22,   2 },
        {  1,  13,  59,  97,  59,  13,   1 },
        {  0,   3,  13,  22,  13,   3,   0 },
        {  0,   0,   1,   2,   1,   0,   0 }
    };

    for (int y = 3; y < resim.rows - 3; y++) {
        for (int x = 3; x < resim.cols - 3; x++) {
            float toplam = 0;
            for (int ky = -3; ky <= 3; ky++) {
                for (int kx = -3; kx <= 3; kx++) {
                    toplam += resim.at<uchar>(y + ky, x + kx) * log_kernel[ky + 3][kx + 3];
                }
            }
            log_resim.at<uchar>(y, x) = saturate_cast<uchar>(abs(toplam) / 16.0f);
        }
    }

    imwrite("log_sonuc.jpg", log_resim);
    cout << "Basarili!" << endl;

    imshow("Orijinal", resim);
    imshow("LoG Sonuc", log_resim);
    waitKey(0);

    return 0;
}
