#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdint>
#include <string>
#include <atomic>
#include <omp.h>
#include <cstdio>
#include <iomanip>
#include <gmp.h>
#include <mpfr.h>

using namespace std;
const double PI = 3.14159265358979323846;
const mpfr_prec_t MPFR_BITS = 5000;

#pragma pack(push, 1)
struct BMPHeader {
    uint16_t type{0x4D42};
    uint32_t size{0};
    uint16_t reserved1{0};
    uint16_t reserved2{0};
    uint32_t offBits{54};
    uint32_t structSize{40};
    int32_t  width{0};
    int32_t  height{0};
    uint16_t planes{1};
    uint16_t bitCount{24};
    uint32_t compression{0};
    uint32_t sizeImage{0};
    int32_t  xpelsPerMeter{2834};
    int32_t  ypelsPerMeter{2834};
    uint32_t clrUsed{0};
    uint32_t clrImportant{0};
};
#pragma pack(pop)

struct ComplexDouble {
    double re;
    double im;
};

void save_bmp(const string& filename, const vector<uint8_t>& data, int w, int h) {
    int rowSize = (w * 3 + 3) & ~3;
    BMPHeader header;
    header.width = w;
    header.height = h;
    header.sizeImage = rowSize * h;
    header.size = header.sizeImage + 54;
    ofstream f(filename, ios::binary);
    f.write(reinterpret_cast<char*>(&header), 54);
    f.write(reinterpret_cast<const char*>(data.data()), data.size());
    f.close();
}

