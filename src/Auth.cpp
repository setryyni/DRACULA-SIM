#include "../include/Auth.h"
#include "../include/Models.h" 
#include "../include/Utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <windows.h> // Untuk Windows


using namespace std;

// Fungsi pembantu untuk memberi jeda waktu
void Tunggu(int Detik) {
    Sleep(Detik * 1000); // Milidetik
}

bool RegisterAkun() {
    string UsernameBaru, PasswordBaru;
    
    Utils::bersihkanLayar();
    cout << "=== PENDAFTARAN PENDONOR ===\n";
    cout << "Masukkan Username (Ketik '0' untuk batal): ";
    cin >> UsernameBaru;

    // Batal saat input username
    if (UsernameBaru == "0") {
        return false; 
    }
    
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

    // Looping sampai user memasukkan password minimal 5 karakter atau batal
    while (true) {
        cout << "Masukkan Password (Ketik '0' untuk batal, min. 5 karakter): ";
        cin >> PasswordBaru;

        // Batal saat input password
        if (PasswordBaru == "0") {
            return false; 
        }

        if (PasswordBaru.length() < 5) {
            cout << "[!] Password terlalu pendek! Harus minimal 5 karakter.\n";
        } else {
            break; // Keluar dari loop jika password sudah valid
        }
    }

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
            
            Tunggu(60); // Jeda 120 detik (2 menit)
            Percobaan = 0; // Reset jumlah percobaan ke 0 setelah 2 menit
        }

        Utils::bersihkanLayar();
        cout << "=== LOGIN SISTEM ===\n";
        if (Percobaan > 0) {
            cout << "[!] Login Gagal (" << Percobaan << "/" << MaksPercobaan << ")\n\n";
        }

        cout << "Username (Ketik '0' untuk kembali): ";
        cin >> InputUser;

        // Batal saat input username
        if (InputUser == "0") {
            return UserData; // Kembali ke menu awal dengan membawa UserData kosong
        }

        cout << "Password (Ketik '0' untuk kembali): ";
        cin >> InputPass;

        // Batal saat input password
        if (InputPass == "0") {
            return UserData; 
        }

        if (InputPass.length() < 5) {
            cout << "\n[!] Format salah. Password minimal 5 karakter!\n";
            Percobaan++; // Tetap dihitung sebagai percobaan gagal
            Tunggu(1);
            continue; // Langsung ulang dari awal loop (minta username lagi)
        }

        ifstream File("data/users.txt");
        if (!File.is_open()) {
            cout << "\n[!] Username tidak ditemukan!\n";
            cout << "[!] Silakan Register akun terlebih dahulu.\n";
            Tunggu(2);
            return UserData; // Kembali ke menu utama
        }

        bool loginBerhasil = false;
        while (File >> UserData.Username >> UserData.Password >> UserData.Role) {
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

// ==========================================
// KODE TESTING (HAPUS NANTI KALAU SUDAH SELESAI)
// ==========================================
int main() {
    // 1. Buat data dummy langsung ke file (menimpa file lama kalau ada)
    // PENTING: Pastikan folder bernama "data" SUDAH ADA di dalam folder utama project-mu!
    ofstream FileDummy("data/users.txt");
    if (FileDummy.is_open()) {
        FileDummy << "admin admin123 admin\n";
        FileDummy << "budi budi123 pendonor\n";
        FileDummy.close();
    } else {
        cout << "[!] Gagal membuat file dummy. Bikin folder 'data' dulu ya!\n";
        Utils::tekanEnter();
    }

    // 2. Layar pembuka testing
    Utils::bersihkanLayar();
    cout << "========================================\n";
    cout << "       SIMULASI TESTING MENU AUTH       \n";
    cout << "========================================\n";
    cout << "Data Dummy yang siap digunakan Login:\n";
    cout << "1. User: admin | Pass: admin123 (Role: admin)\n";
    cout << "2. User: budi  | Pass: budi123  (Role: pendonor)\n";
    cout << "========================================\n";
    Utils::tekanEnter();

    // 3. Jalankan menu Auth
    User HasilLogin = MulaiAuth();

    // 4. Tangkap dan tampilkan data kembalian setelah login sukses
    Utils::bersihkanLayar();
    cout << "========================================\n";
    cout << "            TESTING SELESAI             \n";
    cout << "========================================\n";
    cout << "Kamu berhasil masuk sistem sebagai:\n";
    cout << "Username : " << HasilLogin.Username << "\n";
    cout << "Password : " << HasilLogin.Password << "\n";
    cout << "Role     : " << HasilLogin.Role << "\n";
    cout << "========================================\n";
    
    return 0;
}