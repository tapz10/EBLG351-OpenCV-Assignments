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
        line(resim, Point(100, 50), Point(100, 450), Scalar(0, 0, 0), 4);
        line(resim, Point(200, 50), Point(250, 450), Scalar(0, 0, 0), 4);
        line(resim, Point(50, 300), Point(450, 300), Scalar(0, 0, 0), 4);
    }

    Mat gri, cannyCiktisi, medianCiktisi;
    cvtColor(resim, gri, COLOR_BGR2GRAY);

    Canny(gri, cannyCiktisi, 50, 150, 3);
    medianBlur(cannyCiktisi, medianCiktisi, 3);

    vector<Vec2f> linesStandard;
    HoughLines(medianCiktisi, linesStandard, 1, CV_PI / 180, 150);

    Mat cizimHough1 = resim.clone();
    for (size_t i = 0; i < linesStandard.size(); i++) {
        float rho = linesStandard[i][0];
        float theta = linesStandard[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));
        line(cizimHough1, pt1, pt2, Scalar(0, 0, 255), 2, LINE_AA);
    }

    vector<Vec4i> linesProbabilistic;
    HoughLinesP(medianCiktisi, linesProbabilistic, 1, CV_PI / 180, 50, 50, 10);

    Mat cizimHough2 = resim.clone();
    for (size_t i = 0; i < linesProbabilistic.size(); i++) {
        Vec4i l = linesProbabilistic[i];
        line(cizimHough2, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 255, 0), 2, LINE_AA);
    }

    imwrite("pisa_canny.jpg", cannyCiktisi);
    imwrite("pisa_median.jpg", medianCiktisi);
    imwrite("pisa_hough_standart.jpg", cizimHough1);
    imwrite("pisa_hough_probabilistic.jpg", cizimHough2);

    cout << "Islem basariyla tamamlandi. Sonuclar kaydedildi." << endl;

    return 0;
}
