#ifndef ADMINPAGE_H
#define ADMINPAGE_H

#include "Models.h"
#include "BloodManager.h"

void CetakHeaderTabel();
void CetakBarisPendonor(int No, const Pendonor& P);
void TampilSemuaPendonor(NodePendonor* Head);
void TampilSemuaAkun(NodePendonor* Head);
void TampilAkunAdmin(NodePendonor* Head);
void TampilAkunUser(NodePendonor* Head);
void MenuAdmin(const User& UserAktif, NodePendonor*& Head, StokDarah& Stok);
void AdminTambahAkun(NodePendonor*& Head);
void AdminTampilkanAkun(NodePendonor*& Head);
void AdminEditDataDiri(NodePendonor*& Head);
void AdminUpdateRiwayat(NodePendonor*& Head);
void AdminHapusPendonor(NodePendonor*& Head);
void AdminCekStok(const StokDarah& Stok);
void AdminUpdateStok(StokDarah& Stok);
void AdminKurangiStok(StokDarah& Stok);
void AdminVerifikasiDonor(NodePendonor*& Head);

#endif