#include "../include/AdminPage.h"
#include "../include/Utils.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>

using namespace std;

// =============================================
// HELPER: cetak tabel pendonor
// =============================================
static void CetakHeaderTabel() {
    cout << left
         << setw(4)  << "No"
         << setw(15) << "Username"
         << setw(20) << "Nama"
         << setw(5)  << "Gol"
         << setw(5)  << "Rhs"
         << setw(13) << "No. Telp"
         << setw(12) << "Tgl Donor"
         << setw(6)  << "Total"
         << "\n";
    cout << string(80, '-') << "\n";
}

static void CetakBarisPendonor(int No, const Pendonor& P) {
    cout << left
         << setw(4)  << No
         << setw(15) << P.Username
         << setw(20) << P.Nama
         << setw(5)  << P.GolDarah
         << setw(5)  << P.Rhesus
         << setw(13) << P.NomorTelepon
         << setw(12) << (P.TglTerakhir.empty() || P.TglTerakhir == "-" ? "-" : P.TglTerakhir)
         << setw(6)  << P.TotalDonor
         << "\n";
}

static void TampilSemuaPendonor(NodePendonor* Head) {
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
            case 1: AdminTambahAkun(Head);        break;
            case 2: AdminTampilkanAkun(Head);     break;
            case 3: AdminUpdateRiwayat(Head);     break;
            case 4: AdminHapusPendonor(Head);     break;
            case 5: AdminCekStok(Stok);           break;
            case 6: AdminUpdateStok(Stok);        break;
            case 7: AdminKurangiStok(Stok);       break;
            case 8: AdminVerifikasiDonor(Head);   break;
            case 9:
                cout << "\nSampai Jumpa!\n";
                return;
            default:
                cout << "\n[!] Output Pilihan tidak valid!\n";
                break;
        }
        Utils::tekanEnter();
    }
}

// =============================================
// 1. TAMBAH AKUN
// Flowchart: Input Username+Password+Role →
// cek username ada (LOOP) → cek role valid (LOOP) →
// simpan → output sukses → tambah lagi?
// =============================================
void AdminTambahAkun(NodePendonor*& Head) {
    bool TambahLagi = true;

    while (TambahLagi) {
        Utils::bersihkanLayar();
        cout << "=== TAMBAH AKUN ===\n";

        string Username, Password, Role;
        cin.ignore();

        // Input Username + cek sudah ada (LOOP kiri)
        while (true) {
            cout << "Username : ";
            getline(cin, Username);
            if (!CekUsernameAdaDiFile(Username)) break;
            cout << "[!] Output: Username sudah terdaftar! Coba username lain.\n";
        }

        cout << "Password : ";
        getline(cin, Password);

        // Input Role + validasi (LOOP kanan)
        while (true) {
            cout << "Role (user/admin) : ";
            getline(cin, Role);
            if (Role == "user" || Role == "admin") break;
            cout << "[!] Output: Role tidak valid! Masukkan 'user' atau 'admin'.\n";
        }

        // Simpan ke users.txt (nyambung sama Auth.cpp)
        ofstream FileUser("data/users.txt", ios::app);
        if (FileUser.is_open()) {
            FileUser << Username << " " << Password << " " << Role << "\n";
            FileUser.close();
        }

        // Kalau role = user, buat juga entry kosong di pendonor.txt
        if (Role == "user") {
            Pendonor P;
            P.Username     = Username;
            P.Nik          = "-";
            P.Nama         = Username;
            P.GolDarah     = "-";
            P.Rhesus       = "-";
            P.Alamat       = "-";
            P.NomorTelepon = "-";
            P.TglTerakhir  = "-";
            P.TotalDonor   = 0;
            TambahPendonor(Head, P);
            SimpanPendonorKeFile(Head);
        }

        cout << "\n[OK] Output: Akun berhasil ditambahkan!\n";

        // Tambah akun lagi?
        char Jawab;
        cout << "Tambah akun lagi? (y/n): ";
        cin >> Jawab;
        TambahLagi = (Jawab == 'y' || Jawab == 'Y');
    }
}

