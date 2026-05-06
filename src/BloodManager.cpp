#include "../include/BloodManager.h"
#include "../include/Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <limits>

using namespace std;

void InitList(NodePendonor*& Head) {
    Head = nullptr;
}

void TambahPendonor(NodePendonor*& Head, const Pendonor& DataBaru) {
    NodePendonor* NodeBaru = new NodePendonor();
    NodeBaru->Data = DataBaru;
    NodeBaru->Next = nullptr;

    if (Head == nullptr) {
        Head = NodeBaru;
        return;
    }
    NodePendonor* Temp = Head;
    while (Temp->Next != nullptr) Temp = Temp->Next;
    Temp->Next = NodeBaru;
}

bool HapusPendonor(NodePendonor*& Head, const string& Username) {
    if (Head == nullptr) return false;

    if (Head->Data.Username == Username) {
        NodePendonor* Temp = Head;
        Head = Head->Next;
        delete Temp;
        return true;
    }

    NodePendonor* Prev = Head;
    NodePendonor* Curr = Head->Next;
    while (Curr != nullptr) {
        if (Curr->Data.Username == Username) {
            Prev->Next = Curr->Next;
            delete Curr;
            return true;
        }
        Prev = Curr;
        Curr = Curr->Next;
    }
    return false;
}

NodePendonor* CariPendonorByUsername(NodePendonor* Head, const string& Username) {
    NodePendonor* Curr = Head;
    while (Curr != nullptr) {
        if (Curr->Data.Username == Username) return Curr;
        Curr = Curr->Next;
    }
    return nullptr;
}

int HitungPendonor(NodePendonor* Head) {
    int Count = 0;
    NodePendonor* Curr = Head;
    while (Curr != nullptr) { Count++; Curr = Curr->Next; }
    return Count;
}

bool ListKosong(NodePendonor* Head) {
    return Head == nullptr;
}

void SortByNamaAZ(NodePendonor*& Head) {
    if (Head == nullptr || Head->Next == nullptr) return;
    NodePendonor* I = Head;
    while (I != nullptr) {
        NodePendonor* MinNode = I;
        NodePendonor* J = I->Next;
        while (J != nullptr) {
            string NamaJ   = J->Data.Nama;
            string NamaMin = MinNode->Data.Nama;
            for (char& c : NamaJ)   c = tolower(c);
            for (char& c : NamaMin) c = tolower(c);
            if (NamaJ < NamaMin) MinNode = J;
            J = J->Next;
        }
        if (MinNode != I) swap(I->Data, MinNode->Data);
        I = I->Next;
    }
}

void SortByNamaZA(NodePendonor*& Head) {
    if (Head == nullptr || Head->Next == nullptr) return;
    NodePendonor* Sorted = nullptr;
    NodePendonor* Curr = Head;
    while (Curr != nullptr) {
        NodePendonor* Next = Curr->Next;

        string NamaCurr = Curr->Data.Nama;
        for (char& c : NamaCurr) c = tolower(c);

        if (Sorted == nullptr) {
            Curr->Next = Sorted;
            Sorted = Curr;
        } else {
            string NamaSorted = Sorted->Data.Nama;
            for (char& c : NamaSorted) c = tolower(c);

            if (NamaCurr > NamaSorted) {
                Curr->Next = Sorted;
                Sorted = Curr;
            } else {
                NodePendonor* Temp = Sorted;
                while (Temp->Next != nullptr) {
                    string NamaNext = Temp->Next->Data.Nama;
                    for (char& c : NamaNext) c = tolower(c);
                    if (NamaNext < NamaCurr) break;
                    Temp = Temp->Next;
                }
                Curr->Next = Temp->Next;
                Temp->Next = Curr;
            }
        }
        Curr = Next;
    }
    Head = Sorted;
}

void SortByRole(NodePendonor*& Head) {
    if (Head == nullptr || Head->Next == nullptr) return;
    bool Swapped;
    do {
        Swapped = false;
        NodePendonor* Curr = Head;
        while (Curr->Next != nullptr) {
            if (Curr->Data.GolDarah > Curr->Next->Data.GolDarah) {
                swap(Curr->Data, Curr->Next->Data);
                Swapped = true;
            }
            Curr = Curr->Next;
        }
    } while (Swapped);
}

