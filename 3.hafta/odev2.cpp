#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>
#include <vector>
#include <cmath>

using namespace std;
using namespace cv;

void histogramHesapla(const Mat& resim, int baslangic_y, int bitis_y, vector<int>& yerel_hist) {
    for (int y = baslangic_y; y < bitis_y; y++) {
        for (int x = 0; x < resim.cols; x++) {
            int piksel = resim.at<uchar>(y, x);
            yerel_hist[piksel]++;
        }
    }
}

void esitle(const Mat& resim, Mat& yeni_resim, int baslangic_y, int bitis_y, const vector<uchar>& esitleme_tablosu) {
    for (int y = baslangic_y; y < bitis_y; y++) {
        for (int x = 0; x < resim.cols; x++) {
            int piksel = resim.at<uchar>(y, x);
            yeni_resim.at<uchar>(y, x) = esitleme_tablosu[piksel];
        }
    }
}

int main() {
    Mat resim = imread("C:\\Users\\Tolga\\Desktop\\gurultulusocratessonucu.jpg", IMREAD_GRAYSCALE);
    if (resim.empty()) {
        cout << "HATA" << endl;
        return -1;
    }

    int thread_sayisi = 4;
    vector<thread> threadler;
    vector<vector<int>> yerel_histogramlar(thread_sayisi, vector<int>(256, 0));

    int satir_payi = resim.rows / thread_sayisi;

    for (int i = 0; i < thread_sayisi; i++) {
        int baslangic_y = i * satir_payi;
        int bitis_y = (i == thread_sayisi - 1) ? resim.rows : (i + 1) * satir_payi;
        threadler.push_back(thread(histogramHesapla, cref(resim), baslangic_y, bitis_y, ref(yerel_histogramlar[i])));
    }

    for (int i = 0; i < thread_sayisi; i++) {
        threadler[i].join();
    }

    vector<int> hist(256, 0);
    for (int i = 0; i < thread_sayisi; i++) {
        for (int j = 0; j < 256; j++) {
            hist[j] += yerel_histogramlar[i][j];
        }
    }

    vector<int> cdf(256, 0);
    cdf[0] = hist[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + hist[i];
    }

    int cdf_min = 0;
    for (int i = 0; i < 256; i++) {
        if (cdf[i] > 0) {
            cdf_min = cdf[i];
            break;
        }
    }

    int toplam_piksel = resim.rows * resim.cols;
    vector<uchar> esitleme_tablosu(256, 0);
    for (int i = 0; i < 256; i++) {
        if (cdf[i] > 0) {
            int yeni_deger = round(((double)(cdf[i] - cdf_min) / (toplam_piksel - cdf_min)) * 255.0);
            esitleme_tablosu[i] = saturate_cast<uchar>(yeni_deger);
        }
    }

    Mat yeni_resim = resim.clone();
    threadler.clear();

    for (int i = 0; i < thread_sayisi; i++) {
        int baslangic_y = i * satir_payi;
        int bitis_y = (i == thread_sayisi - 1) ? resim.rows : (i + 1) * satir_payi;
        threadler.push_back(thread(esitle, cref(resim), ref(yeni_resim), baslangic_y, bitis_y, cref(esitleme_tablosu)));
    }

    for (int i = 0; i < thread_sayisi; i++) {
        threadler[i].join();
    }

    imwrite("C:\\Users\\Tolga\\Desktop\\histogram_esitleme.jpg", yeni_resim);
    cout << "Basarili!" << endl;

    imshow("Orijinal Resim", resim);
    imshow("Histogram Esitleme Sonucu", yeni_resim);
    waitKey(0);

    return 0;
}
