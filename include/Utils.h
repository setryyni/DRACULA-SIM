#ifndef UTILS_H
#define UTILS_H

#include <string>

/**
 * File: Utils.h
 * Deskripsi: Kumpulan fungsi pembantu (utility) untuk merapikan tampilan
 * dan melakukan validasi serta perhitungan matematika/tanggal.
 */

namespace Utils {

    // Membersihkan tampilan teks di terminal (mendukung Windows & Linux/Mac)
    void bersihkanLayar();

    // Menghentikan program sejenak sampai user menekan tombol Enter
    // Berguna untuk memberi waktu user membaca pesan sebelum layar dibersihkan
    void tekanEnter();

    // Memeriksa apakah sebuah string hanya berisi karakter angka (0-9)
    // Cocok digunakan untuk validasi input NIK atau Nomor Telepon
    bool isAngka(std::string input);

    // Menghitung selisih hari antara tanggal terakhir donor dengan hari ini
    // Format input tanggal yang diterima: YYYY-MM-DD
    // Mengembalikan -1 jika format salah, atau 999 jika string kosong/"-"
    int hitungSelisihHari(std::string tanggalTerakhir);

    // Mengecek kelayakan waktu donor berdasarkan aturan PMI (minimal 60 hari)
    // Mengembalikan true jika selisih hari >= 60, false jika belum
    bool cekKelayakanWaktu(std::string tanggalTerakhir);

}

#endif