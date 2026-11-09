#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

using namespace cv;
using namespace std;

int main() {
    string dosyaAdi = "pisa.jpg";
    Mat resim = imread(dosyaAdi, IMREAD_COLOR);

    if (resim.empty()) {
        resim = Mat(500, 500, CV_8UC3, Scalar(255, 255, 255));
        circle(resim, Point(250, 250), 100, Scalar(0, 0, 0), 3);
        circle(resim, Point(150, 150), 50, Scalar(0, 0, 0), 3);
    }

    Mat gri, cannyCiktisi, medianCiktisi;
    cvtColor(resim, gri, COLOR_BGR2GRAY);
    medianBlur(gri, gri, 3);

    Canny(gri, cannyCiktisi, 50, 150, 3);
    medianBlur(cannyCiktisi, medianCiktisi, 3);

    vector<Vec3f> cemberler;
    HoughCircles(medianCiktisi, cemberler, HOUGH_GRADIENT, 1, 
                 medianCiktisi.rows / 8, 100, 30, 10, 200);

    Mat sonucResmi = resim.clone();

    for (size_t i = 0; i < cemberler.size(); i++) {
        Point merkez(cvRound(cemberler[i][0]), cvRound(cemberler[i][1]));
        int yaricap = cvRound(cemberler[i][2]);
        
        circle(sonucResmi, merkez, 3, Scalar(255, 0, 0), -1, LINE_AA);
        circle(sonucResmi, merkez, yaricap, Scalar(0, 255, 0), 2, LINE_AA);
    }

    imwrite("pisa_canny_circles.jpg", cannyCiktisi);
    imwrite("pisa_median_circles.jpg", medianCiktisi);
    imwrite("pisa_hough_circles_sonuc.jpg", sonucResmi);

    cout << "Hough Circles islemi basariyla tamamlandi ve sonuclar kaydedildi." << endl;

    return 0;
}
