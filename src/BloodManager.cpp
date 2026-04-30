#include "../include/BloodManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>

using namespace std;

// =============================================
// LINKED LIST
// =============================================

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

NodePendonor* CariPendonorByNama(NodePendonor* Head, const string& Nama) {
    NodePendonor* Curr = Head;
    while (Curr != nullptr) {
        if (Curr->Data.Nama == Nama) return Curr;
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

// =============================================
// SORTING
// =============================================

// Selection Sort A-Z by Nama
void SortByNamaAZ(NodePendonor*& Head) {
    if (Head == nullptr || Head->Next == nullptr) return;
    NodePendonor* I = Head;
    while (I != nullptr) {
        NodePendonor* MinNode = I;
        NodePendonor* J = I->Next;
        while (J != nullptr) {
            if (J->Data.Nama < MinNode->Data.Nama) MinNode = J;
            J = J->Next;
        }
        if (MinNode != I) swap(I->Data, MinNode->Data);
        I = I->Next;
    }
}

// Insertion Sort Z-A by Nama
void SortByNamaZA(NodePendonor*& Head) {
    if (Head == nullptr || Head->Next == nullptr) return;
    NodePendonor* Sorted = nullptr;
    NodePendonor* Curr = Head;
    while (Curr != nullptr) {
        NodePendonor* Next = Curr->Next;
        if (Sorted == nullptr || Curr->Data.Nama > Sorted->Data.Nama) {
            Curr->Next = Sorted;
            Sorted = Curr;
        } else {
            NodePendonor* Temp = Sorted;
            while (Temp->Next != nullptr && Temp->Next->Data.Nama >= Curr->Data.Nama)
                Temp = Temp->Next;
            Curr->Next = Temp->Next;
            Temp->Next = Curr;
        }
        Curr = Next;
    }
    Head = Sorted;
}

// Bubble Sort by Role (admin dulu baru pendonor)
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

// =============================================
// SEARCHING
// =============================================

// Linear Search by Username (untuk data tidak terurut)
NodePendonor* LinearSearchByUsername(NodePendonor* Head, const string& Username) {
    NodePendonor* Curr = Head;
    while (Curr != nullptr) {
        if (Curr->Data.Username == Username) return Curr;
        Curr = Curr->Next;
    }
    return nullptr;
}

// Binary Search by Nama (untuk data yang sudah terurut A-Z)
// Konversi linked list ke array dulu untuk binary search
NodePendonor* BinarySearchByNama(NodePendonor* SortedHead, const string& Nama, int Size) {
    if (SortedHead == nullptr || Size == 0) return nullptr;

    // Masukkan semua pointer ke array
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
        if (Arr[Mid]->Data.Nama == Nama) {
            Hasil = Arr[Mid];
            break;
        } else if (Arr[Mid]->Data.Nama < Nama) {
            Lo = Mid + 1;
        } else {
            Hi = Mid - 1;
        }
    }
    delete[] Arr;
    return Hasil;
}

// =============================================
// FILE I/O PENDONOR
// =============================================

void MuatPendonorDariFile(NodePendonor*& Head) {
    ifstream File("data/pendonor.txt");
    if (!File.is_open()) return;

    string Line;
    while (getline(File, Line)) {
        if (Line.empty()) continue;
        istringstream Iss(Line);
        Pendonor P;
        string TotalStr;
        getline(Iss, P.Username,      '|');
        getline(Iss, P.Nik,           '|');
        getline(Iss, P.Nama,          '|');
        getline(Iss, P.GolDarah,      '|');
        getline(Iss, P.Rhesus,        '|');
        getline(Iss, P.Alamat,        '|');
        getline(Iss, P.NomorTelepon,  '|');
        getline(Iss, P.TglTerakhir,   '|');
        getline(Iss, TotalStr);
        P.TotalDonor = TotalStr.empty() ? 0 : stoi(TotalStr);
        TambahPendonor(Head, P);
    }
    File.close();
}

void SimpanPendonorKeFile(NodePendonor* Head) {
    ofstream File("data/pendonor.txt");
    if (!File.is_open()) return;
    NodePendonor* Curr = Head;
    while (Curr != nullptr) {
        Pendonor& P = Curr->Data;
        File << P.Username     << "|"
             << P.Nik          << "|"
             << P.Nama         << "|"
             << P.GolDarah     << "|"
             << P.Rhesus       << "|"
             << P.Alamat       << "|"
             << P.NomorTelepon << "|"
             << P.TglTerakhir  << "|"
             << P.TotalDonor   << "\n";
        Curr = Curr->Next;
    }
    File.close();
}

// =============================================
// STOK DARAH
// =============================================

StokDarah MuatStokDariFile() {
    StokDarah Stok = {0, 0, 0, 0};
    ifstream File("data/stok.txt");
    if (!File.is_open()) return Stok;
    string Line;
    if (getline(File, Line)) {
        istringstream Iss(Line);
        string Val;
        getline(Iss, Val, '|'); Stok.StokA  = stoi(Val);
        getline(Iss, Val, '|'); Stok.StokB  = stoi(Val);
        getline(Iss, Val, '|'); Stok.StokAB = stoi(Val);
        getline(Iss, Val);      Stok.StokO  = stoi(Val);
    }
    File.close();
    return Stok;
}

void SimpanStokKeFile(const StokDarah& Stok) {
    ofstream File("data/stok.txt");
    if (!File.is_open()) return;
    File << Stok.StokA  << "|"
         << Stok.StokB  << "|"
         << Stok.StokAB << "|"
         << Stok.StokO  << "\n";
    File.close();
}

bool ValidasiGolDarah(const string& GolDarah) {
    return (GolDarah == "A" || GolDarah == "B" || GolDarah == "AB" || GolDarah == "O");
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

// =============================================
// RIWAYAT & VALIDASI
// =============================================

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
    return true;
}

int HitungSelisihHari(const string& TglTerakhir) {
    if (TglTerakhir.empty() || TglTerakhir == "-") return 9999;
    int Tahun = stoi(TglTerakhir.substr(0, 4));
    int Bulan = stoi(TglTerakhir.substr(5, 2));
    int Hari  = stoi(TglTerakhir.substr(8, 2));

    time_t Now = time(nullptr);
    tm TmDonor  = {};
    TmDonor.tm_year = Tahun - 1900;
    TmDonor.tm_mon  = Bulan - 1;
    TmDonor.tm_mday = Hari;
    time_t TDonor = mktime(&TmDonor);
    return (int)(difftime(Now, TDonor) / 86400);
}

bool UpdateRiwayatDonor(NodePendonor*& Head, const string& Username, const string& TglBaru) {
    NodePendonor* Node = CariPendonorByUsername(Head, Username);
    if (Node == nullptr) return false;
    Node->Data.TglTerakhir = TglBaru;
    Node->Data.TotalDonor++;
    SimpanPendonorKeFile(Head);
    return true;
}

// Cek username sudah ada di users.txt (nyambung sama Auth.cpp)
bool CekUsernameAdaDiFile(const string& Username) {
    ifstream File("data/users.txt");
    if (!File.is_open()) return false;
    string U, P, R;
    while (File >> U >> P >> R) {
        if (U == Username) {
            File.close();
            return true;
        }
    }
    File.close();
    return false;
}