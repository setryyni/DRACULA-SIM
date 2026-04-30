#ifndef ADMINPAGE_H
#define ADMINPAGE_H

#include "Models.h"
#include "BloodManager.h"

/**
 * File: AdminPage.h
 * Deskripsi: Deklarasi fungsi menu admin PMI.
 */

// Menu utama admin (loop sampai logout)
void MenuAdmin(const User& UserAktif, NodePendonor*& Head, StokDarah& Stok);

// 1. Tambah akun (input: Username, Password, Role saja)
void AdminTambahAkun(NodePendonor*& Head);

// 2. Tampilkan semua akun + sorting + searching
void AdminTampilkanAkun(NodePendonor*& Head);

// 3. Update riwayat donor terakhir
void AdminUpdateRiwayat(NodePendonor*& Head);

// 4. Hapus pendonor
void AdminHapusPendonor(NodePendonor*& Head);

// 5. Cek stok darah
void AdminCekStok(const StokDarah& Stok);

// 6. Update (tambah) stok darah
void AdminUpdateStok(StokDarah& Stok);

// 7. Kurangi stok darah
void AdminKurangiStok(StokDarah& Stok);

// 8. Verifikasi kelayakan donor
void AdminVerifikasiDonor(NodePendonor*& Head);

#endif