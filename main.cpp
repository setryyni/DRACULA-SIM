#include "include/Auth.h"
#include "include/AdminPage.h"
#include "include/UserPage.h"
#include "include/BloodManager.h"
#include "include/Utils.h"
#include "include/Models.h"
#include <iostream>

using namespace std;

int main() {
    NodePendonor* headPendonor = nullptr;
    InitList(headPendonor);

    MuatPendonorDariFile(headPendonor);
    StokDarah stokDarah = MuatStokDariFile();

    while (true) {
        User userAktif = MulaiAuth();

        if (userAktif.Role == "admin") {
            MenuAdmin(userAktif, headPendonor, stokDarah);
        } 
        else if (userAktif.Role == "user") {
            menuUser(userAktif);
        }

    }
    
    return 0;
}