// =============================================
// 2. TAMPILKAN AKUN
// Flowchart: List kosong? → input pilihan →
// Pilihan 1: tampil semua (loop i++)
// Pilihan 2: Selection Sort A-Z
// Pilihan 3: Insertion Sort Z-A
// Pilihan 4: Bubble Sort by role
// Sub-menu search: Pilihan 1 = Linear (tidak terurut), Pilihan 2 = Binary (terurut)
// =============================================
void AdminTampilkanAkun(NodePendonor*& Head) {
    Utils::bersihkanLayar();
    cout << "=== TAMPILKAN SEMUA AKUN ===\n";

    if (ListKosong(Head)) {
        cout << "[!] Output pesan: Belum ada data.\n";
        return;
    }

    int Pilihan;
    cout << "1. Tampil Semua Akun (User/Admin)\n";
    cout << "2. Sort Nama A-Z (Selection Sort)\n";
    cout << "3. Sort Nama Z-A (Insertion Sort)\n";
    cout << "4. Sort by Role (Bubble Sort)\n";
    cout << "Input pilihan: ";
    cin >> Pilihan;

    if (Pilihan == 1) {
        // Tampil semua dengan loop i++
        Utils::bersihkanLayar();
        cout << "=== DAFTAR SEMUA AKUN ===\n";
        TampilSemuaPendonor(Head);
        cout << "\nTotal: " << HitungPendonor(Head) << " akun\n";

        // Sub-menu searching setelah tampil
        cout << "\n--- SEARCHING ---\n";
        cout << "Input pilihan:\n";
        cout << "1. Cari by Username (Linear Search - data tidak terurut)\n";
        cout << "2. Kembali\n";
        cout << "Input pilihan: ";
        int PilSearch;
        cin >> PilSearch;

        if (PilSearch == 1) {
            string Query;
            cin.ignore();
            cout << "Input Nomor/Username yang Dicari: ";
            getline(cin, Query);
            NodePendonor* Hasil = LinearSearchByUsername(Head, Query);
            if (Hasil != nullptr) {
                cout << "\n=== Output data akun ===\n";
                CetakHeaderTabel();
                CetakBarisPendonor(1, Hasil->Data);
            } else {
                cout << "\n[!] Output Data tidak ditemukan.\n";
            }
        }

    } else if (Pilihan == 2 || Pilihan == 3 || Pilihan == 4) {
        // Buat salinan untuk sorting
        NodePendonor* SortHead = nullptr;
        NodePendonor* Curr = Head;
        while (Curr != nullptr) {
            TambahPendonor(SortHead, Curr->Data);
            Curr = Curr->Next;
        }

        string JudulSort;
        if (Pilihan == 2) { SortByNamaAZ(SortHead); JudulSort = "A-Z (Selection Sort)"; }
        else if (Pilihan == 3) { SortByNamaZA(SortHead); JudulSort = "Z-A (Insertion Sort)"; }
        else { SortByRole(SortHead); JudulSort = "by Role (Bubble Sort)"; }

        Utils::bersihkanLayar();
        cout << "=== DAFTAR AKUN TERURUT " << JudulSort << " ===\n";
        TampilSemuaPendonor(SortHead);

        // Sub-menu searching setelah sort (Binary Search karena data terurut)
        cout << "\n--- SEARCHING (data terurut) ---\n";
        cout << "Input pilihan:\n";
        cout << "2. Cari by Nama (Binary Search - data terurut)\n";
        cout << "0. Kembali\n";
        cout << "Input pilihan: ";
        int PilSearch;
        cin >> PilSearch;

        if (PilSearch == 2) {
            string Query;
            cin.ignore();
            cout << "Input Nama yang Dicari: ";
            getline(cin, Query);
            int Size = HitungPendonor(SortHead);
            NodePendonor* Hasil = BinarySearchByNama(SortHead, Query, Size);
            if (Hasil != nullptr) {
                cout << "\n=== Output data akun ===\n";
                CetakHeaderTabel();
                CetakBarisPendonor(1, Hasil->Data);
            } else {
                cout << "\n[!] Output Data tidak ditemukan.\n";
            }
        }

        // Bersihkan salinan
        while (SortHead != nullptr) {
            NodePendonor* Del = SortHead;
            SortHead = SortHead->Next;
            delete Del;
        }

    } else {
        cout << "\n[!] Output Pilihan tidak valid!\n";
    }
}

// =============================================
// 3. UPDATE RIWAYAT PENDONOR
// Flowchart: List kosong? → tampil semua →
// input nomor (LOOP kalau tidak valid) →
// input tanggal (LOOP kalau tidak valid) →
// simpan → output sukses
// =============================================
void AdminUpdateRiwayat(NodePendonor*& Head) {
    Utils::bersihkanLayar();
    cout << "=== UPDATE RIWAYAT PENDONOR ===\n";

    if (ListKosong(Head)) {
        cout << "[!] Output pesan: Belum ada data.\n";
        return;
    }

    TampilSemuaPendonor(Head);

    string Username, TglBaru;
    cin.ignore();

    // Input nomor pilihan + validasi LOOP
    while (true) {
        cout << "\nInput nomor pilihan (Username): ";
        getline(cin, Username);
        if (CariPendonorByUsername(Head, Username) != nullptr) break;
        cout << "[!] Output: Nomor tidak valid!\n";
    }

    // Input tanggal + validasi LOOP
    while (true) {
        cout << "Input Tanggal Baru (YYYY-MM-DD): ";
        getline(cin, TglBaru);
        if (ValidasiTanggal(TglBaru)) break;
        cout << "[!] Output: Tanggal tidak valid! Gunakan format YYYY-MM-DD.\n";
    }

    UpdateRiwayatDonor(Head, Username, TglBaru);
    cout << "\n[OK] Output: Riwayat berhasil diperbarui!\n";
}

