#include "../include/AdminPage.h"
#include "../include/Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>

using namespace std;

// =============================================
// HELPER
// =============================================
void CetakHeaderTabel() {
    cout << left
         << setw(4)  << "No"
         << setw(15) << "Username"
         << setw(20) << "Nama"
         << setw(5)  << "Gol"
         << setw(5)  << "Rhs"
         << setw(13) << "No. Telp"
         << "\n";
    cout << string(62, '-') << "\n";
}

void CetakBarisPendonor(int No, const Pendonor& P) {
    cout << left
         << setw(4)  << No
         << setw(15) << P.Username
         << setw(20) << P.Nama
         << setw(5)  << P.GolDarah
         << setw(5)  << P.Rhesus
         << setw(13) << P.NomorTelepon
         << "\n";
}

void TampilSemuaPendonor(NodePendonor* Head) {
    CetakHeaderTabel();
    NodePendonor* Curr = Head;
    int No = 1;
    while (Curr != nullptr) {
        CetakBarisPendonor(No++, Curr->Data);
        Curr = Curr->Next;
    }
}

// =============================================
// MENU UTAMA ADMIN
// =============================================
void MenuAdmin(const User& UserAktif, NodePendonor*& Head, StokDarah& Stok) {
    int Pilihan;
    while (true) {
        Utils::bersihkanLayar();
        cout << "========================================\n";
        cout << "  MENU ADMIN - " << UserAktif.Username << "\n";
        cout << "========================================\n";
        cout << "1. Tambah Akun\n";
        cout << "2. Tampilkan Semua Akun\n";
        cout << "3. Update Riwayat Pendonor\n";
        cout << "4. Hapus Pendonor\n";
        cout << "5. Cek Stok Darah\n";
        cout << "6. Update Stok Darah\n";
        cout << "7. Kurangi Stok Darah\n";
        cout << "8. Verifikasi Data\n";
        cout << "9. Logout\n";
        cout << "========================================\n";
        cout << "Input pilihan: ";

        if (!(cin >> Pilihan)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (Pilihan) {
            case 1: AdminTambahAkun(Head);      break;
            case 2: AdminTampilkanAkun(Head);   break;
            case 3: AdminUpdateRiwayat(Head);   break;
            case 4: AdminHapusPendonor(Head);   break;
            case 5: AdminCekStok(Stok);         break;
            case 6: AdminUpdateStok(Stok);      break;
            case 7: AdminKurangiStok(Stok);     break;
            case 8: AdminVerifikasiDonor(Head); break;
            case 9:
                cout << "\nSampai Jumpa!\n";
                return;
            default:
                cout << "\n[!] Pilihan tidak valid!\n";
                break;
        }
        Utils::tekanEnter();
    }
}

// =============================================
// 1. TAMBAH AKUN
// =============================================
void AdminTambahAkun(NodePendonor*& Head) {
    bool TambahLagi = true;

    cin.ignore(); // flush buffer sekali di awal, sebelum loop
    while (TambahLagi) {
        Utils::bersihkanLayar();
        cout << "=== TAMBAH AKUN ===\n";
        cout << "(ketik '0' untuk batal)\n\n";

        string Username, Password, Role;

        // Input Username: tidak boleh kosong + cek duplikat + cancel
        while (true) {
            cout << "Username : ";
            getline(cin, Username);
            if (Username == "0") { cout << "[!] Dibatalkan.\n"; return; }
            if (Username.empty()) { cout << "[!] Username tidak boleh kosong!\n"; continue; }
            if (!CekUsernameAdaDiFile(Username)) break;
            cout << "[!] Username sudah terdaftar! Coba username lain.\n";
        }

        // Input Password: tidak boleh kosong + minimal 5 karakter + cancel
        while (true) {
            cout << "Password (min. 5 karakter) : ";
            getline(cin, Password);
            if (Password == "0") { cout << "[!] Dibatalkan.\n"; return; }
            if (Password.empty()) { cout << "[!] Password tidak boleh kosong!\n"; continue; }
            if (Password.length() < 5) { cout << "[!] Password minimal 5 karakter!\n"; continue; }
            break;
        }

        // Input Role + validasi + cancel
        while (true) {
            cout << "Role (user/admin) : ";
            getline(cin, Role);
            if (Role == "0") { cout << "[!] Dibatalkan.\n"; return; }
            if (Role == "user" || Role == "admin") break;
            cout << "[!] Role tidak valid! Masukkan 'user' atau 'admin'.\n";
        }

        // Simpan ke Users.txt
        ofstream FileUser("data/Users.txt", ios::app);
        if (FileUser.is_open()) {
            FileUser << Username << " " << Password << " " << Role << "\n";
            FileUser.close();
        }

        // Kalau role = user, langsung isi data diri pendonor
        if (Role == "user") {
            Pendonor P;
            P.Username = Username;

            cout << "\n--- Isi Data Diri Pendonor ---\n";

            // Input NIK: harus 16 digit angka
            while (true) {
                cout << "NIK (16 digit) : ";
                getline(cin, P.Nik);
                if (P.Nik.length() != 16) {
                    cout << "[!] NIK harus 16 digit!\n";
                    continue;
                }
                bool SemuaAngka = true;
                for (char c : P.Nik) {
                    if (!isdigit(c)) { SemuaAngka = false; break; }
                }
                if (!SemuaAngka) { cout << "[!] NIK harus berupa angka!\n"; continue; }
                break;
            }

            // Input Nama: tidak boleh kosong
            while (true) {
                cout << "Nama Lengkap   : ";
                getline(cin, P.Nama);
                if (!P.Nama.empty()) break;
                cout << "[!] Nama tidak boleh kosong!\n";
            }

            // Input Gol Darah: validasi A/B/AB/O
            while (true) {
                cout << "Gol. Darah (A/B/AB/O) : ";
                getline(cin, P.GolDarah);
                if (ValidasiGolDarah(P.GolDarah)) break;
                cout << "[!] Golongan darah tidak valid!\n";
            }

            // Input Rhesus: harus + atau -
            while (true) {
                cout << "Rhesus (+/-) : ";
                getline(cin, P.Rhesus);
                if (P.Rhesus == "+" || P.Rhesus == "-") break;
                cout << "[!] Rhesus harus '+' atau '-'!\n";
            }

            // Input Alamat: tidak boleh kosong
            while (true) {
                cout << "Alamat : ";
                getline(cin, P.Alamat);
                if (!P.Alamat.empty()) break;
                cout << "[!] Alamat tidak boleh kosong!\n";
            }

            // Input Nomor Telepon: tidak boleh kosong
            while (true) {
                cout << "Nomor Telepon : ";
                getline(cin, P.NomorTelepon);
                if (!P.NomorTelepon.empty()) break;
                cout << "[!] Nomor telepon tidak boleh kosong!\n";
            }

            // Pertanyaan riwayat donor sebelumnya + validasi y/n
            char PernaDonor;
            while (true) {
                cout << "\nPernah donor sebelumnya? (y/n) : ";
                cin >> PernaDonor;
                cin.ignore();
                if (PernaDonor == 'y' || PernaDonor == 'Y' ||
                    PernaDonor == 'n' || PernaDonor == 'N') break;
                cout << "[!] Input harus 'y' atau 'n'!\n";
            }

            if (PernaDonor == 'y' || PernaDonor == 'Y') {
                string TglTerakhir;
                // Input tanggal donor terakhir + validasi
                while (true) {
                    cout << "Tanggal donor terakhir (YYYY-MM-DD) : ";
                    getline(cin, TglTerakhir);
                    if (ValidasiTanggal(TglTerakhir)) break;
                    cout << "[!] Tanggal tidak valid! Pastikan format YYYY-MM-DD dan bukan tanggal masa depan.\n";
                }

                // Simpan ke Riwayat.txt sebagai data historis
                ofstream FileRiwayat("data/Riwayat.txt", ios::app);
                if (FileRiwayat.is_open()) {
                    FileRiwayat << Username << "|" << TglTerakhir << "|PMI|1|Data historis sebelum daftar\n";
                    FileRiwayat.close();
                }
            }

            TambahPendonor(Head, P);
            SimpanPendonorKeFile(Head);
        }

        cout << "\n[OK] Akun berhasil ditambahkan!\n";

        char Jawab;
        while (true) {
            cout << "Tambah akun lagi? (y/n): ";
            cin >> Jawab;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (Jawab == 'y' || Jawab == 'Y' || Jawab == 'n' || Jawab == 'N') break;
            cout << "[!] Input harus 'y' atau 'n'!\n";
        }
        TambahLagi = (Jawab == 'y' || Jawab == 'Y');
    }
}

// =============================================
// 2. TAMPILKAN AKUN
// =============================================
void AdminTampilkanAkun(NodePendonor*& Head) {
    Utils::bersihkanLayar();
    cout << "=== TAMPILKAN SEMUA AKUN ===\n";

    if (ListKosong(Head)) {
        cout << "[!] Belum ada data.\n";
        return;
    }

    int Pilihan;
    cout << "1. Tampil Semua Akun (User/Admin)\n";
    cout << "2. Sort Nama A-Z   (Selection Sort)\n";
    cout << "3. Sort Nama Z-A   (Insertion Sort)\n";
    cout << "4. Sort by Gol. Darah (Bubble Sort)\n";
    cout << "0. Kembali\n";
    cout << "Input pilihan: ";

    if (!(cin >> Pilihan)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "[!] Input tidak valid!\n";
        return;
    }

    if (Pilihan == 0) return;

    if (Pilihan == 1) {
        Utils::bersihkanLayar();
        cout << "=== DAFTAR SEMUA AKUN ===\n";
        TampilSemuaPendonor(Head);
        cout << "\nTotal: " << HitungPendonor(Head) << " akun\n";

        // Sub-menu searching
        cout << "\n--- SEARCHING ---\n";
        cout << "1. Cari by Username (Linear Search)\n";
        cout << "0. Kembali\n";
        cout << "Input pilihan: ";
        int PilSearch;
        cin >> PilSearch;

        if (PilSearch == 1) {
            string Query;
            cin.ignore();
            cout << "Input Username yang dicari (0=batal): ";
            getline(cin, Query);
            if (Query == "0") return;

            NodePendonor* Hasil = CariPendonorByUsername(Head, Query);
            if (Hasil != nullptr) {
                cout << "\n=== Hasil Pencarian ===\n";
                CetakHeaderTabel();
                CetakBarisPendonor(1, Hasil->Data);
            } else {
                cout << "\n[!] Data tidak ditemukan.\n";
            }
        }

    } else if (Pilihan >= 2 && Pilihan <= 4) {
        // Buat salinan untuk sorting
        NodePendonor* SortHead = nullptr;
        NodePendonor* Curr = Head;
        while (Curr != nullptr) {
            TambahPendonor(SortHead, Curr->Data);
            Curr = Curr->Next;
        }

        string JudulSort;
        if      (Pilihan == 2) { SortByNamaAZ(SortHead); JudulSort = "A-Z (Selection Sort)"; }
        else if (Pilihan == 3) { SortByNamaZA(SortHead); JudulSort = "Z-A (Insertion Sort)"; }
        else                   { SortByRole(SortHead);   JudulSort = "by Gol. Darah (Bubble Sort)"; }

        Utils::bersihkanLayar();
        cout << "=== DAFTAR AKUN TERURUT " << JudulSort << " ===\n";
        TampilSemuaPendonor(SortHead);

        // Sub-menu searching (Binary Search karena data terurut)
        cout << "\n--- SEARCHING (data terurut) ---\n";
        cout << "1. Cari by Nama (Binary Search)\n";
        cout << "0. Kembali\n";
        cout << "Input pilihan: ";
        int PilSearch;
        cin >> PilSearch;

        if (PilSearch == 1) {
            string Query;
            cin.ignore();
            cout << "Input Nama yang dicari (0=batal): ";
            getline(cin, Query);
            if (Query == "0") {
                // Bersihkan salinan sebelum return
                while (SortHead != nullptr) {
                    NodePendonor* Del = SortHead;
                    SortHead = SortHead->Next;
                    delete Del;
                }
                return;
            }

            int Size = HitungPendonor(SortHead);
            NodePendonor* Hasil = BinarySearchByNama(SortHead, Query, Size);
            if (Hasil != nullptr) {
                cout << "\n=== Hasil Pencarian ===\n";
                CetakHeaderTabel();
                CetakBarisPendonor(1, Hasil->Data);
            } else {
                cout << "\n[!] Data tidak ditemukan.\n";
            }
        }

        // Bersihkan salinan
        while (SortHead != nullptr) {
            NodePendonor* Del = SortHead;
            SortHead = SortHead->Next;
            delete Del;
        }

    } else {
        cout << "\n[!] Pilihan tidak valid!\n";
    }
}

// =============================================
// 3. UPDATE RIWAYAT PENDONOR
// =============================================
void AdminUpdateRiwayat(NodePendonor*& Head) {
    Utils::bersihkanLayar();
    cout << "=== UPDATE RIWAYAT PENDONOR ===\n";

    if (ListKosong(Head)) {
        cout << "[!] Belum ada data.\n";
        return;
    }

    TampilSemuaPendonor(Head);
    cout << "(ketik '0' untuk batal di langkah manapun)\n";

    string Username, TglBaru;
    cin.ignore();

    // Input username + LOOP + cancel
    while (true) {
        cout << "\nInput Username pendonor: ";
        getline(cin, Username);
        if (Username == "0") {
            cout << "[!] Dibatalkan.\n";
            return;
        }
        if (CariPendonorByUsername(Head, Username) != nullptr) break;
        cout << "[!] Username tidak ditemukan! Coba lagi.\n";
    }

    // Input tanggal + LOOP + cancel
    while (true) {
        cout << "Input Tanggal Baru (YYYY-MM-DD): ";
        getline(cin, TglBaru);
        if (TglBaru == "0") {
            cout << "[!] Dibatalkan.\n";
            return;
        }
        if (ValidasiTanggal(TglBaru)) break;
        cout << "[!] Tanggal tidak valid! Pastikan format YYYY-MM-DD dan bukan tanggal masa depan.\n";
    }

    // Input lokasi donor - tidak boleh kosong
    string Lokasi;
    while (true) {
        cout << "Input Lokasi donor (contoh: PMI_Pusat) : ";
        getline(cin, Lokasi);
        if (!Lokasi.empty()) break;
        cout << "[!] Lokasi tidak boleh kosong!\n";
    }

    // Input keterangan
    string Keterangan;
    while (true) {
        cout << "Keterangan (Sukses/Ditolak)           : ";
        getline(cin, Keterangan);
        if (Keterangan == "Sukses" || Keterangan == "Ditolak") break;
        cout << "[!] Keterangan harus 'Sukses' atau 'Ditolak'!\n";
    }

    // Simpan ke Riwayat.txt
    RiwayatDonor R;
    R.username      = Username;
    R.TanggalDonor  = TglBaru;
    R.Lokasi        = Lokasi;
    R.JumlahKantong = 1;
    R.Keterangan    = Keterangan;
    TambahRiwayat(R);

    cout << "\n[OK] Riwayat berhasil diperbarui!\n";
}

// =============================================
// 4. HAPUS PENDONOR
// =============================================
void AdminHapusPendonor(NodePendonor*& Head) {
    Utils::bersihkanLayar();
    cout << "=== HAPUS PENDONOR ===\n";

    if (ListKosong(Head)) {
        cout << "[!] Belum ada data.\n";
        return;
    }

    // Tampil semua pendonor dulu biar admin tau mau hapus siapa
    TampilSemuaPendonor(Head);
    cout << "(ketik '0' untuk batal)\n";

    string Username;
    cin.ignore();

    // Input username + LOOP + cancel
    while (true) {
        cout << "\nInput Username yang ingin dihapus: ";
        getline(cin, Username);
        if (Username == "0") {
            cout << "[!] Dibatalkan.\n";
            return;
        }
        if (CariPendonorByUsername(Head, Username) != nullptr) break;
        cout << "[!] Username tidak ditemukan! Coba lagi.\n";
    }

    // Konfirmasi y/n
    char Konfirmasi;
    cout << "Yakin hapus '" << Username << "'? (y/n): ";
    cin >> Konfirmasi;

    if (Konfirmasi != 'y' && Konfirmasi != 'Y') {
        cout << "\n[!] Penghapusan dibatalkan.\n";
        return;
    }

    // Hapus dari linked list + Pendonor.txt
    HapusPendonor(Head, Username);
    SimpanPendonorKeFile(Head);

    // Hapus dari Users.txt
    ifstream FileIn("data/Users.txt");
    ofstream FileTmp("data/Users_tmp.txt");
    string U, P, R;
    while (FileIn >> U >> P >> R) {
        if (U != Username) FileTmp << U << " " << P << " " << R << "\n";
    }
    FileIn.close();
    FileTmp.close();
    remove("data/Users.txt");
    rename("data/Users_tmp.txt", "data/Users.txt");

    // Hapus riwayat dari Riwayat.txt juga biar gak ada data orphan
    ifstream FileRIn("data/Riwayat.txt");
    ofstream FileRTmp("data/Riwayat_tmp.txt");
    string Line;
    while (getline(FileRIn, Line)) {
        if (Line.empty()) continue;
        istringstream Iss(Line);
        string UName;
        getline(Iss, UName, '|');
        if (UName != Username) FileRTmp << Line << "\n";
    }
    FileRIn.close();
    FileRTmp.close();
    remove("data/Riwayat.txt");
    rename("data/Riwayat_tmp.txt", "data/Riwayat.txt");

    cout << "\n[OK] Data berhasil dihapus!\n";
}

// =============================================
// 5. CEK STOK DARAH
// =============================================
void AdminCekStok(const StokDarah& Stok) {
    Utils::bersihkanLayar();
    cout << "=== CEK STOK DARAH ===\n";

    if (StokKosong(Stok)) {
        cout << "[!] Stok darah kosong.\n";
        return;
    }

    cout << left << setw(12) << "Gol. Darah" << setw(15) << "Jumlah Kantong" << "\n";
    cout << string(27, '-') << "\n";
    cout << left << setw(12) << "A"     << Stok.StokA  << "\n";
    cout << left << setw(12) << "B"     << Stok.StokB  << "\n";
    cout << left << setw(12) << "AB"    << Stok.StokAB << "\n";
    cout << left << setw(12) << "O"     << Stok.StokO  << "\n";
    cout << string(27, '-') << "\n";
    cout << left << setw(12) << "Total" << (Stok.StokA + Stok.StokB + Stok.StokAB + Stok.StokO) << "\n";
}

// =============================================
// 6. UPDATE STOK DARAH
// =============================================
void AdminUpdateStok(StokDarah& Stok) {
    Utils::bersihkanLayar();
    cout << "=== UPDATE STOK DARAH ===\n";
    cout << "Stok saat ini: A=" << Stok.StokA << " B=" << Stok.StokB
         << " AB=" << Stok.StokAB << " O=" << Stok.StokO << "\n";
    cout << "(ketik '0' di Gol. Darah untuk batal)\n\n";

    string GolDarah;
    int Jumlah;
    cin.ignore();

    while (true) {
        // Input Gol. Darah + cancel
        cout << "Input Gol. Darah (A/B/AB/O): ";
        getline(cin, GolDarah);
        if (GolDarah == "0") {
            cout << "[!] Dibatalkan.\n";
            return;
        }
        if (!ValidasiGolDarah(GolDarah)) {
            cout << "[!] Golongan darah tidak valid!\n";
            continue;
        }

        // Input Jumlah
        cout << "Input Jumlah Kantong: ";
        if (!(cin >> Jumlah)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "[!] Input tidak valid!\n";
            continue;
        }
        cin.ignore();

        if (Jumlah < 0) {
            cout << "[!] Jumlah harus > 0!\n";
            continue;
        }

        break;
    }

    TambahStok(Stok, GolDarah, Jumlah);
    SimpanStokKeFile(Stok);
    cout << "\n[OK] Stok darah berhasil diperbarui!\n";
    cout << "Stok " << GolDarah << " sekarang: " << GetStok(Stok, GolDarah) << " kantong\n";
}

// =============================================
// 7. KURANGI STOK DARAH
// =============================================
void AdminKurangiStok(StokDarah& Stok) {
    Utils::bersihkanLayar();
    cout << "=== KURANGI STOK DARAH ===\n";
    cout << "Stok saat ini: A=" << Stok.StokA << " B=" << Stok.StokB
         << " AB=" << Stok.StokAB << " O=" << Stok.StokO << "\n";
    cout << "(ketik '0' di Gol. Darah untuk batal)\n\n";

    string GolDarah;
    int Jumlah;
    cin.ignore();

    // Input Gol. Darah + LOOP + cancel
    while (true) {
        cout << "Input Gol. Darah (A/B/AB/O): ";
        getline(cin, GolDarah);
        if (GolDarah == "0") {
            cout << "[!] Dibatalkan.\n";
            return;
        }
        if (ValidasiGolDarah(GolDarah)) break;
        cout << "[!] Golongan darah tidak valid!\n";
    }

    // Input Jumlah
    cout << "Input Jumlah Diminta: ";
    if (!(cin >> Jumlah)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "[!] Input tidak valid!\n";
        return;
    }

    // Jumlah tidak valid → langsung return (sesuai flowchart, tidak loop)
    if (Jumlah <= 0) {
        cout << "\n[!] Jumlah harus > 0!\n";
        return;
    }

    // Stok tidak cukup → langsung return
    if (GetStok(Stok, GolDarah) < Jumlah) {
        cout << "\n[!] Stok tidak mencukupi!\n";
        cout << "Stok " << GolDarah << " tersedia: " << GetStok(Stok, GolDarah) << " kantong\n";
        return;
    }

    KurangiStok(Stok, GolDarah, Jumlah);
    SimpanStokKeFile(Stok);
    cout << "\n[OK] Stok darah berhasil dikurangi!\n";
    cout << "Stok " << GolDarah << " sekarang: " << GetStok(Stok, GolDarah) << " kantong\n";
}

// =============================================
// 8. VERIFIKASI DATA
// =============================================
void AdminVerifikasiDonor(NodePendonor*& Head) {
    Utils::bersihkanLayar();
    cout << "=== VERIFIKASI DATA ===\n";

    if (ListKosong(Head)) {
        cout << "[!] Belum ada data.\n";
        return;
    }

    TampilSemuaPendonor(Head);
    cout << "(ketik '0' untuk batal)\n";

    string Username;
    double Sistolik, Diastolik, Hemoglobin;
    cin.ignore();

    // Input username + LOOP + cancel
    while (true) {
        cout << "\nInput Username pendonor: ";
        getline(cin, Username);
        if (Username == "0") {
            cout << "[!] Dibatalkan.\n";
            return;
        }
        if (CariPendonorByUsername(Head, Username) != nullptr) break;
        cout << "[!] Username tidak ditemukan di data pendonor!\n";
        cout << "    (Akun admin tidak bisa diverifikasi, pastikan username pendonor)\n";
    }
    cout << "\nInput data medis:\n";

    while (true) {
        cout << "Sistolik   (100-140 mmHg) : ";
        if (cin >> Sistolik && Sistolik >= 50 && Sistolik <= 250) break;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "[!] Input tidak valid! Sistolik harus angka antara 50-250.\n";
    }

    while (true) {
        cout << "Diastolik  (60-90 mmHg)   : ";
        if (cin >> Diastolik && Diastolik >= 30 && Diastolik <= 150) break;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "[!] Input tidak valid! Diastolik harus angka antara 30-150.\n";
    }

    while (true) {
        cout << "Hemoglobin (>=12.5 g/dL)  : ";
        if (cin >> Hemoglobin && Hemoglobin >= 1 && Hemoglobin <= 25) break;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "[!] Input tidak valid! Hemoglobin harus angka antara 1-25.\n";
    }
    cin.ignore();

    NodePendonor* Node = CariPendonorByUsername(Head, Username);
    cout << "\n" << string(45, '=') << "\n";
    cout << "HASIL VERIFIKASI: " << Node->Data.Nama << "\n";
    cout << string(45, '=') << "\n";

    // Cek kondisi medis
    bool MedisTidakNormal = !(Sistolik  >= 100 && Sistolik  <= 140 &&
                              Diastolik >= 60  && Diastolik <= 90  &&
                              Hemoglobin >= 12.5);

    if (MedisTidakNormal) {
        cout << "[X] Tidak layak donor - kondisi medis tidak normal:\n";
        if (Sistolik  < 100 || Sistolik  > 140)
            cout << "    - Sistolik  : " << Sistolik  << " mmHg (normal: 100-140)\n";
        if (Diastolik < 60  || Diastolik > 90)
            cout << "    - Diastolik : " << Diastolik << " mmHg (normal: 60-90)\n";
        if (Hemoglobin < 12.5)
            cout << "    - Hemoglobin: " << Hemoglobin << " g/dL (minimal: 12.5)\n";
        return;
    }

    cout << "[OK] Kondisi medis: Normal\n";

    // Ambil tanggal terakhir dari Riwayat.txt
    string TglTerakhir = AmbilTglTerakhir(Username);
    if (TglTerakhir == "-") {
        cout << "[OK] Output Status: LAYAK DONOR!\n";
        cout << "     Belum ada riwayat donor sebelumnya.\n";
        return;
    }

    int Selisih = HitungSelisihHari(TglTerakhir);
    if (Selisih < 90) {
        int SisaHari = 90 - Selisih;
        cout << "[X] Output Status: Ga layak donor\n";
        cout << "    Donor terakhir: " << TglTerakhir << " (" << Selisih << " hari lalu)\n";
        cout << "    Masih perlu " << SisaHari << " hari lagi baru boleh donor.\n";
        return;
    }

    cout << "[OK] Output Status: LAYAK DONOR!\n";
    cout << "     Donor terakhir: " << TglTerakhir
         << " (" << Selisih << " hari lalu)\n";
}