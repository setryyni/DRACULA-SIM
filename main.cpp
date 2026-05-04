#include "../include/Auth.h"
#include "../include/AdminPage.h"
#include "../include/UserPage.h"
#include "../include/BloodManager.h"
#include "../include/Utils.h"
#include "../include/Models.h"
#include <iostream>

using namespace std;

/**
 * File: main.cpp
 * Deskripsi: Titik masuk utama aplikasi Sistem Donor Darah PMI.
 * Mengatur alur pemindahan antara login dan menu spesifik role.
 */

int main() {
    // 1. Inisialisasi Data Utama
    NodePendonor* headPendonor = nullptr;
    InitList(headPendonor);

    // 2. Muat Data dari File Txt ke dalam Program
    MuatPendonorDariFile(headPendonor);
    StokDarah stokDarah = MuatStokDariFile();

    // 3. Loop Utama Program
    while (true) {
        // Jalankan sistem Auth (Login/Register)
        // MulaiAuth akan loop sampai user berhasil login atau memilih keluar (exit)
        User userAktif = MulaiAuth();

        // 4. Pengalihan Menu Berdasarkan Role
        if (userAktif.Role == "admin") {
            // Jika role adalah admin, masuk ke menu admin
            MenuAdmin(userAktif, headPendonor, stokDarah);
        } 
        else if (userAktif.Role == "pendonor") {
            // Jika role adalah pendonor, masuk ke menu user
            menuUser(userAktif);
        }

        // Setelah logout dari MenuAdmin atau menuUser, 
        // program akan kembali ke MulaiAuth() di atas.
    }

    // Membersihkan memory linked list sebelum keluar (opsional karena loop di atas infinite)
    // SimpanPendonorKeFile(headPendonor); 
    
    return 0;
}