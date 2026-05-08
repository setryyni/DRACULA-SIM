#include "../include/AdminPage.h"
#include "../include/Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>

using namespace std;

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

void TampilAkunAdmin(NodePendonor* Head) {
    cout << left
         << setw(4)  << "No"
         << setw(15) << "Username"
         << setw(20) << "Password"
         << setw(8)  << "Role"
         << "\n";
    cout << string(47, '-') << "\n";

    ifstream File("data/Users.csv");
    if (!File.is_open()) return;

    string Line;
    int No = 1;
    while (getline(File, Line)) {
        if (Line.empty()) continue;
        istringstream Iss(Line);
        string U, P, R;
        getline(Iss, U, ',');
        getline(Iss, P, ',');
        getline(Iss, R);
        if (R == "admin") {
            cout << left
                 << setw(4)  << No++
                 << setw(15) << U
                 << setw(20) << P
                 << setw(8)  << R
                 << "\n";
        }
    }
    File.close();
}

void TampilAkunUser(NodePendonor* Head) {
    cout << left
         << setw(4)  << "No"
         << setw(12) << "Username"
         << setw(10) << "Password"
         << setw(18) << "NIK"
         << setw(18) << "Nama"
         << setw(5)  << "Gol"
         << setw(5)  << "Rhs"
         << setw(35) << "Alamat"
         << setw(15) << "No. Telp"
         << "\n";
    cout << string(122, '-') << "\n";

    ifstream File("data/Users.csv");
    if (!File.is_open()) return;

    string Line;
    int No = 1;
    while (getline(File, Line)) {
        if (Line.empty()) continue;
        istringstream Iss(Line);
        string U, P, R;
        getline(Iss, U, ',');
        getline(Iss, P, ',');
        getline(Iss, R);
        if (R == "user" || R == "pendonor") {
            NodePendonor* Node = CariPendonorByUsername(Head, U);

            string Alamat = Node ? Node->Data.Alamat : "-";
            if (Alamat.length() > 33) Alamat = Alamat.substr(0, 30) + "...";

            cout << left
                 << setw(4)  << No++
                 << setw(12) << U
                 << setw(10) << P
                 << setw(18) << (Node ? Node->Data.Nik          : "-")
                 << setw(18) << (Node ? Node->Data.Nama         : "-")
                 << setw(5)  << (Node ? Node->Data.GolDarah     : "-")
                 << setw(5)  << (Node ? Node->Data.Rhesus       : "-")
                 << setw(35) << Alamat
                 << setw(15) << (Node ? Node->Data.NomorTelepon : "-")
                 << "\n";
        }
    }
    File.close();
}

void TampilSemuaAkun(NodePendonor* Head) {
    cout << left
         << setw(4)  << "No"
         << setw(15) << "Username"
         << setw(20) << "Nama"
         << setw(8)  << "Role"
         << setw(5)  << "Gol"
         << setw(5)  << "Rhs"
         << setw(13) << "No. Telp"
         << "\n";
    cout << string(70, '-') << "\n";

    ifstream File("data/Users.csv");
    if (!File.is_open()) return;

    string Line;
    int No = 1;
    while (getline(File, Line)) {
        if (Line.empty()) continue;
        istringstream Iss(Line);
        string U, P, R;
        getline(Iss, U, ',');
        getline(Iss, P, ',');
        getline(Iss, R);
        NodePendonor* Node = CariPendonorByUsername(Head, U);
        cout << left
             << setw(4)  << No++
             << setw(15) << U
             << setw(20) << (Node ? Node->Data.Nama         : "-")
             << setw(8)  << R
             << setw(5)  << (Node ? Node->Data.GolDarah     : "-")
             << setw(5)  << (Node ? Node->Data.Rhesus       : "-")
             << setw(13) << (Node ? Node->Data.NomorTelepon : "-")
             << "\n";
    }
    File.close();
}

void MenuAdmin(const User& UserAktif, NodePendonor*& Head, StokDarah& Stok) {
    int Pilihan;
    while (true) {
        Utils::bersihkanLayar();
        cout << "========================================\n";
        cout << "  MENU ADMIN - " << UserAktif.Username << "\n";
        cout << "========================================\n";
        cout << "1. Tambah Akun\n";
        cout << "2. Tampilkan Akun\n";
        cout << "3. Edit Data Diri Pendonor\n";
        cout << "4. Update Riwayat Pendonor\n";
        cout << "5. Hapus Pendonor\n";
        cout << "6. Cek Stok Darah\n";
        cout << "7. Update Stok Darah\n";
        cout << "8. Kurangi Stok Darah\n";
        cout << "9. Verifikasi Data\n";
        cout << "0. Logout\n";
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
            case 3: AdminEditDataDiri(Head);    break;
            case 4: AdminUpdateRiwayat(Head);   break;
            case 5: AdminHapusPendonor(Head);   break;
            case 6: AdminCekStok(Stok);         break;
            case 7: AdminUpdateStok(Stok);      break;
            case 8: AdminKurangiStok(Stok);     break;
            case 9: AdminVerifikasiDonor(Head); break;
            case 0:
                cout << "\nSampai Jumpa!\n";
                return;
            default:
                cout << "\n[!] Pilihan tidak valid!\n";
                break;
        }
        Utils::tekanEnter();
    }
}