NodePendonor* BinarySearchByNama(NodePendonor* SortedHead, const string& Nama, int Size) {
    if (SortedHead == nullptr || Size == 0) return nullptr;
    string NamaLower = Nama;
    for (char& c : NamaLower) c = tolower(c);

    NodePendonor** Arr = new NodePendonor*[Size];
    NodePendonor* Curr = SortedHead;
    for (int i = 0; i < Size; i++) {
        Arr[i] = Curr;
        Curr = Curr->Next;
    }

    int Lo = 0, Hi = Size - 1;
    NodePendonor* Hasil = nullptr;
    while (Lo <= Hi) {
        int Mid = (Lo + Hi) / 2;
        string NamaMid = Arr[Mid]->Data.Nama;
        for (char& c : NamaMid) c = tolower(c);

        if (NamaMid == NamaLower) {
            Hasil = Arr[Mid];
            break;
        } else if (NamaMid < NamaLower) {
            Lo = Mid + 1;
        } else {
            Hi = Mid - 1;
        }
    }
    delete[] Arr;
    return Hasil;
}

void MuatPendonorDariFile(NodePendonor*& Head) {
    ifstream File("data/Pendonor.csv");
    if (!File.is_open()) return;

    string Line;
    while (getline(File, Line)) {
        if (Line.empty()) continue;
        istringstream Iss(Line);
        Pendonor P;
        // Kalau salah satu field gagal dibaca, skip baris ini
        if (!getline(Iss, P.Username,     ',')) continue;
        if (!getline(Iss, P.Nik,          ',')) continue;
        if (!getline(Iss, P.Nama,         ',')) continue;
        if (!getline(Iss, P.GolDarah,     ',')) continue;
        if (!getline(Iss, P.Rhesus,       ',')) continue;
        if (!getline(Iss, P.Alamat,       ',')) continue;
        if (!getline(Iss, P.NomorTelepon))      continue;
        // Semua field valid, baru tambah ke linked list
        TambahPendonor(Head, P);
    }
    File.close();
}

void SimpanPendonorKeFile(NodePendonor* Head) {
    ofstream File("data/Pendonor.csv");
    if (!File.is_open()) return;
    NodePendonor* Curr = Head;
    while (Curr != nullptr) {
        Pendonor& P = Curr->Data;
        File << P.Username     << ","
             << P.Nik          << ","
             << P.Nama         << ","
             << P.GolDarah     << ","
             << P.Rhesus       << ","
             << P.Alamat       << ","
             << P.NomorTelepon << "\n";
        Curr = Curr->Next;
    }
    File.close();
}

StokDarah MuatStokDariFile() {
    StokDarah Stok = {0, 0, 0, 0};
    ifstream File("data/StokDarah.csv");
    if (!File.is_open()) return Stok;
    string Line;
    if (getline(File, Line) && !Line.empty()) {
        try {
            istringstream Iss(Line);
            string Val;
            getline(Iss, Val, ','); Stok.StokA  = Val.empty() ? 0 : stoi(Val);
            getline(Iss, Val, ','); Stok.StokB  = Val.empty() ? 0 : stoi(Val);
            getline(Iss, Val, ','); Stok.StokAB = Val.empty() ? 0 : stoi(Val);
            getline(Iss, Val);      Stok.StokO  = Val.empty() ? 0 : stoi(Val);
        } catch (...) {
            Stok = {0, 0, 0, 0};
        }
    }
    File.close();
    return Stok;
}

void SimpanStokKeFile(const StokDarah& Stok) {
    ofstream File("data/StokDarah.csv");
    if (!File.is_open()) return;
    File << Stok.StokA  << ","
         << Stok.StokB  << ","
         << Stok.StokAB << ","
         << Stok.StokO  << "\n";
    File.close();
}

bool ValidasiGolDarah(const string& GolDarah) {
    string Upper = GolDarah;
    for (char& c : Upper) c = toupper(c);
    return (Upper == "A" || Upper == "B" || Upper == "AB" || Upper == "O");
}

string NormalisasiGolDarah(const string& GolDarah) {
    string Upper = GolDarah;
    for (char& c : Upper) c = toupper(c);
    return Upper;
}

