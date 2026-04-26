#ifndef MODELS_H
#define MODELS_H

#include <string>

/**
 * File: Models.h
 * Deskripsi: Berisi semua struktur data (struct) untuk aplikasi PMI.
 * Format penamaan variabel menggunakan PascalCase (NamaVariabel).
 */

// 1. Struktur untuk akun sistem (Login & Register)
struct User {
    std::string Username;
    std::string Password;
    std::string Role; // "admin" atau "pendonor"
};

// 2. Struktur untuk data lengkap profil Pendonor
struct Pendonor {
    std::string Username;      // Kunci penghubung ke struct User
    std::string Nik;           // Disimpan sebagai string (16 digit)
    std::string Nama;
    std::string GolDarah;      // A, B, AB, O
    std::string Rhesus;        // Positif (+) atau Negatif (-)
    std::string Alamat;
    std::string NomorTelepon;
    std::string TglTerakhir;   // Format: YYYY-MM-DD
    int TotalDonor;            // Jumlah total kantong yang disumbangkan
};

// 3. Struktur untuk pengelolaan stok darah di markas PMI
struct StokDarah {
    int StokA;
    int StokB;
    int StokAB;
    int StokO;
};

// 4. Struktur untuk input pemeriksaan medis singkat (Verifikasi Kelayakan)
struct VerifikasiMedis {
    std::string Nik;
    double TekananDarah;
    double KadarHemoglobin;
    bool IsLayak;              // Hasil keputusan petugas PMI
};

// 5. Struktur untuk pendaftaran mandiri (Pre-Registration)
struct PreRegistration {
    std::string Nama;
    std::string Nik;
    std::string GolDarah;
    std::string TglRencana;    // Tanggal rencana datang ke lokasi PMI
};

#endif