int main() {
    cout << "Cleaning old frames..." << endl;
    for (int i = 0; i < 255; ++i) {
        string filename = "Mandelbrot" + to_string(1000 + i).substr(1) + ".bmp";
        std::remove(filename.c_str());
    }
    
    string absc_str, ordi_str, size_str;
    int choice;
    std::cout << "Select point (1-8): ";
    if (!(std::cin >> choice)) choice = 1;
    switch (choice) {
        case 1: absc_str = "-1.7491976289657893741942376816272921165326158557416159"; 
                ordi_str = "-0.00000042530777152440422725855012159249401150956515248"; 
                size_str = "4.31e-51"; break;
        case 2: absc_str = "-1.74907816150520173167912454515663360420734509948112463480292338384"; 
                ordi_str = "-0.00000550991906629096602513098567268615714673236269915508056068145"; 
                size_str = "1.53e-62"; break;
        case 3: absc_str = "-1.7489436617686633372073552153211507258063533373824414679761"; 
                ordi_str = "-0.0000073748967541889836640985849393311615399776865199722998"; 
                size_str = "1.01e-55"; break;
        case 4: absc_str = "-1.7491311840575335110236048528001036247123430447933925298694915282522178938437759580179"; 
                ordi_str = "-0.0001996960702541036804654299663680287246637758588467627907752429037173153157138373325"; 
                size_str = "2.84e-82"; break;
        case 5: absc_str = "-1.74994586497557459407526067070055710001"; 
                ordi_str = "-0.0000000852088539604644334731909824511"; 
                size_str = "7.17e-36"; break;
        case 6: absc_str = "-1.267078059171397835210199054200436920994876769284288837862647"; 
                ordi_str = "-0.123788215196292957558264285607075473360968832625384429809391"; 
                size_str = "2.4e-57"; break;
        case 7: absc_str = "-1.99999543561201124623198345433951143502785679245726844745821388800402678499411681518036306219179273434395557574279985918047221291197081186140687781560831995";
                ordi_str = "-0.00000000000000000000000026198152173811047783694060060607013913873144250985383083459221663448338433592617272786772587281530484110756597337683912309313885172";
                size_str = "2.6e-141"; break;
        case 8:
        {
            std::ifstream ff("Mandelbrot.txt");
            if (!ff.is_open()) {
                std::cerr << "Error: Mandelbrot.txt not found!" << std::endl;
                return 1;
            }
            std::vector<std::string> lines;
            std::string line;
            while (std::getline(ff, line)) {
                if (!line.empty()) lines.push_back(line);
                if (lines.size() == 3) break;
            }
            ff.close();
            if (lines.size() == 3) {
                absc_str = lines[0];
                ordi_str = lines[1];
                size_str = lines[2];
            } else {
                std::cerr << "Error: Mandelbrot.txt has invalid format!" << std::endl;
                return 1;
            }
            break;
        }
    }
    
    const int targetW = 2160;
    const int targetH = 2160;
    const int scale = 8;
    const int rawW = targetW * scale;
    const int rawH = targetH * scale;
    
    cout << "Step 1: Calculating Raw Map (" << rawW << "x" << rawH << ") using Perturbation..." << endl;
    vector<uint8_t> iterMap((size_t)rawW * rawH);
    
    mpfr_t rx, ry, zr, zi, zr2, zi2, tmp, sz, st;
    mpfr_inits2(MPFR_BITS, rx, ry, zr, zi, zr2, zi2, tmp, sz, st, NULL);
    mpfr_set_str(rx, absc_str.c_str(), 10, MPFR_RNDN);
    mpfr_set_str(ry, ordi_str.c_str(), 10, MPFR_RNDN);
    mpfr_set_str(sz, size_str.c_str(), 10, MPFR_RNDN);
    mpfr_div_ui(st, sz, rawW, MPFR_RNDN);
    double step_d = mpfr_get_d(st, MPFR_RNDN);
    
    double ref_rec_d = mpfr_get_d(rx, MPFR_RNDN);
    double ref_imc_d = mpfr_get_d(ry, MPFR_RNDN);
    
    vector<ComplexDouble> ref_orbit_double(50005);
    mpfr_set_ui(zr, 0, MPFR_RNDN);
    mpfr_set_ui(zi, 0, MPFR_RNDN);
    mpfr_set_ui(zr2, 0, MPFR_RNDN);
    mpfr_set_ui(zi2, 0, MPFR_RNDN);
    uint32_t ref_i = 0;
    bool escaped = false;
    
    while (ref_i < 50000) {
        ref_orbit_double[ref_i].re = mpfr_get_d(zr, MPFR_RNDN);
        ref_orbit_double[ref_i].im = mpfr_get_d(zi, MPFR_RNDN);
        mpfr_mul(tmp, zr, zi, MPFR_RNDN);
        mpfr_mul_ui(zi, tmp, 2, MPFR_RNDN);
        mpfr_add(zi, zi, ry, MPFR_RNDN);
        mpfr_sub(zr, zr2, zi2, MPFR_RNDN);
        mpfr_add(zr, zr, rx, MPFR_RNDN);
        mpfr_mul(zr2, zr, zr, MPFR_RNDN);
        mpfr_mul(zi2, zi, zi, MPFR_RNDN);
        if (escaped) {
            ref_i++;
            break;
        }
        mpfr_add(tmp, zr2, zi2, MPFR_RNDN);
        if (mpfr_cmp_d(tmp, 40000.0) >= 0) { 
            escaped = true;
        }
        ref_i++;
    }
    
    ref_orbit_double[ref_i].re = mpfr_get_d(zr, MPFR_RNDN);
    ref_orbit_double[ref_i].im = mpfr_get_d(zi, MPFR_RNDN);
    uint32_t max_valid_ref_iter = ref_i; 
    mpfr_clears(rx, ry, zr, zi, zr2, zi2, tmp, sz, st, NULL);
    
    atomic<int> linesDone{0};
    
    #pragma omp parallel for schedule(dynamic)
    for (size_t b = 0; b < (size_t)rawH; ++b) {
        for (size_t a = 0; a < (size_t)rawW; ++a) {
            double delta_rec = (double)((long long)a - (rawW / 2)) * step_d;
            double delta_imc = (double)((long long)b - (rawH / 2)) * step_d;
            uint32_t index = 0;    
            double delta_re = 0.0; 
            double delta_im = 0.0;
            double z_re = 0.0;     
            double z_im = 0.0;
            uint32_t i = 0;
            const ComplexDouble* ref_ptr = ref_orbit_double.data();
            bool has_re_based = false;
            
            while (i < 50000) {
                if ((z_re * z_re + z_im * z_im) >= 40000.0) {
                    break;
                }
                
                if (index >= max_valid_ref_iter) {
                    if (!has_re_based) {
                        break; 
                    } else {
                        double ld_cx = ref_rec_d + delta_rec;
                        double ld_cy = ref_imc_d + delta_imc; 
                        while (i < 50000 && (z_re * z_re + z_im * z_im) < 40000.0) {
                            double old_re = z_re;
                            double old_im = z_im;
                            z_re = old_re * old_re - old_im * old_im + ld_cx;
                            z_im = 2.0 * old_re * old_im + ld_cy;
                            i++;
                        }
                        break;
                    }
                }
                
                if ((z_re * z_re + z_im * z_im) < (delta_re * delta_re + delta_im * delta_im)) {
                    index = 0; 
                    delta_re = z_re;
                    delta_im = z_im;
                    has_re_based = true;
                }
                
                for (int step = 0; step < 2; ++step) {
                    double Ur = ref_ptr[index].re;
                    double Ui = ref_ptr[index].im;
                    double next_delta_im = 2.0 * Ur * delta_im + 2.0 * Ui * delta_re + 2.0 * delta_re * delta_im + delta_imc;
                    delta_re = 2.0 * Ur * delta_re - 2.0 * Ui * delta_im + delta_re * delta_re - delta_im * delta_im + delta_rec;
                    delta_im = next_delta_im;
                    index++;
                }
                z_re = ref_ptr[index].re + delta_re;
                z_im = ref_ptr[index].im + delta_im;
                i += 2; 
            }
            
            int final_t = 50000 - i;
            if (final_t == 0) {
                iterMap[b * (size_t)rawW + a] = 255;
            } else {
                iterMap[b * (size_t)rawW + a] = (uint8_t)(final_t % 254);
            }
        }
        if (++linesDone % 100 == 0) cout << "Progress: " << linesDone << "/" << rawH << "\r" << flush;
    }
    
    uint8_t pal[256][3];
    for (int a = 0; a < 255; ++a) {
        pal[a][0] = (uint8_t)round(127.0 + 127.0 * cos(2.0 * PI * a / 255.0));
        pal[a][1] = (uint8_t)round(127.0 + 127.0 * sin(2.0 * PI * a / 255.0));
        pal[a][2] = (uint8_t)round(127.0 + 127.0 * sin(2.0 * PI * a / 255.0));
    }
    pal[255][0] = 255; pal[255][1] = 255; pal[255][2] = 255;
    
    cout << "\nStep 2: Rendering frames..." << endl;
    int rowSize = (targetW * 3 + 3) & ~3;
    
    for (int frame = 0; frame < 255; ++frame) {
        vector<uint8_t> frameData(rowSize * targetH);        
        #pragma omp parallel for schedule(static)
        for (int y = 0; y < targetH; ++y) {
            for (int x = 0; x < targetW; ++x) {
                uint32_t rSum = 0, gSum = 0, bSum = 0;
                for (int j = 0; j < scale; ++j) {
                    size_t mapRowIdx = (size_t)(y * scale + j) * rawW;
                    for (int i = 0; i < scale; ++i) {
                        uint8_t t = iterMap[mapRowIdx + (x * scale + i)];
                        int colorIdx;
                        if (t == 255) {
                            colorIdx = 255;
                        } else {
                            colorIdx = (t - frame + 255) % 255;
                        }
                        bSum += pal[colorIdx][0];
                        gSum += pal[colorIdx][1];
                        rSum += pal[colorIdx][2];
                    }
                }
                int outIdx = y * rowSize + x * 3;
                frameData[outIdx + 0] = (uint8_t)(bSum >> 6);
                frameData[outIdx + 1] = (uint8_t)(gSum >> 6);
                frameData[outIdx + 2] = (uint8_t)(rSum >> 6);
            }
        }
        string filename = "Mandelbrot" + to_string(1000 + frame).substr(1) + ".bmp";
        save_bmp(filename, frameData, targetW, targetH);
        cout << "Frame " << frame << "/254 saved.   \r" << flush;
    }
return 0;
}
