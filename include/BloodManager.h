#ifndef BLOODMANAGER_H
#define BLOODMANAGER_H

#include "Models.h"
#include <string>

struct NodePendonor {
    Pendonor Data;
    NodePendonor* Next;
};

void InitList(NodePendonor*& Head);
void TambahPendonor(NodePendonor*& Head, const Pendonor& DataBaru);
bool HapusPendonor(NodePendonor*& Head, const std::string& Username);
NodePendonor* CariPendonorByUsername(NodePendonor* Head, const std::string& Username);
int HitungPendonor(NodePendonor* Head);
bool ListKosong(NodePendonor* Head);
void SortByNamaAZ(NodePendonor*& Head);   
void SortByNamaZA(NodePendonor*& Head);   
void SortByRole(NodePendonor*& Head);     
NodePendonor* BinarySearchByNama(NodePendonor* SortedHead, const std::string& Nama, int Size);
void MuatPendonorDariFile(NodePendonor*& Head);
void SimpanPendonorKeFile(NodePendonor* Head);
StokDarah MuatStokDariFile();
void SimpanStokKeFile(const StokDarah& Stok);
bool TambahStok(StokDarah& Stok, const std::string& GolDarah, int Jumlah);
bool KurangiStok(StokDarah& Stok, const std::string& GolDarah, int Jumlah);
int GetStok(const StokDarah& Stok, const std::string& GolDarah);
bool StokKosong(const StokDarah& Stok);
void TambahRiwayat(const RiwayatDonor& Riwayat);
std::string AmbilTglTerakhir(const std::string& Username);
int HitungSelisihHari(const std::string& TglTerakhir);
bool ValidasiTanggal(const std::string& Tanggal);
bool ValidasiGolDarah(const std::string& GolDarah);
std::string NormalisasiGolDarah(const std::string& GolDarah);
bool CekUsernameAdaDiFile(const std::string& Username);

#endif