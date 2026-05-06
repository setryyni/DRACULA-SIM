#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace Utils {
    void bersihkanLayar();
    void tekanEnter();
    bool isAngka(std::string input);
    int hitungSelisihHari(std::string tanggalTerakhir);
    bool cekKelayakanWaktu(std::string tanggalTerakhir);
}

#endif