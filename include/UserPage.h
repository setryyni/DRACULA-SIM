#ifndef USERPAGE_H
#define USERPAGE_H

#include "Utils.h"
#include "Models.h"
#include "BloodManager.h"

void menuUser(User& UserAktif, NodePendonor*& Head);
void Profil(User& UserAktif, NodePendonor*& Head);
void EditProfil(User& UserAktif, NodePendonor*& Head);
void cekJadwal(const User& UserAktif);
void riwayatDonor(const User& UserAktif);
void edukasi();
void HapusAkun(const User& UserAktif, NodePendonor*& Head);

#endif