void AdminTambahAkun(NodePendonor*& Head) {
    bool TambahLagi = true;

    cin.ignore();
    while (TambahLagi) {
        Utils::bersihkanLayar();
        cout << "=== TAMBAH AKUN ===\n";
        cout << "(ketik '0' untuk batal)\n\n";

        string Username, Password, Role;

        while (true) {
            cout << "Username : ";
            getline(cin, Username);
            if (Username == "0") { cout << "[!] Dibatalkan.\n"; return; }
            if (Username.empty()) { cout << "[!] Username tidak boleh kosong!\n"; continue; }
            if (!CekUsernameAdaDiFile(Username)) break;
            cout << "[!] Username sudah terdaftar! Coba username lain.\n";
        }

        while (true) {
            cout << "Password (min. 5 karakter) : ";
            getline(cin, Password);
            if (Password == "0") { cout << "[!] Dibatalkan.\n"; return; }
            if (Password.empty()) { cout << "[!] Password tidak boleh kosong!\n"; continue; }
            if (Password.length() < 5) { cout << "[!] Password minimal 5 karakter!\n"; continue; }
            break;
        }

        while (true) {
            cout << "Role (user/admin) : ";
            getline(cin, Role);
            if (Role == "0") { cout << "[!] Dibatalkan.\n"; return; }
            if (Role == "user" || Role == "admin") break;
            cout << "[!] Role tidak valid! Masukkan 'user' atau 'admin'.\n";
        }

        {
            bool FileKosong = true;
            ifstream CekAkhir("data/Users.csv");
            if (CekAkhir.is_open()) {
                CekAkhir.seekg(0, ios::end);
                FileKosong = (CekAkhir.tellg() == 0);
                CekAkhir.close();
            }

            ofstream FileUser("data/Users.csv", ios::app);
            if (FileUser.is_open()) {
                if (!FileKosong) FileUser << "\n";
                FileUser << Username << "," << Password << "," << Role;
                FileUser.close();
            }
        }

        if (Role == "user") {
            Pendonor P;
            P.Username = Username;

            cout << "\n--- Isi Data Diri Pendonor ---\n";
            cout << "(ketik '0' di field manapun untuk batal, data tidak akan tersimpan)\n\n";

            while (true) {
                cout << "NIK (16 digit) : ";
                getline(cin, P.Nik);
                if (P.Nik == "0") {
                    ifstream FileIn("data/Users.csv");
                    ofstream FileTmp("data/Users_tmp.csv");
                    string Line;
                    bool Pertama = true;
                    while (getline(FileIn, Line)) {
                        if (Line.empty()) continue;
                        istringstream Iss(Line);
                        string U, Pw, R;
                        getline(Iss, U, ',');
                        getline(Iss, Pw, ',');
                        getline(Iss, R);
                        if (U != Username) {
                            if (!Pertama) FileTmp << "\n";
                            FileTmp << U << "," << Pw << "," << R;
                            Pertama = false;
                        }
                    }
                    FileIn.close(); FileTmp.close();
                    remove("data/Users.csv");
                    rename("data/Users_tmp.csv", "data/Users.csv");
                    cout << "[!] Dibatalkan. Akun tidak jadi ditambahkan.\n";
                    return;
                }
                if (P.Nik.length() != 16) { cout << "[!] NIK harus 16 digit!\n"; continue; }
                bool SemuaAngka = true;
                for (char c : P.Nik) if (!isdigit(c)) { SemuaAngka = false; break; }
                if (!SemuaAngka) { cout << "[!] NIK harus berupa angka!\n"; continue; }
                break;
            }

            while (true) {
                cout << "Nama Lengkap   : ";
                getline(cin, P.Nama);
                if (P.Nama == "0") {
                    ifstream FileIn("data/Users.csv");
                    ofstream FileTmp("data/Users_tmp.csv");
                    string Line; bool Pertama = true;
                    while (getline(FileIn, Line)) {
                        if (Line.empty()) continue;
                        istringstream Iss(Line);
                        string U, Pw, R;
                        getline(Iss, U, ','); getline(Iss, Pw, ','); getline(Iss, R);
                        if (U != Username) {
                            if (!Pertama) FileTmp << "\n";
                            FileTmp << U << "," << Pw << "," << R;
                            Pertama = false;
                        }
                    }
                    FileIn.close(); FileTmp.close();
                    remove("data/Users.csv"); rename("data/Users_tmp.csv", "data/Users.csv");
                    cout << "[!] Dibatalkan. Akun tidak jadi ditambahkan.\n";
                    return;
                }
                if (!P.Nama.empty()) break;
                cout << "[!] Nama tidak boleh kosong!\n";
            }

            while (true) {
                cout << "Gol. Darah (A/B/AB/O) : ";
                getline(cin, P.GolDarah);
                if (P.GolDarah == "0") {
                    ifstream FileIn("data/Users.csv");
                    ofstream FileTmp("data/Users_tmp.csv");
                    string Line; bool Pertama = true;
                    while (getline(FileIn, Line)) {
                        if (Line.empty()) continue;
                        istringstream Iss(Line);
                        string U, Pw, R;
                        getline(Iss, U, ','); getline(Iss, Pw, ','); getline(Iss, R);
                        if (U != Username) {
                            if (!Pertama) FileTmp << "\n";
                            FileTmp << U << "," << Pw << "," << R;
                            Pertama = false;
                        }
                    }
                    FileIn.close(); FileTmp.close();
                    remove("data/Users.csv"); rename("data/Users_tmp.csv", "data/Users.csv");
                    cout << "[!] Dibatalkan. Akun tidak jadi ditambahkan.\n";
                    return;
                }
                P.GolDarah = NormalisasiGolDarah(P.GolDarah);
                if (ValidasiGolDarah(P.GolDarah)) break;
                cout << "[!] Golongan darah tidak valid!\n";
            }

            while (true) {
                cout << "Rhesus (+/-) : ";
                getline(cin, P.Rhesus);
                if (P.Rhesus == "0") {
                    ifstream FileIn("data/Users.csv");
                    ofstream FileTmp("data/Users_tmp.csv");
                    string Line; bool Pertama = true;
                    while (getline(FileIn, Line)) {
                        if (Line.empty()) continue;
                        istringstream Iss(Line);
                        string U, Pw, R;
                        getline(Iss, U, ','); getline(Iss, Pw, ','); getline(Iss, R);
                        if (U != Username) {
                            if (!Pertama) FileTmp << "\n";
                            FileTmp << U << "," << Pw << "," << R;
                            Pertama = false;
                        }
                    }
                    FileIn.close(); FileTmp.close();
                    remove("data/Users.csv"); rename("data/Users_tmp.csv", "data/Users.csv");
                    cout << "[!] Dibatalkan. Akun tidak jadi ditambahkan.\n";
                    return;
                }
                if (P.Rhesus == "+" || P.Rhesus == "-") break;
                cout << "[!] Rhesus harus '+' atau '-'!\n";
            }

            while (true) {
                cout << "Alamat : ";
                getline(cin, P.Alamat);
                if (P.Alamat == "0") {
                    ifstream FileIn("data/Users.csv");
                    ofstream FileTmp("data/Users_tmp.csv");
                    string Line; bool Pertama = true;
                    while (getline(FileIn, Line)) {
                        if (Line.empty()) continue;
                        istringstream Iss(Line);
                        string U, Pw, R;
                        getline(Iss, U, ','); getline(Iss, Pw, ','); getline(Iss, R);
                        if (U != Username) {
                            if (!Pertama) FileTmp << "\n";
                            FileTmp << U << "," << Pw << "," << R;
                            Pertama = false;
                        }
                    }
                    FileIn.close(); FileTmp.close();
                    remove("data/Users.csv"); rename("data/Users_tmp.csv", "data/Users.csv");
                    cout << "[!] Dibatalkan. Akun tidak jadi ditambahkan.\n";
                    return;
                }
                if (!P.Alamat.empty()) break;
                cout << "[!] Alamat tidak boleh kosong!\n";
            }

            while (true) {
                cout << "Nomor Telepon (11-13 digit) : ";
                getline(cin, P.NomorTelepon);
                if (P.NomorTelepon == "0") {
                    ifstream FileIn("data/Users.csv");
                    ofstream FileTmp("data/Users_tmp.csv");
                    string Line; bool Pertama = true;
                    while (getline(FileIn, Line)) {
                        if (Line.empty()) continue;
                        istringstream Iss(Line);
                        string U, Pw, R;
                        getline(Iss, U, ','); getline(Iss, Pw, ','); getline(Iss, R);
                        if (U != Username) {
                            if (!Pertama) FileTmp << "\n";
                            FileTmp << U << "," << Pw << "," << R;
                            Pertama = false;
                        }
                    }
                    FileIn.close(); FileTmp.close();
                    remove("data/Users.csv"); rename("data/Users_tmp.csv", "data/Users.csv");
                    cout << "[!] Dibatalkan. Akun tidak jadi ditambahkan.\n";
                    return;
                }
                if (P.NomorTelepon.length() < 11 || P.NomorTelepon.length() > 13) {
                    cout << "[!] Nomor telepon harus 11-13 digit!\n"; continue;
                }
                bool SemuaAngka = true;
                for (char c : P.NomorTelepon) if (!isdigit(c)) { SemuaAngka = false; break; }
                if (!SemuaAngka) { cout << "[!] Nomor telepon harus berupa angka!\n"; continue; }
                break;
            }

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
                while (true) {
                    cout << "Tanggal donor terakhir (YYYY-MM-DD) : ";
                    getline(cin, TglTerakhir);
                    if (ValidasiTanggal(TglTerakhir)) break;
                    cout << "[!] Tanggal tidak valid! Pastikan format YYYY-MM-DD dan bukan tanggal masa depan.\n";
                }

                // Fix newline Riwayat.csv
                bool RiwayatKosong = true;
                ifstream CekRiwayat("data/Riwayat.csv");
                if (CekRiwayat.is_open()) {
                    CekRiwayat.seekg(0, ios::end);
                    RiwayatKosong = (CekRiwayat.tellg() == 0);
                    CekRiwayat.close();
                }
                ofstream FileRiwayat("data/Riwayat.csv", ios::app);
                if (FileRiwayat.is_open()) {
                    if (!RiwayatKosong) FileRiwayat << "\n";
                    FileRiwayat << Username << "," << TglTerakhir << ",PMI,1,Sukses";
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

void AdminTampilkanAkun(NodePendonor*& Head) {
    Utils::bersihkanLayar();
    cout << "=== TAMPILKAN SEMUA AKUN ===\n";

    ifstream CekFile("data/Users.csv");
    bool FileKosong = CekFile.peek() == ifstream::traits_type::eof();
    CekFile.close();

    if (FileKosong) {
        cout << "[!] Belum ada data.\n";
        return;
    }

    int Pilihan;
    cout << "1. Tampilkan Akun Admin\n";
    cout << "2. Tampilkan Akun User\n";
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
        cout << "=== DAFTAR AKUN ADMIN ===\n";
        TampilAkunAdmin(Head);

    } else if (Pilihan == 2) {
        Utils::bersihkanLayar();
        cout << "=== DAFTAR AKUN USER ===\n";
        TampilAkunUser(Head);

        cout << "\n1. Sort Nama A-Z   (Selection Sort)\n";
        cout << "2. Sort Nama Z-A   (Insertion Sort)\n";
        cout << "3. Sort by Gol. Darah (Bubble Sort)\n";
        cout << "4. Linear Search by Username\n";
        cout << "0. Kembali\n";
        cout << "Input pilihan: ";

        int PilSub;
        if (!(cin >> PilSub)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "[!] Input tidak valid!\n";
            return;
        }

        if (PilSub == 0) return;

        if (PilSub >= 1 && PilSub <= 3) {
            NodePendonor* SortHead = nullptr;
            NodePendonor* Curr = Head;
            while (Curr != nullptr) {
                TambahPendonor(SortHead, Curr->Data);
                Curr = Curr->Next;
            }

            string JudulSort;
            if      (PilSub == 1) { SortByNamaAZ(SortHead); JudulSort = "A-Z (Selection Sort)"; }
            else if (PilSub == 2) { SortByNamaZA(SortHead); JudulSort = "Z-A (Insertion Sort)"; }
            else                  { SortByRole(SortHead);   JudulSort = "by Gol. Darah (Bubble Sort)"; }

            Utils::bersihkanLayar();
            cout << "=== DAFTAR AKUN USER TERURUT " << JudulSort << " ===\n";
            TampilSemuaPendonor(SortHead);

            string Query;
            cin.ignore();
            cout << "\nInput Nama yang dicari (0=batal): ";
            getline(cin, Query);

            if (Query != "0") {
                int Size = HitungPendonor(SortHead);
                NodePendonor* Hasil = BinarySearchByNama(SortHead, Query, Size);
                if (Hasil != nullptr) {
                    cout << "\n=== Hasil Pencarian (Binary Search) ===\n";
                    CetakHeaderTabel();
                    CetakBarisPendonor(1, Hasil->Data);
                } else {
                    cout << "\n[!] Data tidak ditemukan.\n";
                }
            }

            while (SortHead != nullptr) {
                NodePendonor* Del = SortHead;
                SortHead = SortHead->Next;
                delete Del;
            }

        } else if (PilSub == 4) {
            string Query;
            cin.ignore();
            cout << "Input Username yang dicari (0=batal): ";
            getline(cin, Query);
            if (Query == "0") return;

            NodePendonor* Hasil = CariPendonorByUsername(Head, Query);
            if (Hasil != nullptr) {
                cout << "\n=== Hasil Pencarian (Linear Search) ===\n";
                CetakHeaderTabel();
                CetakBarisPendonor(1, Hasil->Data);
            } else {
                cout << "\n[!] Data tidak ditemukan.\n";
            }
        } else {
            cout << "\n[!] Pilihan tidak valid!\n";
        }
    } else {
        cout << "\n[!] Pilihan tidak valid!\n";
    }
}

void AdminEditDataDiri(NodePendonor*& Head) {
    Utils::bersihkanLayar();
    cout << "=== EDIT DATA DIRI PENDONOR ===\n";

    if (Head == nullptr) {
        cout << "[!] Belum ada data pendonor.\n";
        return;
    }

    TampilSemuaPendonor(Head);
    cout << "(ketik '0' untuk batal)\n";

    string Username;
    cin.ignore();

    while (true) {
        cout << "\nInput Username pendonor yang ingin diedit: ";
        getline(cin, Username);
        if (Username == "0") { cout << "[!] Dibatalkan.\n"; return; }
        if (CariPendonorByUsername(Head, Username) != nullptr) break;
        cout << "[!] Username tidak ditemukan! Coba lagi.\n";
    }

    NodePendonor* Node = CariPendonorByUsername(Head, Username);
    Pendonor& P = Node->Data;

    cout << "\nData saat ini:\n";
    cout << string(40, '-') << "\n";
    cout << "1. Username     : " << P.Username    << "\n";
    cout << "2. NIK          : " << P.Nik         << "\n";
    cout << "3. Nama         : " << P.Nama        << "\n";
    cout << "4. Gol. Darah   : " << P.GolDarah    << "\n";
    cout << "5. Rhesus       : " << P.Rhesus      << "\n";
    cout << "6. Alamat       : " << P.Alamat      << "\n";
    cout << "7. No. Telepon  : " << P.NomorTelepon << "\n";
    cout << "8. Password\n";
    cout << "0. Batal\n";
    cout << string(40, '-') << "\n";

    int Pilihan;
    cout << "Pilih field yang ingin diedit: ";
    if (!(cin >> Pilihan)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "[!] Input tidak valid!\n";
        return;
    }
    cin.ignore();

    if (Pilihan == 0) { cout << "[!] Dibatalkan.\n"; return; }

    string NilaBaru;

    switch (Pilihan) {
        case 1: {
            while (true) {
                cout << "Username baru (0=batal): ";
                getline(cin, NilaBaru);
                if (NilaBaru == "0") { cout << "[!] Dibatalkan.\n"; return; }
                if (NilaBaru.empty()) { cout << "[!] Username tidak boleh kosong!\n"; continue; }
                if (CekUsernameAdaDiFile(NilaBaru)) { cout << "[!] Username sudah terdaftar!\n"; continue; }
                break;
            }
            string UsernameLama = P.Username;
            P.Username = NilaBaru;
            SimpanPendonorKeFile(Head);

            ifstream FileIn("data/Users.csv");
            ofstream FileTmp("data/Users_tmp.csv");
            string Line; bool Pertama = true;
            while (getline(FileIn, Line)) {
                if (Line.empty()) continue;
                istringstream Iss(Line);
                string U, Pw, R;
                getline(Iss, U, ',');
                getline(Iss, Pw, ',');
                getline(Iss, R);
                if (!Pertama) FileTmp << "\n";
                if (U == UsernameLama) FileTmp << NilaBaru << "," << Pw << "," << R;
                else FileTmp << U << "," << Pw << "," << R;
                Pertama = false;
            }
            FileIn.close(); FileTmp.close();
            remove("data/Users.csv");
            rename("data/Users_tmp.csv", "data/Users.csv");

            ifstream FileRIn("data/Riwayat.csv");
            ofstream FileRTmp("data/Riwayat_tmp.csv");
            Pertama = true;
            while (getline(FileRIn, Line)) {
                if (Line.empty()) continue;
                istringstream Iss(Line);
                string U2; getline(Iss, U2, ',');
                string Sisa; getline(Iss, Sisa);
                if (!Pertama) FileRTmp << "\n";
                if (U2 == UsernameLama) FileRTmp << NilaBaru << "," << Sisa;
                else FileRTmp << U2 << "," << Sisa;
                Pertama = false;
            }
            FileRIn.close(); FileRTmp.close();
            remove("data/Riwayat.csv");
            rename("data/Riwayat_tmp.csv", "data/Riwayat.csv");

            cout << "[OK] Username berhasil diubah!\n";
            break;
        }
        case 2: {
            while (true) {
                cout << "NIK baru (16 digit, 0=batal): ";
                getline(cin, NilaBaru);
                if (NilaBaru == "0") { cout << "[!] Dibatalkan.\n"; return; }
                if (NilaBaru.length() != 16) { cout << "[!] NIK harus 16 digit!\n"; continue; }
                bool SemuaAngka = true;
                for (char c : NilaBaru) if (!isdigit(c)) { SemuaAngka = false; break; }
                if (!SemuaAngka) { cout << "[!] NIK harus berupa angka!\n"; continue; }
                break;
            }
            P.Nik = NilaBaru;
            SimpanPendonorKeFile(Head);
            cout << "[OK] NIK berhasil diubah!\n";
            break;
        }
        case 3: {
            while (true) {
                cout << "Nama baru (0=batal): ";
                getline(cin, NilaBaru);
                if (NilaBaru == "0") { cout << "[!] Dibatalkan.\n"; return; }
                if (!NilaBaru.empty()) break;
                cout << "[!] Nama tidak boleh kosong!\n";
            }
            P.Nama = NilaBaru;
            SimpanPendonorKeFile(Head);
            cout << "[OK] Nama berhasil diubah!\n";
            break;
        }
        case 4: {
            while (true) {
                cout << "Gol. Darah baru (A/B/AB/O, 0=batal): ";
                getline(cin, NilaBaru);
                if (NilaBaru == "0") { cout << "[!] Dibatalkan.\n"; return; }
                NilaBaru = NormalisasiGolDarah(NilaBaru);
                if (ValidasiGolDarah(NilaBaru)) break;
                cout << "[!] Golongan darah tidak valid!\n";
            }
            P.GolDarah = NilaBaru;
            SimpanPendonorKeFile(Head);
            cout << "[OK] Golongan darah berhasil diubah!\n";
            break;
        }
        case 5: {
            while (true) {
                cout << "Rhesus baru (+/-, 0=batal): ";
                getline(cin, NilaBaru);
                if (NilaBaru == "0") { cout << "[!] Dibatalkan.\n"; return; }
                if (NilaBaru == "+" || NilaBaru == "-") break;
                cout << "[!] Rhesus harus '+' atau '-'!\n";
            }
            P.Rhesus = NilaBaru;
            SimpanPendonorKeFile(Head);
            cout << "[OK] Rhesus berhasil diubah!\n";
            break;
        }
        case 6: {
            while (true) {
                cout << "Alamat baru (0=batal): ";
                getline(cin, NilaBaru);
                if (NilaBaru == "0") { cout << "[!] Dibatalkan.\n"; return; }
                if (!NilaBaru.empty()) break;
                cout << "[!] Alamat tidak boleh kosong!\n";
            }
            P.Alamat = NilaBaru;
            SimpanPendonorKeFile(Head);
            cout << "[OK] Alamat berhasil diubah!\n";
            break;
        }
        case 7: {
            while (true) {
                cout << "No. Telepon baru (11-13 digit, 0=batal): ";
                getline(cin, NilaBaru);
                if (NilaBaru == "0") { cout << "[!] Dibatalkan.\n"; return; }
                if (NilaBaru.length() < 11 || NilaBaru.length() > 13) {
                    cout << "[!] Nomor telepon harus 11-13 digit!\n"; continue;
                }
                bool SemuaAngka = true;
                for (char c : NilaBaru) if (!isdigit(c)) { SemuaAngka = false; break; }
                if (!SemuaAngka) { cout << "[!] Nomor telepon harus berupa angka!\n"; continue; }
                break;
            }
            P.NomorTelepon = NilaBaru;
            SimpanPendonorKeFile(Head);
            cout << "[OK] No. Telepon berhasil diubah!\n";
            break;
        }
        case 8: {
            while (true) {
                cout << "Password baru (min. 5 karakter, 0=batal): ";
                getline(cin, NilaBaru);
                if (NilaBaru == "0") { cout << "[!] Dibatalkan.\n"; return; }
                if (NilaBaru.empty()) { cout << "[!] Password tidak boleh kosong!\n"; continue; }
                if (NilaBaru.length() < 5) { cout << "[!] Password minimal 5 karakter!\n"; continue; }
                break;
            }
            ifstream FileIn("data/Users.csv");
            ofstream FileTmp("data/Users_tmp.csv");
            string Line; bool Pertama = true;
            while (getline(FileIn, Line)) {
                if (Line.empty()) continue;
                istringstream Iss(Line);
                string U, Pw, R;
                getline(Iss, U, ',');
                getline(Iss, Pw, ',');
                getline(Iss, R);
                if (!Pertama) FileTmp << "\n";
                if (U == P.Username) FileTmp << U << "," << NilaBaru << "," << R;
                else FileTmp << U << "," << Pw << "," << R;
                Pertama = false;
            }
            FileIn.close(); FileTmp.close();
            remove("data/Users.csv");
            rename("data/Users_tmp.csv", "data/Users.csv");
            cout << "[OK] Password berhasil diubah!\n";
            break;
        }
        default:
            cout << "[!] Pilihan tidak valid!\n";
            break;
    }
}

void AdminUpdateRiwayat(NodePendonor*& Head) {
    Utils::bersihkanLayar();
    cout << "=== UPDATE RIWAYAT PENDONOR ===\n";

    if (Head == nullptr) {
        cout << "[!] Belum ada data.\n";
        return;
    }

    TampilSemuaPendonor(Head);
    cout << "(ketik '0' untuk batal di langkah manapun)\n";

    string Username, TglBaru;
    cin.ignore();

    while (true) {
        cout << "\nInput Username pendonor: ";
        getline(cin, Username);
        if (Username == "0") { cout << "[!] Dibatalkan.\n"; return; }
        if (CariPendonorByUsername(Head, Username) != nullptr) break;
        cout << "[!] Username tidak ditemukan! Coba lagi.\n";
    }

    while (true) {
        cout << "Input Tanggal Baru (YYYY-MM-DD): ";
        getline(cin, TglBaru);
        if (TglBaru == "0") { cout << "[!] Dibatalkan.\n"; return; }
        if (ValidasiTanggal(TglBaru)) break;
        cout << "[!] Tanggal tidak valid! Pastikan format YYYY-MM-DD dan bukan tanggal masa depan.\n";
    }

    string Lokasi;
    while (true) {
        cout << "Input Lokasi donor (contoh: PMI_Pusat) : ";
        getline(cin, Lokasi);
        if (Lokasi == "0") { cout << "[!] Dibatalkan.\n"; return; }
        if (!Lokasi.empty()) break;
        cout << "[!] Lokasi tidak boleh kosong!\n";
    }

    string Keterangan;
    while (true) {
        cout << "Keterangan (Sukses/Ditolak) : ";
        getline(cin, Keterangan);
        if (Keterangan == "Sukses" || Keterangan == "Ditolak") break;
        cout << "[!] Keterangan harus 'Sukses' atau 'Ditolak'!\n";
    }

    RiwayatDonor R;
    R.username      = Username;
    R.TanggalDonor  = TglBaru;
    R.Lokasi        = Lokasi;
    R.JumlahKantong = 1;
    R.Keterangan    = Keterangan;
    TambahRiwayat(R);

    cout << "\n[OK] Riwayat berhasil diperbarui!\n";
}

void AdminHapusPendonor(NodePendonor*& Head) {
    Utils::bersihkanLayar();
    cout << "=== HAPUS PENDONOR ===\n";

    if (Head == nullptr) {
        cout << "[!] Belum ada data.\n";
        return;
    }

    TampilSemuaPendonor(Head);

    int TotalData = 0;
    NodePendonor* Curr = Head;
    while (Curr != nullptr) { TotalData++; Curr = Curr->Next; }

    cout << "(input '0' untuk batal)\n";
    int NomorPilih;
    cin.ignore();
    while (true) {
        cout << "\nInput nomor yang ingin dihapus: ";
        if (!(cin >> NomorPilih)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "[!] Input tidak valid!\n";
            continue;
        }
        if (NomorPilih == 0) { cout << "[!] Dibatalkan.\n"; return; }
        if (NomorPilih >= 1 && NomorPilih <= TotalData) break;
        cout << "[!] Nomor tidak valid! Masukkan 1-" << TotalData << "\n";
    }
    cin.ignore();

    string Username;
    NodePendonor* Cari = Head;
    for (int i = 1; i < NomorPilih; i++) Cari = Cari->Next;
    Username = Cari->Data.Username;

    char Konfirmasi;
    cout << "Yakin hapus '" << Username << "'? (y/n): ";
    cin >> Konfirmasi;

    if (Konfirmasi != 'y' && Konfirmasi != 'Y') {
        cout << "\n[!] Penghapusan dibatalkan.\n";
        return;
    }

    HapusPendonor(Head, Username);
    SimpanPendonorKeFile(Head);

    ifstream FileIn("data/Users.csv");
    ofstream FileTmp("data/Users_tmp.csv");
    string Line; bool Pertama = true;
    while (getline(FileIn, Line)) {
        if (Line.empty()) continue;
        istringstream Iss(Line);
        string U, P, R;
        getline(Iss, U, ',');
        getline(Iss, P, ',');
        getline(Iss, R);
        if (U != Username) {
            if (!Pertama) FileTmp << "\n";
            FileTmp << U << "," << P << "," << R;
            Pertama = false;
        }
    }
    FileIn.close(); FileTmp.close();
    remove("data/Users.csv");
    rename("data/Users_tmp.csv", "data/Users.csv");

    ifstream FileRIn("data/Riwayat.csv");
    ofstream FileRTmp("data/Riwayat_tmp.csv");
    Pertama = true;
    while (getline(FileRIn, Line)) {
        if (Line.empty()) continue;
        istringstream Iss(Line);
        string UName;
        getline(Iss, UName, ',');
        string Sisa; getline(Iss, Sisa);
        if (UName != Username) {
            if (!Pertama) FileRTmp << "\n";
            FileRTmp << UName << "," << Sisa;
            Pertama = false;
        }
    }
    FileRIn.close(); FileRTmp.close();
    remove("data/Riwayat.csv");
    rename("data/Riwayat_tmp.csv", "data/Riwayat.csv");

    cout << "\n[OK] Data berhasil dihapus!\n";
}

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
        cout << "Input Gol. Darah (A/B/AB/O): ";
        getline(cin, GolDarah);
        GolDarah = NormalisasiGolDarah(GolDarah);
        if (GolDarah == "0") { cout << "[!] Dibatalkan.\n"; return; }
        if (!ValidasiGolDarah(GolDarah)) { cout << "[!] Golongan darah tidak valid!\n"; continue; }

        cout << "Input Jumlah Kantong: ";
        if (!(cin >> Jumlah)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "[!] Input tidak valid!\n";
            continue;
        }
        cin.ignore();
        if (Jumlah < 0) { cout << "[!] Jumlah harus > 0!\n"; continue; }
        break;
    }

    TambahStok(Stok, GolDarah, Jumlah);
    SimpanStokKeFile(Stok);
    cout << "\n[OK] Stok darah berhasil diperbarui!\n";
    cout << "Stok " << GolDarah << " sekarang: " << GetStok(Stok, GolDarah) << " kantong\n";
}

void AdminKurangiStok(StokDarah& Stok) {
    Utils::bersihkanLayar();
    cout << "=== KURANGI STOK DARAH ===\n";
    cout << "Stok saat ini: A=" << Stok.StokA << " B=" << Stok.StokB
         << " AB=" << Stok.StokAB << " O=" << Stok.StokO << "\n";
    cout << "(ketik '0' di Gol. Darah untuk batal)\n\n";

    string GolDarah;
    int Jumlah;
    cin.ignore();

    while (true) {
        cout << "Input Gol. Darah (A/B/AB/O): ";
        getline(cin, GolDarah);
        GolDarah = NormalisasiGolDarah(GolDarah);
        if (GolDarah == "0") { cout << "[!] Dibatalkan.\n"; return; }
        if (ValidasiGolDarah(GolDarah)) break;
        cout << "[!] Golongan darah tidak valid!\n";
    }

    cout << "Input Jumlah Diminta: ";
    if (!(cin >> Jumlah)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "[!] Input tidak valid!\n";
        return;
    }

    if (Jumlah <= 0) { cout << "\n[!] Jumlah harus > 0!\n"; return; }

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

void AdminVerifikasiDonor(NodePendonor*& Head) {
    Utils::bersihkanLayar();
    cout << "=== VERIFIKASI DATA ===\n";

    if (Head == nullptr) {
        cout << "[!] Belum ada data.\n";
        return;
    }

    TampilSemuaPendonor(Head);
    cout << "(ketik '0' untuk batal)\n";

    string Username;
    double Sistolik, Diastolik, Hemoglobin;
    cin.ignore();

    while (true) {
        cout << "\nInput Username pendonor: ";
        getline(cin, Username);
        if (Username == "0") { cout << "[!] Dibatalkan.\n"; return; }
        if (CariPendonorByUsername(Head, Username) != nullptr) break;
        cout << "[!] Username tidak ditemukan di data pendonor!\n";
        cout << "    (Akun admin tidak bisa diverifikasi)\n";
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

    string TglTerakhir = AmbilTglTerakhir(Username);
    if (TglTerakhir == "-") {
        cout << "[OK] Output Status: LAYAK DONOR!\n";
        cout << "     Belum ada riwayat donor sebelumnya.\n";
        return;
    }

    int Selisih = Utils::hitungSelisihHari(TglTerakhir);
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