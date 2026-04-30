#ifndef BLOODMANAGER_H
#define BLOODMANAGER_H

#include "Models.h"
#include <string>

/**
 * File: BloodManager.h
 * Deskripsi: Mengelola data pendonor (linked list) dan stok darah.
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
NodePendonor* CariPendonorByNama(NodePendonor* Head, const std::string& Nama);
int HitungPendonor(NodePendonor* Head);
bool ListKosong(NodePendonor* Head);

// =============================================
// SORTING
// =============================================
void SortByNamaAZ(NodePendonor*& Head);   // Selection Sort
void SortByNamaZA(NodePendonor*& Head);   // Insertion Sort
void SortByRole(NodePendonor*& Head);     // Bubble Sort

// =============================================
// SEARCHING
// =============================================
NodePendonor* LinearSearchByUsername(NodePendonor* Head, const std::string& Username);
NodePendonor* BinarySearchByNama(NodePendonor* SortedHead, const std::string& Nama, int Size);

// =============================================
// FILE I/O
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
// RIWAYAT & VALIDASI
// =============================================
bool UpdateRiwayatDonor(NodePendonor*& Head, const std::string& Username, const std::string& TglBaru);
int HitungSelisihHari(const std::string& TglTerakhir);
bool ValidasiTanggal(const std::string& Tanggal);
bool ValidasiGolDarah(const std::string& GolDarah);
bool CekUsernameAdaDiFile(const std::string& Username);

#endif