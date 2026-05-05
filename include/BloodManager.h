#ifndef BLOODMANAGER_H
#define BLOODMANAGER_H

#include "Models.h"
#include <string>

/**
 * File: BloodManager.h
 * Deskripsi: Mengelola data pendonor (linked list), stok darah, dan riwayat donor.
 */

// =============================================
// NODE untuk Linked List Pendonor
// =============================================
struct NodePendonor {
    Pendonor Data;
    NodePendonor* Next;
};

// =============================================
// LINKED LIST PENDONOR
// =============================================
void InitList(NodePendonor*& Head);
void TambahPendonor(NodePendonor*& Head, const Pendonor& DataBaru);
bool HapusPendonor(NodePendonor*& Head, const std::string& Username);
NodePendonor* CariPendonorByUsername(NodePendonor* Head, const std::string& Username);
int HitungPendonor(NodePendonor* Head);
bool ListKosong(NodePendonor* Head);

// =============================================
// SORTING
// =============================================
void SortByNamaAZ(NodePendonor*& Head);   // Selection Sort
void SortByNamaZA(NodePendonor*& Head);   // Insertion Sort
void SortByRole(NodePendonor*& Head);     // Bubble Sort by Gol. Darah

// =============================================
// SEARCHING
// =============================================
NodePendonor* BinarySearchByNama(NodePendonor* SortedHead, const std::string& Nama, int Size);

// =============================================
// FILE I/O PENDONOR
// =============================================
void MuatPendonorDariFile(NodePendonor*& Head);
void SimpanPendonorKeFile(NodePendonor* Head);

// =============================================
// STOK DARAH
// =============================================
StokDarah MuatStokDariFile();
void SimpanStokKeFile(const StokDarah& Stok);
bool TambahStok(StokDarah& Stok, const std::string& GolDarah, int Jumlah);
bool KurangiStok(StokDarah& Stok, const std::string& GolDarah, int Jumlah);
int GetStok(const StokDarah& Stok, const std::string& GolDarah);
bool StokKosong(const StokDarah& Stok);

// =============================================
// RIWAYAT DONOR
// =============================================

// Tambah 1 baris riwayat baru ke Riwayat.txt
void TambahRiwayat(const RiwayatDonor& Riwayat);

// Ambil tanggal donor terakhir pendonor dari Riwayat.txt (buat verifikasi)
std::string AmbilTglTerakhir(const std::string& Username);

// Hitung total donor pendonor dari Riwayat.txt
int HitungTotalDonor(const std::string& Username);

// Hitung selisih hari dari tanggal donor terakhir sampai hari ini
int HitungSelisihHari(const std::string& TglTerakhir);

// =============================================
// VALIDASI
// =============================================
bool ValidasiTanggal(const std::string& Tanggal);
bool ValidasiGolDarah(const std::string& GolDarah);
std::string NormalisasiGolDarah(const std::string& GolDarah);
bool CekUsernameAdaDiFile(const std::string& Username);

#endif