// =============================================
// 4. HAPUS PENDONOR
// Flowchart: List kosong? → input nomor (LOOP) →
// konfirmasi y/n → y: hapus+output sukses / n: output dibatalkan
// =============================================
void AdminHapusPendonor(NodePendonor*& Head) {
    Utils::bersihkanLayar();
    cout << "=== HAPUS PENDONOR ===\n";

    if (ListKosong(Head)) {
        cout << "[!] Output pesan: Belum ada data.\n";
        return;
    }

    string Username;
    cin.ignore();

    // Input nomor pilihan + LOOP
    while (true) {
        cout << "Input nomor pilihan (Username): ";
        getline(cin, Username);
        if (CariPendonorByUsername(Head, Username) != nullptr) break;
        cout << "[!] Output: Nomor tidak ditemukan!\n";
    }

    // Konfirmasi y/n
    char Konfirmasi;
    cout << "Yakin hapus '" << Username << "'? (y/n): ";
    cin >> Konfirmasi;

    if (Konfirmasi == 'y' || Konfirmasi == 'Y') {
        // Hapus dari pendonor.txt
        HapusPendonor(Head, Username);
        SimpanPendonorKeFile(Head);

        // Hapus dari users.txt juga (nyambung sama Auth.cpp)
        ifstream FileIn("data/users.txt");
        ofstream FileTmp("data/users_tmp.txt");
        string U, P, R;
        while (FileIn >> U >> P >> R) {
            if (U != Username) FileTmp << U << " " << P << " " << R << "\n";
        }
        FileIn.close();
        FileTmp.close();
        remove("data/users.txt");
        rename("data/users_tmp.txt", "data/users.txt");

        cout << "\n[OK] Output: Data berhasil dihapus!\n";
    } else {
        cout << "\n[!] Output: Penghapusan dibatalkan.\n";
    }
}

// =============================================
// 5. CEK STOK DARAH
// Flowchart: Stok kosong? → output kosong / output semua stok
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
    cout << left << setw(12) << "A"  << Stok.StokA  << "\n";
    cout << left << setw(12) << "B"  << Stok.StokB  << "\n";
    cout << left << setw(12) << "AB" << Stok.StokAB << "\n";
    cout << left << setw(12) << "O"  << Stok.StokO  << "\n";
    cout << string(27, '-') << "\n";
    cout << left << setw(12) << "Total" << (Stok.StokA + Stok.StokB + Stok.StokAB + Stok.StokO) << "\n";
}

// =============================================
// 6. UPDATE STOK DARAH
// Flowchart: Input Gol+Jumlah →
// Gol tidak valid → output+LOOP balik /
// Jumlah < 0 → output+LOOP balik /
// Simpan → output sukses
// =============================================
void AdminUpdateStok(StokDarah& Stok) {
    Utils::bersihkanLayar();
    cout << "=== UPDATE STOK DARAH ===\n";
    cout << "Stok saat ini: A=" << Stok.StokA << " B=" << Stok.StokB
         << " AB=" << Stok.StokAB << " O=" << Stok.StokO << "\n\n";

    string GolDarah;
    int Jumlah;
    cin.ignore();

    while (true) {
        // Input gol darah + jumlah sekaligus
        cout << "Input Gol. Darah (A/B/AB/O): ";
        getline(cin, GolDarah);

        if (!ValidasiGolDarah(GolDarah)) {
            cout << "[!] Output: Golongan darah tidak valid!\n";
            continue; // LOOP balik input
        }

        cout << "Input Jumlah Kantong: ";
        cin >> Jumlah;
        cin.ignore();

        if (Jumlah < 0) {
            cout << "[!] Output: Jumlah harus > 0!\n";
            continue; // LOOP balik input
        }

        break;
    }

    TambahStok(Stok, GolDarah, Jumlah);
    SimpanStokKeFile(Stok);
    cout << "\n[OK] Output: Stok darah berhasil diperbarui!\n";
    cout << "Stok " << GolDarah << " sekarang: " << GetStok(Stok, GolDarah) << " kantong\n";
}

