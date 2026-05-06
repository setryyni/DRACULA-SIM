#include "../include/Auth.h"
#include "../include/Models.h" 
#include "../include/Utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream> 
#include <cstdlib>
#include <windows.h> 

using namespace std;

void Tunggu(int Detik) {
    Sleep(Detik * 1000); 
}

bool RegisterAkun() {
    string UsernameBaru, PasswordBaru;
    
    Utils::bersihkanLayar();
    cout << "=== PENDAFTARAN PENDONOR ===\n";
    
    while (true) {
        cout << "Masukkan Username (Ketik '0' untuk batal): ";
        getline(cin, UsernameBaru); 

        if (UsernameBaru.empty()) {
            cout << "[!] Username dan password tidak boleh kosong!\n";
            continue;
        }
        if (UsernameBaru.find(' ') != string::npos) {
            cout << "[!] Username tidak boleh menggunakan spasi!\n";
            continue;
        }
        break; 
    }

    if (UsernameBaru == "0") return false; 

    ifstream FileIn("data/users.csv");
    if (FileIn.is_open()) {
        string barisData;
        User U;
        // Membaca per baris dari file CSV
        while (getline(FileIn, barisData)) {
            stringstream ss(barisData);
            // Memotong data berdasarkan tanda koma (,)
            getline(ss, U.Username, ',');
            getline(ss, U.Password, ',');
            getline(ss, U.Role, ',');

            if (U.Username == UsernameBaru) {
                cout << "\n[!] Username sudah terdaftar. Gunakan nama lain!\n";
                FileIn.close();
                return false;
            }
        }
        FileIn.close();
    }

    while (true) {
        cout << "Masukkan Password (Ketik '0' untuk batal, min. 5 karakter): ";
        getline(cin, PasswordBaru);

        if (PasswordBaru.empty()) {
            cout << "[!] Username dan password tidak boleh kosong!\n";
            continue;
        }
        if (PasswordBaru.find(' ') != string::npos) {
            cout << "[!] Password tidak boleh menggunakan spasi!\n";
            continue;
        }
        if (PasswordBaru == "0") return false; 
        if (PasswordBaru.length() < 5) {
            cout << "[!] Password terlalu pendek! Harus minimal 5 karakter.\n";
            continue;
        }
        break; 
    }

    ofstream FileOut("data/users.csv", ios::app);
    if (FileOut.is_open()) {
        FileOut << UsernameBaru << "," << PasswordBaru << ",pendonor\n";
        FileOut.close();
        cout << "\n[OK] Registrasi berhasil! Silakan login.\n";
        return true;
    }
    
    cout << "\n[!] Gagal menyimpan data ke database.\n";
    return false;
}

User LoginAkun() {
    string InputUser, InputPass;
    int Percobaan = 0;
    const int MaksPercobaan = 3;
    User UserData = {"", "", ""};

    while (true) {
        if (Percobaan >= MaksPercobaan) {
            Utils::bersihkanLayar();
            cout << "==========================================\n";
            cout << "       TERLALU BANYAK GAGAL LOGIN         \n";
            cout << "  Akun terkunci sementara demi keamanan.  \n";
            cout << "==========================================\n\n";
            
            int totalWaktu = 60;
            int lebarBar = 30; 

            for (int waktuBerjalan = 0; waktuBerjalan <= totalWaktu; waktuBerjalan++) {
                int sisaWaktu = totalWaktu - waktuBerjalan;
                float persentase = (float)waktuBerjalan / totalWaktu;
                int posisiBar = lebarBar * persentase;

                cout << "\r⏳ Membuka kunci: [";
                for (int j = 0; j < lebarBar; ++j) {
                    if (j < posisiBar) cout << "█"; 
                    else cout << " ";               
                }
                cout << "] " << sisaWaktu << " detik tersisa... " << flush;

                if (waktuBerjalan < totalWaktu) Tunggu(1); 
            }
            
            cout << "\n\n[OK] Kunci terbuka. Silakan coba lagi.\n";
            Tunggu(2); 

            cin.clear(); 
            cin.ignore(1000, '\n'); 
            Percobaan = 0; 
        }

        Utils::bersihkanLayar();
        cout << "=== LOGIN SISTEM ===\n";
        if (Percobaan > 0) {
            cout << "[!] Login Gagal (" << Percobaan << "/" << MaksPercobaan << ")\n\n";
        }

        cout << "Username (Ketik '0' untuk kembali): ";
        getline(cin, InputUser); 

        if (InputUser.empty()) {
            cout << "\n[!] Username dan password tidak boleh kosong!\n";
            Percobaan++;
            Tunggu(1);
            continue;
        }
        if (InputUser == "0") return UserData; 

        cout << "Password (Ketik '0' untuk kembali): ";
        getline(cin, InputPass);

        if (InputPass.empty()) {
            cout << "\n[!] Username dan password tidak boleh kosong!\n";
            Percobaan++;
            Tunggu(1);
            continue;
        }
        if (InputPass == "0") return UserData; 

        if (InputPass.length() < 5) {
            cout << "\n[!] Format salah. Password minimal 5 karakter!\n";
            Percobaan++; 
            Tunggu(1);
            continue; 
        }

        ifstream File("data/users.csv");
        if (!File.is_open()) {
            cout << "\n[!] Database tidak ditemukan!\n";
            cout << "[!] Silakan Register akun terlebih dahulu.\n";
            Tunggu(2);
            return UserData; 
        }

        bool loginBerhasil = false;
        string barisData;
        
        while (getline(File, barisData)) {
            stringstream ss(barisData);
            getline(ss, UserData.Username, ',');
            getline(ss, UserData.Password, ',');
            getline(ss, UserData.Role, ',');

            if (UserData.Username == InputUser && UserData.Password == InputPass) {
                loginBerhasil = true;
                break;
            }
        }
        File.close();

        if (loginBerhasil) {
            cout << "\n[OK] Berhasil masuk! Mohon tunggu...";
            Tunggu(1);
            return UserData; 
        } else {
            Percobaan++; 
            if (Percobaan < MaksPercobaan) {
                cout << "\nKredensial salah. Mengulang...";
                Tunggu(1);
            }
        }
    } 
} 

User MulaiAuth() {
    int Pilihan;
    User UserAktif = {"", "", ""};

    while (true) {
        Utils::bersihkanLayar();
        cout << "=== SISTEM DONOR DARAH PMI ===\n";
        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "3. Keluar\n";
        cout << "Pilih [1-3]: ";
        
        if (!(cin >> Pilihan)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        
        cin.ignore(1000, '\n'); 

        switch (Pilihan) {
            case 1:
                UserAktif = LoginAkun();
                if (UserAktif.Username != "") {
                    return UserAktif; 
                }
                break;
            case 2:
                RegisterAkun();
                Utils::tekanEnter();
                break;
            case 3:
                cout << "\nSampai jumpa lagi, Yar!\n";
                exit(0);
            default:
                cout << "\n[!] Pilihan tidak tersedia.\n";
                Utils::tekanEnter();
                break;
        }
    }
}