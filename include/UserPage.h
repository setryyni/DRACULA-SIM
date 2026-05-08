#ifndef USERPAGE_H
#define USERPAGE_H

#include "Utils.h"
#include "Models.h"
#include "BloodManager.h"

void menuUser(const User& UserAktif, NodePendonor*& Head);
void Profil(const User& UserAktif, NodePendonor*& Head);
void cekJadwal(const User& UserAktif);
void riwayatDonor(const User& UserAktif);
void edukasi();

#endif