int GetStok(const StokDarah& Stok, const string& GolDarah) {
    if (GolDarah == "A")  return Stok.StokA;
    if (GolDarah == "B")  return Stok.StokB;
    if (GolDarah == "AB") return Stok.StokAB;
    if (GolDarah == "O")  return Stok.StokO;
    return -1;
}

bool TambahStok(StokDarah& Stok, const string& GolDarah, int Jumlah) {
    if (!ValidasiGolDarah(GolDarah) || Jumlah <= 0) return false;
    if (GolDarah == "A")  Stok.StokA  += Jumlah;
    if (GolDarah == "B")  Stok.StokB  += Jumlah;
    if (GolDarah == "AB") Stok.StokAB += Jumlah;
    if (GolDarah == "O")  Stok.StokO  += Jumlah;
    return true;
}

bool KurangiStok(StokDarah& Stok, const string& GolDarah, int Jumlah) {
    if (!ValidasiGolDarah(GolDarah) || Jumlah <= 0) return false;
    if (GetStok(Stok, GolDarah) < Jumlah) return false;
    if (GolDarah == "A")  Stok.StokA  -= Jumlah;
    if (GolDarah == "B")  Stok.StokB  -= Jumlah;
    if (GolDarah == "AB") Stok.StokAB -= Jumlah;
    if (GolDarah == "O")  Stok.StokO  -= Jumlah;
    return true;
}

bool StokKosong(const StokDarah& Stok) {
    return (Stok.StokA == 0 && Stok.StokB == 0 && Stok.StokAB == 0 && Stok.StokO == 0);
}

void TambahRiwayat(const RiwayatDonor& Riwayat) {
    ofstream File("data/Riwayat.csv", ios::app);
    if (!File.is_open()) return;
    File << Riwayat.username      << ","
         << Riwayat.TanggalDonor  << ","
         << Riwayat.Lokasi        << ","
         << Riwayat.JumlahKantong << ","
         << Riwayat.Keterangan    << "\n";
    File.close();
}

string AmbilTglTerakhir(const string& Username) {
    ifstream File("data/Riwayat.csv");
    if (!File.is_open()) return "-";

    string Line, TglTerakhir = "-";
    while (getline(File, Line)) {
        if (Line.empty()) continue;
        istringstream Iss(Line);
        string U, Tgl, Lok, Ket;
        getline(Iss, U,   ',');
        getline(Iss, Tgl, ',');
        getline(Iss, Lok, ',');
        Iss.ignore(numeric_limits<streamsize>::max(), ','); 
        getline(Iss, Ket);
        if (U == Username && Ket == "Sukses") TglTerakhir = Tgl;
    }
    File.close();
    return TglTerakhir;
}

bool ValidasiTanggal(const string& Tanggal) {
    if (Tanggal.size() != 10) return false;
    if (Tanggal[4] != '-' || Tanggal[7] != '-') return false;
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(Tanggal[i])) return false;
    }
    int Bulan = stoi(Tanggal.substr(5, 2));
    int Hari  = stoi(Tanggal.substr(8, 2));
    int Tahun = stoi(Tanggal.substr(0, 4));
    if (Bulan < 1 || Bulan > 12) return false;
    if (Hari  < 1 || Hari  > 31) return false;
    if (Tahun < 2000)             return false;

    time_t Now = time(nullptr);
    tm TmInput = {};
    TmInput.tm_year = Tahun - 1900;
    TmInput.tm_mon  = Bulan - 1;
    TmInput.tm_mday = Hari;
    time_t TInput = mktime(&TmInput);
    if (TInput > Now) return false;

    return true;
}

int HitungSelisihHari(const string& TglTerakhir) {
    return Utils::hitungSelisihHari(TglTerakhir);
}

bool CekUsernameAdaDiFile(const string& Username) {
    ifstream File("data/Users.csv");
    if (!File.is_open()) return false;
    string Line;
    while (getline(File, Line)) {
        if (Line.empty()) continue;
        istringstream Iss(Line);
        string U, P, R;
        getline(Iss, U, ',');
        getline(Iss, P, ',');
        getline(Iss, R);
        if (U == Username) {
            File.close();
            return true;
        }
    }
    File.close();
    return false;
}