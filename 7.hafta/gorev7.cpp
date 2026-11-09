#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>

using namespace cv;
using namespace std;

Mat dft2D(const Mat& giris) {
    Mat gri;
    if (giris.channels() == 3) cvtColor(giris, gri, COLOR_BGR2GRAY);
    else gri = giris.clone();
    
    gri.convertTo(gri, CV_64F);
    int M = gri.rows;
    int N = gri.cols;
    Mat harmonik = Mat::zeros(M, N, CV_64F);

    for (int u = 0; u < M; u++) {
        for (int v = 0; v < N; v++) {
            double realSum = 0.0;
            double imagSum = 0.0;
            for (int x = 0; x < M; x++) {
                for (int y = 0; y < N; y++) {
                    double aci = 2.0 * CV_PI * ((u * x) / (double)M + (v * y) / (double)N);
                    double pixel = gri.at<double>(x, y);
                    realSum += pixel * cos(aci);
                    imagSum -= pixel * sin(aci);
                }
            }
            harmonik.at<double>(u, v) = sqrt(realSum * realSum + imagSum * imagSum) / (M * N);
        }
    }
    return harmonik;
}

int main() {
    Mat resim = imread("pisa.jpg", IMREAD_GRAYSCALE);
    if (resim.empty()) {
        resim = Mat(64, 64, CV_8UC1, Scalar(128));
    }

    Mat kucukResim;
    resize(resim, kucukResim, Size(64, 64));

    Mat harmonikMatris = dft2D(kucukResim);

    Mat logHarmonik;
    log(harmonikMatris + 1.0, logHarmonik);
    normalize(logHarmonik, logHarmonik, 0, 255, NORM_MINMAX, CV_8UC1);

    imwrite("dft2d_harmonik_sonuc.jpg", logHarmonik);
    cout << "2D DFT harmonik matrisi basariyla hesaplandi ve kaydedildi." << endl;

    return 0;
}
