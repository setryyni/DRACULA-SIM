#include "../include/Utils.h"
#include "../include/Models.h"
#include "../include/UserPage.h"
#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <string>
using namespace std;

void menuUser(const User& UserAktif) {
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
        cout << " 5. Logout\n";
        cout << "----------------------------------------\n";
        cout << " Pilih menu (1-5): ";
        
        if (!(cin >> pilihan)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (pilihan) {
            case 1:
                Profil(UserAktif);
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
                cout << "\nLogout berhasil. Sampai jumpa!\n";
                lanjut = false;
                return;
            default:
                cout << "[!] Pilihan tidak ada. Silakan masukkan angka 1-5." << endl;
                Utils::tekanEnter();
                break;
        }
    }
}

void Profil(const User& UserAktif) {
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

    if (!ditemukan) {
        cout << "\n[!] Data belum ada, silakan isi.\n";

        p.Username = UserAktif.Username;

        while (true) {    
            cout << "NIK (16 digit) : ";
                getline(cin, p.Nik);
                if (p.Nik.length() != 16) { cout << "[!] NIK harus 16 digit!\n"; continue; }
                bool SemuaAngka = true;
                for (char c : p.Nik) if (!isdigit(c)) { SemuaAngka = false; break; }
                if (!SemuaAngka) { cout << "[!] NIK harus berupa angka!\n"; continue; }
                break;
            }

        while (true) {
                cout << "Nama Lengkap   : ";
                getline(cin, p.Nama);
                if (!p.Nama.empty()) break;
                cout << "[!] Nama tidak boleh kosong!\n";
            }

        while (true) {
            cout << "Gol. Darah (A/B/AB/O) : ";
            getline(cin, p.GolDarah);
            if (ValidasiGolDarah(p.GolDarah)) break;
            cout << "[!] Golongan darah tidak valid!\n";
        }

        while (true) {
            cout << "Rhesus (+/-) : ";
            getline(cin, p.Rhesus);
            if (p.Rhesus == "+" || p.Rhesus == "-") break;
            cout << "[!] Rhesus harus '+' atau '-'!\n";
        }

        while (true) {
            cout << "Alamat : ";
            getline(cin, p.Alamat);
            if (!p.Alamat.empty()) break;
            cout << "[!] Alamat tidak boleh kosong!\n";
        }

        while (true) {
            cout << "Nomor Telepon : ";
            getline(cin, p.NomorTelepon);
            if (!p.NomorTelepon.empty()) break;
            cout << "[!] Nomor telepon tidak boleh kosong!\n";
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
            string tanggal, lokasi;

            while (true) {
                cout << "Tanggal donor terakhir (YYYY-MM-DD): ";
                getline(cin, tanggal);

                if (ValidasiTanggal(tanggal)) break;
                cout << "[!] Format tanggal salah!\n";
            }

            while (true) {
                cout << "Lokasi donor: ";
                getline(cin, lokasi);

                if (!lokasi.empty()) break;
                cout << "[!] Lokasi tidak boleh kosong!\n";
            }

            ofstream fileRiwayat("data/Riwayat.csv", ios::app);
            if (fileRiwayat.is_open()) {
                fileRiwayat << p.Username << ","
                            << tanggal << ","
                            << lokasi << ","
                            << "1,"
                            << "Data awal\n";
                fileRiwayat.close();
            }
        }

        ofstream out("data/Pendonor.csv", ios::app);
        out << p.Username << ","
            << p.Nik << ","
            << p.Nama << ","
            << p.GolDarah << ","
            << p.Rhesus << ","
            << p.Alamat << ","
            << p.NomorTelepon << endl;
        out.close();

        cout << "\n[+] Data berhasil disimpan!\n";
    }

    Utils::tekanEnter();
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

    int selisih = HitungSelisihHari(tglTerakhir);
    int batas = 90;
    int sisa = batas - selisih;

    // Informasi utama
    cout << setw(20) << "Donor terakhir" << ": " << tglTerakhir << endl;
    cout << setw(20) << "Jeda hari"      << ": " << selisih << " hari\n";
    cout << "----------------------------------------\n";

    // Status
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

    cout << "\n+----+------------+----------------------+--------+---------------+\n";
    cout << "| No | Tanggal    | Lokasi              | Jumlah | Status        |\n";
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

    if (!ditemukan) {
        cout << "\n[!] Belum ada riwayat donor.\n";
    }

    Utils::tekanEnter();
}

// informasi syarat donor
void edukasi() {
    int pilih;
    do {
        Utils::bersihkanLayar();
        cout << "\n================================\n";
        cout << "         INFORMASI EDUKASI\n";
        cout << "          DONOR DARAH\n";
        cout << "================================\n";
        cout << "1. Syarat Donor Darah\n";
        cout << "2. Manfaat Donor Darah\n";
        cout << "3. Kondisi Tidak Boleh Donor\n";
        cout << "4. Tips Sebelum Donor\n";
        cout << "0. Kembali\n";
        cout << "Pilih: ";

        if (!(cin >> pilih)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch(pilih) {
            case 1:
                cout << "\n================================\n";
                cout << "         SYARAT DONOR DARAH\n";
                cout << "================================\n";
                cout << "- Usia 17 - 60 tahun\n";
                cout << "- Berat badan minimal 45 kg\n";
                cout << "- Tekanan darah normal (90/60 - 140/90)\n";
                cout << "- Kadar hemoglobin minimal 12.5 g/dL\n";
                cout << "- Dalam kondisi sehat\n";
                cout << "- Tidak sedang mengonsumsi obat tertentu\n";
                cout << "- Jarak donor minimal 2-3 bulan\n";
                Utils::tekanEnter();
                break;

            case 2:
                cout << "\n================================\n";
                cout << "       MANFAAT DONOR DARAH\n";
                cout << "================================\n";
                cout << "- Membantu menyelamatkan nyawa orang lain\n";
                cout << "- Meningkatkan kesehatan jantung\n";
                cout << "- Merangsang produksi sel darah baru\n";
                cout << "- Mengurangi risiko kelebihan zat besi\n";
                cout << "- Mendapatkan pemeriksaan kesehatan gratis\n";
                Utils::tekanEnter();
                break;

            case 3:
                cout << "\n================================\n";
                cout << "   KONDISI TIDAK BOLEH DONOR\n";
                cout << "================================\n";
                cout << "- Sedang demam atau sakit\n";
                cout << "- Tekanan darah tidak normal\n";
                cout << "- Hb rendah\n";
                cout << "- Baru melakukan operasi\n";
                cout << "- Memiliki penyakit menular (HIV, Hepatitis)\n";
                cout << "- Sedang hamil atau menyusui\n";
                Utils::tekanEnter();
                break;

            case 4:
                cout << "\n================================\n";
                cout << "       TIPS SEBELUM DONOR\n";
                cout << "================================\n";
                cout << "- Tidur cukup minimal 6-8 jam\n";
                cout << "- Makan makanan bergizi sebelum donor\n";
                cout << "- Minum air putih yang cukup\n";
                cout << "- Hindari alkohol\n";
                cout << "- Jangan donor saat perut kosong\n";
                Utils::tekanEnter();
                break;

            case 0:
                return;

            default:
                cout << "\n[!] Pilihan tidak tersedia.\n";
                Utils::tekanEnter();
                break;
        }

    } while (pilih != 0);
}