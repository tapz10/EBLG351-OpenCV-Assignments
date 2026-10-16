#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>
#include <vector>

using namespace std;
using namespace cv;

void minMaxBul(const Mat& resim, int baslangic_y, int bitis_y, int& yerel_min, int& yerel_max) {
    int min_deger = 255;
    int max_deger = 0;
    for (int y = baslangic_y; y < bitis_y; y++) {
        for (int x = 0; x < resim.cols; x++) {
            int piksel = resim.at<uchar>(y, x);
            if (piksel < min_deger) min_deger = piksel;
            if (piksel > max_deger) max_deger = piksel;
        }
    }
    yerel_min = min_deger;
    yerel_max = max_deger;
}

void olcekle(const Mat& resim, Mat& yeni_resim, int baslangic_y, int bitis_y, int min_val, int max_val) {
    for (int y = baslangic_y; y < bitis_y; y++) {
        for (int x = 0; x < resim.cols; x++) {
            int piksel = resim.at<uchar>(y, x);
            int yeni_piksel = ((piksel - min_val) * 255) / (max_val - min_val);
            yeni_resim.at<uchar>(y, x) = saturate_cast<uchar>(yeni_piksel);
        }
    }
}

int main() {
    Mat resim = imread("socrates.jpg", IMREAD_GRAYSCALE);
    if (resim.empty()) {
        cout << "HATA" << endl;
        return -1;
    }

    int thread_sayisi = 4;
    vector<thread> threadler;
    vector<int> min_degerler(thread_sayisi, 255);
    vector<int> max_degerler(thread_sayisi, 0);

    int satir_payi = resim.rows / thread_sayisi;

    for (int i = 0; i < thread_sayisi; i++) {
        int baslangic_y = i * satir_payi;
        int bitis_y = (i == thread_sayisi - 1) ? resim.rows : (i + 1) * satir_payi;
        threadler.push_back(thread(minMaxBul, cref(resim), baslangic_y, bitis_y, ref(min_degerler[i]), ref(max_degerler[i])));
    }

    for (int i = 0; i < thread_sayisi; i++) {
        threadler[i].join();
    }

    int min_val = 255;
    int max_val = 0;
    for (int i = 0; i < thread_sayisi; i++) {
        if (min_degerler[i] < min_val) min_val = min_degerler[i];
        if (max_degerler[i] > max_val) max_val = max_degerler[i];
    }

    Mat yeni_resim = resim.clone();
    threadler.clear();

    for (int i = 0; i < thread_sayisi; i++) {
        int baslangic_y = i * satir_payi;
        int bitis_y = (i == thread_sayisi - 1) ? resim.rows : (i + 1) * satir_payi;
        threadler.push_back(thread(olcekle, cref(resim), ref(yeni_resim), baslangic_y, bitis_y, min_val, max_val));
    }

    for (int i = 0; i < thread_sayisi; i++) {
        threadler[i].join();
    }

    imwrite("C:\\Users\\Tolga\\Desktop\\kontrast_germe.jpg", yeni_resim);
    cout << "Basarili!" << endl;

    imshow("Orijinal Resim", resim);
    imshow("Kontrast Germe Sonucu", yeni_resim);
    waitKey(0);

    return 0;
}
