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
    std::string TglTerakhir;
    int TotalDonor;
};

// 6. Struktur untuk mencatat riwayat donor (1 baris = 1 kali kegiatan donor)
struct RiwayatDonor {
    std::string username;           // Kunci penghubung ke siapa yang donor
    std::string TanggalDonor;  // Format: YYYY-MM-DD
    std::string Lokasi;        // Misal: "PMI_Pusat" atau "Mobile_Unit_Unlam"
    int JumlahKantong;         // Biasanya 1
    std::string Keterangan;    // Misal: "Sukses", "Ditolak_Tensi_Tinggi"
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

#endif