// =============================================
// 7. KURANGI STOK DARAH
// Flowchart: Input Gol+Jumlah →
// Gol tidak valid → output+LOOP /
// Jumlah <= 0 → output → END /
// Stok cukup? → tidak → output tidak cukup → END /
// Kurangi → output sukses → END
// =============================================
void AdminKurangiStok(StokDarah& Stok) {
    Utils::bersihkanLayar();
    cout << "=== KURANGI STOK DARAH ===\n";
    cout << "Stok saat ini: A=" << Stok.StokA << " B=" << Stok.StokB
         << " AB=" << Stok.StokAB << " O=" << Stok.StokO << "\n\n";

    string GolDarah;
    int Jumlah;
    cin.ignore();

    // Input Gol Darah + validasi LOOP
    while (true) {
        cout << "Input Gol. Darah (A/B/AB/O): ";
        getline(cin, GolDarah);
        if (ValidasiGolDarah(GolDarah)) break;
        cout << "[!] Output: Golongan darah tidak valid!\n";
    }

    cout << "Input Jumlah Diminta: ";
    cin >> Jumlah;

    // Jumlah tidak valid → output → END (tidak loop)
    if (Jumlah <= 0) {
        cout << "\n[!] Output: Jumlah harus > 0!\n";
        return;
    }

    // Stok cukup? → tidak → output → END
    if (GetStok(Stok, GolDarah) < Jumlah) {
        cout << "\n[!] Output: Stok tidak mencukupi!\n";
        cout << "Stok " << GolDarah << " tersedia: " << GetStok(Stok, GolDarah) << " kantong\n";
        return;
    }

    KurangiStok(Stok, GolDarah, Jumlah);
    SimpanStokKeFile(Stok);
    cout << "\n[OK] Output: Stok darah berhasil dikurangi!\n";
    cout << "Stok " << GolDarah << " sekarang: " << GetStok(Stok, GolDarah) << " kantong\n";
}

// =============================================
// 8. VERIFIKASI DATA
// Flowchart: List kosong? → tampil semua →
// input nomor+sistolik+diastolik+hemoglobin →
// nomor tidak valid → LOOP /
// kondisi medis ga normal? → output tidak layak → END /
// tgl donor ada dan < 90 hari? → ya → output ga layak → END /
// output layak donor → END
// =============================================
void AdminVerifikasiDonor(NodePendonor*& Head) {
    Utils::bersihkanLayar();
    cout << "=== VERIFIKASI DATA ===\n";

    if (ListKosong(Head)) {
        cout << "[!] Output pesan: Belum ada data.\n";
        return;
    }

    TampilSemuaPendonor(Head);

    string Username;
    double Sistolik, Diastolik, Hemoglobin;
    cin.ignore();

    // Input nomor + sistolik + diastolik + hemoglobin, LOOP kalau nomor tidak valid
    while (true) {
        cout << "\nInput nomor (Username): ";
        getline(cin, Username);

        if (CariPendonorByUsername(Head, Username) == nullptr) {
            cout << "[!] Output: Nomor tidak ditemukan!\n";
            continue; // LOOP balik
        }

        cout << "Input sistolik   : ";
        cin >> Sistolik;
        cout << "Input diastolik  : ";
        cin >> Diastolik;
        cout << "Input hemoglobin : ";
        cin >> Hemoglobin;
        cin.ignore();
        break;
    }

    NodePendonor* Node = CariPendonorByUsername(Head, Username);
    cout << "\n--- Hasil Verifikasi: " << Node->Data.Nama << " ---\n";

    // Cek kondisi medis ga normal?
    bool MedisTidakNormal = !(Sistolik  >= 100 && Sistolik  <= 140 &&
                              Diastolik >= 60  && Diastolik <= 90  &&
                              Hemoglobin >= 12.5);

    if (MedisTidakNormal) {
        cout << "[X] Output: Tidak layak donor (kondisi medis tidak normal)\n";
        if (Sistolik  < 100 || Sistolik  > 140) cout << "    Sistolik tidak normal  : " << Sistolik  << " mmHg\n";
        if (Diastolik < 60  || Diastolik > 90)  cout << "    Diastolik tidak normal : " << Diastolik << " mmHg\n";
        if (Hemoglobin < 12.5)                  cout << "    Hemoglobin rendah      : " << Hemoglobin << " g/dL\n";
        return;
    }

    // Cek tgl donor ada dan < 90 hari?
    string TglTerakhir = Node->Data.TglTerakhir;
    bool TglAdaDanKurang90 = false;

    if (!TglTerakhir.empty() && TglTerakhir != "-") {
        int Selisih = HitungSelisihHari(TglTerakhir);
        if (Selisih < 90) TglAdaDanKurang90 = true;
    }

    if (TglAdaDanKurang90) {
        int Selisih  = HitungSelisihHari(TglTerakhir);
        int SisaHari = 90 - Selisih;
        cout << "[X] Output Status: Ga layak donor (" << SisaHari << " hari lagi baru boleh)\n";
        return;
    }

    cout << "[OK] Output Status: Layak donor!\n";
}