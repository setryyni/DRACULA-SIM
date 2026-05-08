#ifndef MODELS_H
#define MODELS_H

#include <string>

struct User {
    std::string Username;
    std::string Password;
    std::string Role; 
};

struct Pendonor {
    std::string Username;      
    std::string Nik;           
    std::string Nama;
    std::string GolDarah;      
    std::string Rhesus;        
    std::string Alamat;
    std::string NomorTelepon;  
};

struct RiwayatDonor {
    std::string username;           
    std::string TanggalDonor;  
    std::string Lokasi;        
    int JumlahKantong;         
    std::string Keterangan;    
};

struct StokDarah {
    int StokA;
    int StokB;
    int StokAB;
    int StokO;
};

#endif