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

    imwrite("C:\\Users\\Tolga\\Desktop\\clahe_hazir.jpg", clahe_resim);
    cout << "Basarili!" << endl;

    imshow("Orijinal Resim", resim);
    imshow("CLAHE Hazir Sonuc", clahe_resim);
    waitKey(0);

    return 0;
}
