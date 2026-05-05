#ifndef USERPAGE_H
#define USERPAGE_H

#include "Utils.h"
#include "Models.h"
#include "BloodManager.h"

void menuUser(const User& UserAktif);
void Profil(const User& UserAktif);
void cekJadwal(const User& UserAktif);
void riwayatDonor(const User& UserAktif);
void edukasi();

#endif // USERPAGE_H