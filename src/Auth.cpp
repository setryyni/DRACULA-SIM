#include "../include/Auth.h"
#include "../include/Models.h" 
#include "../include/Utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

// Deteksi OS untuk fungsi sleep yang simpel
#ifdef _WIN32
    #include <windows.h> // Untuk Windows
#else
    #include <unistd.h>  // Untuk Linux/Mac
#endif

using namespace std;

// Fungsi pembantu untuk memberi jeda waktu
void Tunggu(int Detik) {
    #ifdef _WIN32
        Sleep(Detik * 1000); // Milidetik
    #else
        sleep(Detik); // Detik
    #endif
}

bool RegisterAkun() {
    string UsernameBaru, PasswordBaru;
    
    Utils::bersihkanLayar();
    cout << "=== PENDAFTARAN PENDONOR ===\n";
    cout << "Masukkan Username : ";
    cin >> UsernameBaru;
    
    // Cek apakah username sudah ada di database
    ifstream FileIn("data/users.txt");
    User U;
    while (FileIn >> U.Username >> U.Password >> U.Role) {
        if (U.Username == UsernameBaru) {
            cout << "\n[!] Username sudah terdaftar. Gunakan nama lain!\n";
            FileIn.close();
            return false;
        }
    }
    FileIn.close();

    cout << "Masukkan Password : ";
    cin >> PasswordBaru;

    // Simpan ke database (append mode)
    ofstream FileOut("data/users.txt", ios::app);
    if (FileOut.is_open()) {
        FileOut << UsernameBaru << " " << PasswordBaru << " pendonor\n";
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

    // Menggunakan infinite loop agar bisa reset percobaan setelah waktu tunggu 2 menit
    while (true) {
        // Cek jika percobaan sudah mencapai 3 kali gagal
        if (Percobaan >= MaksPercobaan) {
            Utils::bersihkanLayar();
            cout << "==========================================\n";
            cout << "       TERLALU BANYAK GAGAL LOGIN         \n";
            cout << "  Akun terkunci sementara demi keamanan.  \n";
            cout << "  Silakan tunggu 2 menit (120 detik)...   \n";
            cout << "==========================================\n";
            
            Tunggu(120); // Jeda 120 detik (2 menit)
            Percobaan = 0; // Reset jumlah percobaan ke 0 setelah 2 menit
        }

        Utils::bersihkanLayar();
        cout << "=== LOGIN SISTEM ===\n";
        if (Percobaan > 0) {
            cout << "[!] Login Gagal (" << Percobaan << "/" << MaksPercobaan << ")\n\n";
        }

        cout << "Username : ";
        cin >> InputUser;
        cout << "Password : ";
        cin >> InputPass;

        ifstream File("data/users.txt");
        if (!File.is_open()) {
            cout << "\n[!] Username tidak ditemukan!\n";
            cout << "[!] Silakan Register akun terlebih dahulu.\n";
            Tunggu(2);
            return UserData; // Kembali ke menu utama
        }

        bool loginBerhasil = false;
        while (File >> UserData.Username >> UserData.Password >> UserData.Role) {
            // Operator == pada string otomatis case-sensitive (huruf besar/kecil harus sama persis)
            if (UserData.Username == InputUser && UserData.Password == InputPass) {
                loginBerhasil = true;
                break;
            }
        }
        File.close();

        if (loginBerhasil) {
            cout << "\n[OK] Berhasil masuk! Mohon tunggu...";
            Tunggu(1);
            return UserData; // Mengembalikan data user yang berhasil login
        } else {
            Percobaan++; // Tambah jumlah gagal
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
        
        // Validasi jika input bukan angka
        if (!(cin >> Pilihan)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (Pilihan) {
            case 1:
                UserAktif = LoginAkun();
                if (UserAktif.Username != "") {
                    return UserAktif; // Keluar dari loop jika login sukses
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