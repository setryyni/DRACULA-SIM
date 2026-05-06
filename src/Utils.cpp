#include "../include/Utils.h" 
#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <cctype>

using namespace std;

namespace Utils {

    void bersihkanLayar() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    void tekanEnter() {
        cout << "\nTekan Enter untuk melanjutkan...";
        cin.ignore(10000, '\n');
        cin.get();
    }

    bool isAngka(string input) {
        if (input.empty()) return false;
        for (char c : input) {
            if (!isdigit(c)) return false;
        }
        return true;
    }

    int hitungSelisihHari(string tanggalTerakhir) {
        if (tanggalTerakhir == "" || tanggalTerakhir == "-") {
            return 999; 
        }

        struct tm t_input = {0};
        stringstream ss(tanggalTerakhir);
        char dash1, dash2;
        int year, month, day;
        
        // Memisahkan string berdasarkan tanda '-'
        if (!(ss >> year >> dash1 >> month >> dash2 >> day)) return -1;

        t_input.tm_year = year - 1900;
        t_input.tm_mon = month - 1;
        t_input.tm_mday = day;

        time_t time_input = mktime(&t_input);
        time_t time_now = time(0); 

        if (time_input == -1) return -1;

        double detik = difftime(time_now, time_input);
        
        return detik / (60 * 60 * 24);
    }

    bool cekKelayakanWaktu(string tanggalTerakhir) {
        int selisih = hitungSelisihHari(tanggalTerakhir);
        const int MIN_HARI = 90; 

        if (selisih >= MIN_HARI) {
            return true;
        } else {
            return false;
        }
    }
}