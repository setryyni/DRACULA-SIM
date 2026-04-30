#include "../include/Utils.h"
#include "../include/Auth.h"
#include "../include/UserPage.h"
#include <iostream>
using namespace std;

// informasi syarat donor
void edukasi() {
    int pilih;
    do {
        cout << "\n=== EDUKASI DONOR DARAH ===\n";
        cout << "1. Syarat Donor Darah\n";
        cout << "2. Manfaat Donor Darah\n";
        cout << "3. Kondisi Tidak Boleh Donor\n";
        cout << "4. Tips Sebelum Donor\n";
        cout << "0. Kembali\n";
        cout << "Pilih: ";
        cin >> pilih;

        switch(pilih) {
            case 1:
                cout << "\n=== SYARAT DONOR DARAH ===\n";
                cout << "- Usia 17 - 60 tahun\n";
                cout << "- Berat badan minimal 45 kg\n";
                cout << "- Tekanan darah normal (90/60 - 140/90)\n";
                cout << "- Kadar hemoglobin minimal 12.5 g/dL\n";
                cout << "- Dalam kondisi sehat\n";
                cout << "- Tidak sedang mengonsumsi obat tertentu\n";
                cout << "- Jarak donor minimal 2-3 bulan\n";
                break;

            case 2:
                cout << "\n=== MANFAAT DONOR DARAH ===\n";
                cout << "- Membantu menyelamatkan nyawa orang lain\n";
                cout << "- Meningkatkan kesehatan jantung\n";
                cout << "- Merangsang produksi sel darah baru\n";
                cout << "- Mengurangi risiko kelebihan zat besi\n";
                cout << "- Mendapatkan pemeriksaan kesehatan gratis\n";
                break;

            case 3:
                cout << "\n=== KONDISI TIDAK BOLEH DONOR ===\n";
                cout << "- Sedang demam atau sakit\n";
                cout << "- Tekanan darah tidak normal\n";
                cout << "- Hb rendah\n";
                cout << "- Baru melakukan operasi\n";
                cout << "- Memiliki penyakit menular (HIV, Hepatitis)\n";
                cout << "- Sedang hamil atau menyusui\n";
                break;

            case 4:
                cout << "\n=== TIPS SEBELUM DONOR ===\n";
                cout << "- Tidur cukup minimal 6-8 jam\n";
                cout << "- Makan makanan bergizi sebelum donor\n";
                cout << "- Minum air putih yang cukup\n";
                cout << "- Hindari alkohol\n";
                cout << "- Jangan donor saat perut kosong\n";
                break;

            default:
                cout << "\n[!] Pilihan tidak tersedia.\n";
                Utils::tekanEnter();
                break;
        }

    } while (pilih != 0);
}