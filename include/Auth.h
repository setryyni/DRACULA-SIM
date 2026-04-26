#ifndef AUTH_H
#define AUTH_H

#include "Models.h"

/**
 * File: Auth.h
 * Deskripsi: Deklarasi fungsi-fungsi utama untuk sistem autentikasi
 * (Registrasi, Login, dan Navigasi Menu Awal).
 */

// Menampilkan form pendaftaran untuk akun pendonor baru ke dalam database.
// Mengembalikan true jika pendaftaran berhasil, false jika gagal atau username sudah ada.
bool RegisterAkun();

// Menangani proses verifikasi login dengan batasan 3x percobaan gagal.
// Jika gagal 3x, akan memberlakukan waktu tunggu (cooldown) selama 2 menit.
// Mengembalikan struct User yang berisi data pengguna yang berhasil login.
User LoginAkun();

// Menampilkan menu utama aplikasi untuk memilih Login, Register, atau Keluar.
// Mengatur alur (flow) program hingga pengguna berhasil mendapatkan sesi login.
// Mengembalikan data User aktif untuk digunakan di menu utama aplikasi.
User MulaiAuth();

#endif