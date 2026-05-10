#include "../include/Utils.h"
#include "../include/Models.h"
#include "../include/UserPage.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <iomanip>
#include <string>
using namespace std;

void menuUser(User& UserAktif, NodePendonor*& Head) {
    int pilihan;
    bool lanjut = true;

    while (lanjut) {
        Utils::bersihkanLayar();
        cout << "\n========================================\n";
        cout << "         SISTEM DONOR DARAH\n";
        cout << "              MENU USER\n";
        cout << "========================================\n";
        cout << " 1. Profil\n";
        cout << " 2. Cek Jadwal\n";
        cout << " 3. Riwayat Donor\n";
        cout << " 4. Informasi Edukasi\n";
        cout << " 5. Hapus Akun\n";
        cout << " 6. Logout\n";
        cout << "----------------------------------------\n";
        cout << " Pilih menu (1-6): ";

        if (!(cin >> pilihan)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore(1000, '\n');

        switch (pilihan) {
            case 1:
                Profil(UserAktif, Head);
                break;
            case 2:
                cekJadwal(UserAktif);
                break;
            case 3:
                riwayatDonor(UserAktif);
                break;
            case 4:
                edukasi();
                break;
            case 5:
                HapusAkun(UserAktif, Head);
                lanjut = false;
                break;
            case 6:
                cout << "\nLogout berhasil. Sampai jumpa!\n";
                lanjut = false;
                return;
            default:
                cout << "[!] Pilihan tidak ada. Silakan masukkan angka 1-6." << endl;
                Utils::tekanEnter();
                break;
        }
    }
}

void Profil(User& UserAktif, NodePendonor*& Head) {
    ifstream file("data/Pendonor.csv");
    Pendonor p;
    bool ditemukan = false;

    while (getline(file, p.Username, ',')) {
        getline(file, p.Nik, ',');
        getline(file, p.Nama, ',');
        getline(file, p.GolDarah, ',');
        getline(file, p.Rhesus, ',');
        getline(file, p.Alamat, ',');
        getline(file, p.NomorTelepon);

        if (p.Username == UserAktif.Username) {
            ditemukan = true;

            cout << "\n========================================\n";
            cout << "              DATA DIRI\n";
            cout << "========================================\n";
            cout << left << setw(20) << "Username"       << ": " << p.Username << endl;
            cout << left << setw(20) << "NIK"            << ": " << p.Nik << endl;
            cout << left << setw(20) << "Nama"           << ": " << p.Nama << endl;
            cout << left << setw(20) << "Golongan Darah" << ": " << p.GolDarah << endl;
            cout << left << setw(20) << "Rhesus"         << ": " << p.Rhesus << endl;
            cout << left << setw(20) << "Alamat"         << ": " << p.Alamat << endl;
            cout << left << setw(20) << "No HP"          << ": " << p.NomorTelepon << endl;
            cout << "========================================\n";

            break;
        }
    }

    file.close();

    if (ditemukan) {
        int pilih;
        cout << "\n1. Edit Data";
        cout << "\n0. Kembali";
        cout << "\nPilih : ";

        if (!(cin >> pilih)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "[!] Input tidak valid!\n";
            Utils::tekanEnter();
            return;
        }
        cin.ignore();

        if (pilih == 1) {
            EditProfil(UserAktif, Head);
        } else if (pilih == 0) {
            return;
        } else {
            cout << "[!] Pilihan tidak valid!\n";
            Utils::tekanEnter();
        }
        return;
    }

    if (!ditemukan) {
        cout << "\n[!] Data belum ada, silakan isi.\n";
        cout << "(ketik '0' di field manapun untuk batal)\n\n";

        p.Username = UserAktif.Username;

        while (true) {
            cout << "NIK (16 digit) : ";
            getline(cin, p.Nik);
            if (p.Nik == "0") { cout << "[!] Dibatalkan.\n"; Utils::tekanEnter(); return; }
            if (p.Nik.length() != 16) { cout << "[!] NIK harus 16 digit!\n"; continue; }
            bool SemuaAngka = true;
            for (char c : p.Nik) if (!isdigit(c)) { SemuaAngka = false; break; }
            if (!SemuaAngka) { cout << "[!] NIK harus berupa angka!\n"; continue; }
            break;
        }

        while (true) {
            cout << "Nama Lengkap   : ";
            getline(cin, p.Nama);
            if (p.Nama == "0") { cout << "[!] Dibatalkan.\n"; Utils::tekanEnter(); return; }
            if (!p.Nama.empty()) break;
            cout << "[!] Nama tidak boleh kosong!\n";
        }

        while (true) {
            cout << "Gol. Darah (A/B/AB/O) : ";
            getline(cin, p.GolDarah);
            if (p.GolDarah == "0") { cout << "[!] Dibatalkan.\n"; Utils::tekanEnter(); return; }
            p.GolDarah = NormalisasiGolDarah(p.GolDarah);
            if (ValidasiGolDarah(p.GolDarah)) break;
            cout << "[!] Golongan darah tidak valid!\n";
        }

        while (true) {
            cout << "Rhesus (+/-) : ";
            getline(cin, p.Rhesus);
            if (p.Rhesus == "0") { cout << "[!] Dibatalkan.\n"; Utils::tekanEnter(); return; }
            if (p.Rhesus == "+" || p.Rhesus == "-") break;
            cout << "[!] Rhesus harus '+' atau '-'!\n";
        }

        while (true) {
            cout << "Alamat : ";
            getline(cin, p.Alamat);
            if (p.Alamat == "0") { cout << "[!] Dibatalkan.\n"; Utils::tekanEnter(); return; }
            if (!p.Alamat.empty()) break;
            cout << "[!] Alamat tidak boleh kosong!\n";
        }

        while (true) {
            cout << "Nomor Telepon (11-13 digit) : ";
            getline(cin, p.NomorTelepon);
            if (p.NomorTelepon == "0") { cout << "[!] Dibatalkan.\n"; Utils::tekanEnter(); return; }
            if (p.NomorTelepon.length() < 11 || p.NomorTelepon.length() > 13) {
                cout << "[!] Nomor telepon harus 11-13 digit!\n"; continue;
            }
            bool SemuaAngka = true;
            for (char c : p.NomorTelepon) if (!isdigit(c)) { SemuaAngka = false; break; }
            if (!SemuaAngka) { cout << "[!] Nomor telepon harus berupa angka!\n"; continue; }
            break;
        }

        char pernahDonor;
        while (true) {
            cout << "\nPernah donor sebelumnya? (y/n): ";
            cin >> pernahDonor;
            cin.ignore(1000, '\n');
            if (pernahDonor == 'y' || pernahDonor == 'Y' ||
                pernahDonor == 'n' || pernahDonor == 'N') break;
            cout << "[!] Input harus y atau n!\n";
        }

        if (pernahDonor == 'y' || pernahDonor == 'Y') {
            string tanggal;

            while (true) {
                cout << "Tanggal donor terakhir (YYYY-MM-DD): ";
                getline(cin, tanggal);
                if (tanggal == "0") { cout << "[!] Dibatalkan.\n"; Utils::tekanEnter(); return; }
                if (ValidasiTanggal(tanggal)) break;
                cout << "[!] Format tanggal salah!\n";
            }

            bool RiwayatKosong = true;
            ifstream CekRiwayat("data/Riwayat.csv");
            if (CekRiwayat.is_open()) {
                CekRiwayat.seekg(0, ios::end);
                RiwayatKosong = (CekRiwayat.tellg() == 0);
                CekRiwayat.close();
            }
            ofstream fileRiwayat("data/Riwayat.csv", ios::app);
            if (fileRiwayat.is_open()) {
                if (!RiwayatKosong) fileRiwayat << "\n";
                fileRiwayat << p.Username << ","
                            << tanggal << ","
                            << "PMI,1,Sukses";
                fileRiwayat.close();
            }
        }

        bool PendonorKosong = true;
        ifstream CekPendonor("data/Pendonor.csv");
        if (CekPendonor.is_open()) {
            CekPendonor.seekg(0, ios::end);
            PendonorKosong = (CekPendonor.tellg() == 0);
            CekPendonor.close();
        }
        ofstream out("data/Pendonor.csv", ios::app);
        if (out.is_open()) {
            if (!PendonorKosong) out << "\n";
            out << p.Username << ","
                << p.Nik << ","
                << p.Nama << ","
                << p.GolDarah << ","
                << p.Rhesus << ","
                << p.Alamat << ","
                << p.NomorTelepon;
            out.close();
        }

        TambahPendonor(Head, p);

        cout << "\n[OK] Data berhasil disimpan!\n";
    }

    Utils::tekanEnter();
}

void EditProfil(User& UserAktif, NodePendonor*& Head) {
    Utils::bersihkanLayar();
    string UsernameLamaSession = UserAktif.Username;
    ifstream File("data/Pendonor.csv");

    if (!File.is_open()) {
        cout << "[!] File tidak ditemukan!\n";
        return;
    }

    Pendonor p;
    bool Ditemukan = false;

    while (getline(File, p.Username, ',')) {
        getline(File, p.Nik, ',');
        getline(File, p.Nama, ',');
        getline(File, p.GolDarah, ',');
        getline(File, p.Rhesus, ',');
        getline(File, p.Alamat, ',');
        getline(File, p.NomorTelepon);

        if (p.Username == UserAktif.Username) {
            Ditemukan = true;
            break;
        }
    }

    File.close();

    if (!Ditemukan) {
        cout << "[!] Data tidak ditemukan!\n";
        return;
    }

    cout << "\n========================================\n";
    cout << "              DATA DIRI\n";
    cout << "========================================\n";
    cout << left << setw(20) << "1. Username"       << ": " << p.Username << "\n";
    cout << left << setw(20) << "2. NIK"            << ": " << p.Nik << "\n";
    cout << left << setw(20) << "3. Nama"           << ": " << p.Nama << "\n";
    cout << left << setw(20) << "4. Golongan Darah" << ": " << p.GolDarah << "\n";
    cout << left << setw(20) << "5. Rhesus"         << ": " << p.Rhesus << "\n";
    cout << left << setw(20) << "6. Alamat"         << ": " << p.Alamat << "\n";
    cout << left << setw(20) << "7. No HP"          << ": " << p.NomorTelepon << "\n";
    cout << "========================================\n";
    cout << "\nPilih data yang ingin diedit (0 untuk batal): ";

    int Pilihan;
    if (!(cin >> Pilihan)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "[!] Input tidak valid!\n";
        return;
    }
    cin.ignore();

    if (Pilihan == 0) { cout << "[!] Dibatalkan.\n"; return; }

    string NilaiBaru;

    switch (Pilihan) {
        case 1: {
            while (true) {
                cout << "Username baru (0=batal): ";
                getline(cin, NilaiBaru);
                if (NilaiBaru == "0") { cout << "[!] Dibatalkan.\n"; return; }
                if (NilaiBaru.empty()) { cout << "[!] Username tidak boleh kosong!\n"; continue; }
                if (CekUsernameAdaDiFile(NilaiBaru)) { cout << "[!] Username sudah digunakan!\n"; continue; }
                break;
            }

            string UsernameLama = p.Username;
            p.Username = NilaiBaru;
            UserAktif.Username = NilaiBaru;

            ifstream UserIn("data/Users.csv");
            ofstream UserTmp("data/Users_tmp.csv");
            string Line; bool Pertama = true;
            while (getline(UserIn, Line)) {
                if (Line.empty()) continue;
                istringstream Iss(Line);
                string U, Pw, Role;
                getline(Iss, U, ','); getline(Iss, Pw, ','); getline(Iss, Role);
                if (!Pertama) UserTmp << "\n";
                if (U == UsernameLama) UserTmp << NilaiBaru << "," << Pw << "," << Role;
                else UserTmp << U << "," << Pw << "," << Role;
                Pertama = false;
            }
            UserIn.close(); UserTmp.close();
            remove("data/Users.csv");
            rename("data/Users_tmp.csv", "data/Users.csv");

            ifstream RiwayatIn("data/Riwayat.csv");
            ofstream RiwayatTmp("data/Riwayat_tmp.csv");
            Pertama = true;
            while (getline(RiwayatIn, Line)) {
                if (Line.empty()) continue;
                istringstream Iss(Line);
                string U; getline(Iss, U, ',');
                string Sisa; getline(Iss, Sisa);
                if (!Pertama) RiwayatTmp << "\n";
                if (U == UsernameLama) RiwayatTmp << NilaiBaru << "," << Sisa;
                else RiwayatTmp << U << "," << Sisa;
                Pertama = false;
            }
            RiwayatIn.close(); RiwayatTmp.close();
            remove("data/Riwayat.csv");
            rename("data/Riwayat_tmp.csv", "data/Riwayat.csv");

            cout << "[OK] Username berhasil diubah!\n";
            break;
        }
        case 2: {
            while (true) {
                cout << "NIK baru (16 digit, 0=batal): ";
                getline(cin, NilaiBaru);
                if (NilaiBaru == "0") { cout << "[!] Dibatalkan.\n"; return; }
                if (NilaiBaru.length() != 16) { cout << "[!] NIK harus 16 digit!\n"; continue; }
                bool SemuaAngka = true;
                for (char c : NilaiBaru) if (!isdigit(c)) { SemuaAngka = false; break; }
                if (!SemuaAngka) { cout << "[!] NIK harus berupa angka!\n"; continue; }
                break;
            }
            p.Nik = NilaiBaru;
            cout << "[OK] NIK berhasil diubah!\n";
            break;
        }
        case 3: {
            while (true) {
                cout << "Nama baru (0=batal): ";
                getline(cin, NilaiBaru);
                if (NilaiBaru == "0") { cout << "[!] Dibatalkan.\n"; return; }
                if (!NilaiBaru.empty()) break;
                cout << "[!] Nama tidak boleh kosong!\n";
            }
            p.Nama = NilaiBaru;
            cout << "[OK] Nama berhasil diubah!\n";
            break;
        }
        case 4: {
            while (true) {
                cout << "Gol. Darah baru (A/B/AB/O, 0=batal): ";
                getline(cin, NilaiBaru);
                if (NilaiBaru == "0") { cout << "[!] Dibatalkan.\n"; return; }
                NilaiBaru = NormalisasiGolDarah(NilaiBaru);
                if (ValidasiGolDarah(NilaiBaru)) break;
                cout << "[!] Golongan darah tidak valid!\n";
            }
            p.GolDarah = NilaiBaru;
            cout << "[OK] Golongan darah berhasil diubah!\n";
            break;
        }
        case 5: {
            while (true) {
                cout << "Rhesus baru (+/-, 0=batal): ";
                getline(cin, NilaiBaru);
                if (NilaiBaru == "0") { cout << "[!] Dibatalkan.\n"; return; }
                if (NilaiBaru == "+" || NilaiBaru == "-") break;
                cout << "[!] Rhesus harus '+' atau '-'\n";
            }
            p.Rhesus = NilaiBaru;
            cout << "[OK] Rhesus berhasil diubah!\n";
            break;
        }
        case 6: {
            while (true) {
                cout << "Alamat baru (0=batal): ";
                getline(cin, NilaiBaru);
                if (NilaiBaru == "0") { cout << "[!] Dibatalkan.\n"; return; }
                if (!NilaiBaru.empty()) break;
                cout << "[!] Alamat tidak boleh kosong!\n";
            }
            p.Alamat = NilaiBaru;
            cout << "[OK] Alamat berhasil diubah!\n";
            break;
        }
        case 7: {
            while (true) {
                cout << "No. Telepon baru (11-13 digit, 0=batal): ";
                getline(cin, NilaiBaru);
                if (NilaiBaru == "0") { cout << "[!] Dibatalkan.\n"; return; }
                if (NilaiBaru.length() < 11 || NilaiBaru.length() > 13) {
                    cout << "[!] Nomor telepon harus 11-13 digit!\n"; continue;
                }
                bool SemuaAngka = true;
                for (char c : NilaiBaru) if (!isdigit(c)) { SemuaAngka = false; break; }
                if (!SemuaAngka) { cout << "[!] Nomor telepon harus berupa angka!\n"; continue; }
                break;
            }
            p.NomorTelepon = NilaiBaru;
            cout << "[OK] Nomor telepon berhasil diubah!\n";
            break;
        }
        default:
            cout << "[!] Pilihan tidak valid!\n";
            return;
    }

    ifstream FileIn("data/Pendonor.csv");
    ofstream FileTmp("data/Pendonor_tmp.csv");
    string Line; bool Pertama = true;
    while (getline(FileIn, Line)) {
        if (Line.empty()) continue;
        istringstream Iss(Line);
        string Username; getline(Iss, Username, ',');
        if (!Pertama) FileTmp << "\n";
        if (Username == UsernameLamaSession) {
            FileTmp << p.Username << ","
                    << p.Nik << ","
                    << p.Nama << ","
                    << p.GolDarah << ","
                    << p.Rhesus << ","
                    << p.Alamat << ","
                    << p.NomorTelepon;
        } else {
            FileTmp << Line;
        }
        Pertama = false;
    }
    FileIn.close(); FileTmp.close();
    remove("data/Pendonor.csv");
    rename("data/Pendonor_tmp.csv", "data/Pendonor.csv");

    cout << "[OK] Data berhasil diperbarui!\n";

    NodePendonor* Node = CariPendonorByUsername(Head, UsernameLamaSession);
    if (Node != nullptr) Node->Data = p;
}

void cekJadwal(const User& UserAktif) {
    string tglTerakhir = AmbilTglTerakhir(UserAktif.Username);

    cout << "\n========================================\n";
    cout << "            CEK JADWAL DONOR\n";
    cout << "========================================\n";

    cout << left;

    if (tglTerakhir == "-") {
        cout << setw(20) << "Status"     << ": BELUM PERNAH DONOR\n";
        cout << setw(20) << "Keterangan" << ": Anda bisa langsung donor\n";
        cout << "----------------------------------------\n";
        Utils::tekanEnter();
        return;
    }

    int selisih = Utils::hitungSelisihHari(tglTerakhir);
    int batas = 90;
    int sisa = batas - selisih;

    cout << setw(20) << "Donor terakhir" << ": " << tglTerakhir << endl;
    cout << setw(20) << "Jeda hari"      << ": " << selisih << " hari\n";
    cout << "----------------------------------------\n";

    if (selisih >= batas) {
        cout << setw(20) << "Status"     << ": SUDAH BOLEH DONOR\n";
        cout << setw(20) << "Keterangan" << ": Silakan donor kembali\n";
    } else {
        cout << setw(20) << "Status"     << ": BELUM BOLEH DONOR\n";
        cout << setw(20) << "Sisa waktu" << ": " << sisa << " hari lagi\n";
    }

    cout << "========================================\n";
    Utils::tekanEnter();
}

void riwayatDonor(const User& UserAktif) {
    ifstream file("data/Riwayat.csv");

    if (!file.is_open()) {
        cout << "[!] File riwayat tidak ditemukan!\n";
        Utils::tekanEnter();
        return;
    }

    string username, tanggal, lokasi, jumlahStr, keterangan;
    bool ditemukan = false;
    int no = 1;

    cout << "\n===================================================================\n";
    cout << "|                       RIWAYAT DONOR ANDA                       |\n";
    cout << "===================================================================\n";
    cout << "| No | Tanggal    | Lokasi               | Jumlah | Status        |\n";
    cout << "+----+------------+----------------------+--------+---------------+\n";

    while (getline(file, username, ',')) {
        getline(file, tanggal, ',');
        getline(file, lokasi, ',');
        getline(file, jumlahStr, ',');
        getline(file, keterangan);

        if (username == UserAktif.Username) {
            ditemukan = true;
            cout << "| "
                 << setw(2) << no++ << " | "
                 << setw(10) << tanggal << " | "
                 << setw(20) << left << lokasi << " | "
                 << setw(6) << (jumlahStr + "x") << " | "
                 << setw(13) << left << keterangan << " |\n";
        }
    }

    cout << "+----+------------+----------------------+--------+---------------+\n";
    file.close();

    if (!ditemukan) cout << "\n[!] Belum ada riwayat donor.\n";

    Utils::tekanEnter();
}

void edukasi() {
    int pilih;
    do {
        Utils::bersihkanLayar();
        cout << "\n========================================\n";
        cout << "          INFORMASI EDUKASI\n";
        cout << "              DONOR DARAH\n";
        cout << "========================================\n";
        cout << " 1. Syarat Donor Darah\n";
        cout << " 2. Manfaat Donor Darah\n";
        cout << " 3. Kondisi Tidak Boleh Donor\n";
        cout << " 4. Tips Sebelum Donor\n";
        cout << " 0. Kembali\n";
        cout << "----------------------------------------\n";
        cout << " Pilih menu : ";

        if (!(cin >> pilih)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\n[!] Input harus berupa angka!\n";
            Utils::tekanEnter();
            continue;
        }
        cin.ignore(1000, '\n');

        switch (pilih) {
            case 1:
                Utils::bersihkanLayar();
                cout << "\n========================================\n";
                cout << "           SYARAT DONOR DARAH\n";
                cout << "========================================\n";
                cout << " - Usia 17 - 60 tahun\n";
                cout << " - Berat badan minimal 45 kg\n";
                cout << " - Tekanan darah normal\n";
                cout << "   (90/60 - 140/90 mmHg)\n";
                cout << " - Hemoglobin minimal 12.5 g/dL\n";
                cout << " - Dalam kondisi sehat\n";
                cout << " - Tidak sedang konsumsi obat tertentu\n";
                cout << " - Jarak donor minimal 2-3 bulan\n";
                cout << "========================================\n";
                Utils::tekanEnter();
                break;
            case 2:
                Utils::bersihkanLayar();
                cout << "\n========================================\n";
                cout << "           MANFAAT DONOR DARAH\n";
                cout << "========================================\n";
                cout << " - Membantu menyelamatkan nyawa\n";
                cout << " - Menjaga kesehatan jantung\n";
                cout << " - Merangsang pembentukan sel darah baru\n";
                cout << " - Mengurangi kelebihan zat besi\n";
                cout << " - Mendapat pemeriksaan kesehatan gratis\n";
                cout << " - Membakar kalori dalam tubuh\n";
                cout << "========================================\n";
                Utils::tekanEnter();
                break;
            case 3:
                Utils::bersihkanLayar();
                cout << "\n========================================\n";
                cout << "        KONDISI TIDAK BOLEH DONOR\n";
                cout << "========================================\n";
                cout << " - Sedang demam atau sakit\n";
                cout << " - Tekanan darah tidak normal\n";
                cout << " - Hemoglobin rendah\n";
                cout << " - Baru menjalani operasi\n";
                cout << " - Memiliki penyakit menular\n";
                cout << "   (HIV, Hepatitis, dll)\n";
                cout << " - Sedang hamil atau menyusui\n";
                cout << "========================================\n";
                Utils::tekanEnter();
                break;
            case 4:
                Utils::bersihkanLayar();
                cout << "\n========================================\n";
                cout << "            TIPS SEBELUM DONOR\n";
                cout << "========================================\n";
                cout << " - Tidur cukup 6 - 8 jam\n";
                cout << " - Konsumsi makanan bergizi\n";
                cout << " - Perbanyak minum air putih\n";
                cout << " - Hindari alkohol dan rokok\n";
                cout << " - Jangan donor saat perut kosong\n";
                cout << " - Hindari begadang sebelum donor\n";
                cout << "========================================\n";
                Utils::tekanEnter();
                break;
            case 0:
                return;
            default:
                cout << "\n[!] Pilihan tidak tersedia!\n";
                Utils::tekanEnter();
                break;
        }
    } while (pilih != 0);
}

void HapusAkun(const User& UserAktif, NodePendonor*& Head) {
    Utils::bersihkanLayar();

    cout << "\n========================================\n";
    cout << "             HAPUS AKUN\n";
    cout << "========================================\n";
    cout << "Username : " << UserAktif.Username << endl;
    cout << "\nPERINGATAN!\n";
    cout << "Semua data akun akan dihapus permanen.\n";
    cout << "Data profil dan riwayat donor juga ikut terhapus.\n";
    cout << "\nKetik 'HAPUS' untuk konfirmasi: ";

    string konfirmasi;
    getline(cin, konfirmasi);

    if (konfirmasi != "HAPUS") {
        cout << "\n[!] Penghapusan akun dibatalkan.\n";
        Utils::tekanEnter();
        return;
    }

    ifstream fileUser("data/Users.csv");
    ofstream tempUser("data/Users_tmp.csv");
    string baris; bool Pertama = true;
    while (getline(fileUser, baris)) {
        if (baris.empty()) continue;
        istringstream ss(baris);
        string username; getline(ss, username, ',');
        if (username != UserAktif.Username) {
            if (!Pertama) tempUser << "\n";
            tempUser << baris;
            Pertama = false;
        }
    }
    fileUser.close(); tempUser.close();
    remove("data/Users.csv");
    rename("data/Users_tmp.csv", "data/Users.csv");

    ifstream filePendonor("data/Pendonor.csv");
    ofstream tempPendonor("data/Pendonor_tmp.csv");
    Pertama = true;
    while (getline(filePendonor, baris)) {
        if (baris.empty()) continue;
        istringstream ss(baris);
        string username; getline(ss, username, ',');
        if (username != UserAktif.Username) {
            if (!Pertama) tempPendonor << "\n";
            tempPendonor << baris;
            Pertama = false;
        }
    }
    filePendonor.close(); tempPendonor.close();
    remove("data/Pendonor.csv");
    rename("data/Pendonor_tmp.csv", "data/Pendonor.csv");

    ifstream fileRiwayat("data/Riwayat.csv");
    ofstream tempRiwayat("data/Riwayat_tmp.csv");
    Pertama = true;
    while (getline(fileRiwayat, baris)) {
        if (baris.empty()) continue;
        istringstream ss(baris);
        string username; getline(ss, username, ',');
        if (username != UserAktif.Username) {
            if (!Pertama) tempRiwayat << "\n";
            tempRiwayat << baris;
            Pertama = false;
        }
    }
    fileRiwayat.close(); tempRiwayat.close();
    remove("data/Riwayat.csv");
    rename("data/Riwayat_tmp.csv", "data/Riwayat.csv");

    HapusPendonor(Head, UserAktif.Username);

    cout << "\n[OK] Akun berhasil dihapus.\n";
    cout << "[OK] Anda telah logout.\n";
    Utils::tekanEnter();
}