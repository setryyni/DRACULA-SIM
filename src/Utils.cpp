#include "../include/Utils.h" // Menggunakan path relatif naik satu folder ke include
#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <cctype>

using namespace std;

namespace Utils {

    void bersihkanLayar() {
        // Otomatis mendeteksi sistem operasi (Windows vs Linux/Mac)
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    void tekanEnter() {
        cout << "\nTekan Enter untuk melanjutkan...";
        // Membersihkan sisa buffer input agar tidak terlewat saat pakai cin/getline
        cin.ignore(10000, '\n');
        cin.get();
    }

    bool isAngka(string input) {
        // Cek apakah string kosong
        if (input.empty()) return false;
        
        // Cek setiap karakter, pastikan semuanya adalah digit (0-9)
        for (char c : input) {
            if (!isdigit(c)) return false;
        }
        return true;
    }

    int hitungSelisihHari(string tanggalTerakhir) {
        // Jika user belum pernah donor atau datanya kosong/strip
        if (tanggalTerakhir == "" || tanggalTerakhir == "-") {
            return 999; // Mengembalikan angka besar agar dianggap pasti boleh donor
        }

        // Format input: "YYYY-MM-DD"
        struct tm t_input = {0};
        stringstream ss(tanggalTerakhir);
        char dash1, dash2;
        int year, month, day;
        
        // Memisahkan string berdasarkan tanda '-'
        if (!(ss >> year >> dash1 >> month >> dash2 >> day)) return -1;

        // tm_year dihitung sejak 1900, dan tm_mon dari 0 (Januari)
        t_input.tm_year = year - 1900;
        t_input.tm_mon = month - 1;
        t_input.tm_mday = day;

        // Konversi struct tm ke format time_t (detik sejak epoch)
        time_t time_input = mktime(&t_input);
        time_t time_now = time(0); // Mengambil waktu lokal komputer saat ini

        if (time_input == -1) return -1;

        // Menghitung selisih waktu dalam hitungan detik
        double detik = difftime(time_now, time_input);
        
        // Konversi detik menjadi hitungan hari (60 detik * 60 menit * 24 jam)
        return detik / (60 * 60 * 24);
    }

    bool cekKelayakanWaktu(string tanggalTerakhir) {
        int selisih = hitungSelisihHari(tanggalTerakhir);
        const int MIN_HARI = 90; // Aturan PMI: jarak minimal donor adalah 90 hari

        if (selisih >= MIN_HARI) {
            return true;
        } else {
            return false;
        }
    }
}