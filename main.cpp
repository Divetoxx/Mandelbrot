#include <atomic>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <mpfr.h>
#include <omp.h>

using std::vector;
using std::string;
using std::min;

const double PI = 3.14159265358979323846;
const mpfr_prec_t MPFR_BITS = 1000;

const int TARGET_W = 1000;
const int TARGET_H = 1000;
const int SCALE = 8;
const int RAW_W = TARGET_W * SCALE;
const int RAW_H = TARGET_H * SCALE;
const int MAX_ITER = 50000;
const double ESCAPE_RADIUS_SQUARED = 50000.0;
const int REF_SIZE = MAX_ITER + 200;

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
    
    std::ofstream f(filename, std::ios::binary);
    if (f.is_open()) {
        f.write(reinterpret_cast<char*>(&header), 54);
        f.write(reinterpret_cast<const char*>(data.data()), data.size());
        f.close();
    }
}

int main() {
    const double startTime = omp_get_wtime();

    std::fprintf(stderr, "Cleaning old frames...\n");
    for (int i = 0; i < 255; ++i) {
        char filename[64];
        std::snprintf(filename, sizeof(filename), "Mandelbrot%03d.bmp", i);
        std::remove(filename);
    }

    string absc_str, ordi_str, size_str;
    int choice = 1;
    std::printf("Select point (1-8): ");
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
                size_str = "1.15e-119"; break;
        case 8:
        {
            std::ifstream ff("Mandelbrot.txt");
            if (!ff.is_open()) {
                std::fprintf(stderr, "Error: Mandelbrot.txt not found!\n");
                return 1;
            }
            vector<string> lines;
            string line;
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
                std::fprintf(stderr, "Error: Mandelbrot.txt has invalid format!\n");
                return 1;
            }
            break;
        }
    }
	
    std::fprintf(stderr, "Step 1: Calculating Reference Orbit using MPFR...\n");
    vector<uint8_t> iterMap(static_cast<size_t>(RAW_W) * RAW_H);
    mpfr_t rx, ry, zr, zi, zr2, zi2, tmp, sz, st;
    mpfr_inits2(MPFR_BITS, rx, ry, zr, zi, zr2, zi2, tmp, sz, st, nullptr);
    mpfr_set_str(rx, absc_str.c_str(), 10, MPFR_RNDN);
    mpfr_set_str(ry, ordi_str.c_str(), 10, MPFR_RNDN);
    mpfr_set_str(sz, size_str.c_str(), 10, MPFR_RNDN);
    mpfr_div_ui(st, sz, RAW_W, MPFR_RNDN);
    double step_d = mpfr_get_d(st, MPFR_RNDN);
    double ref_rec_d = mpfr_get_d(rx, MPFR_RNDN);
    double ref_imc_d = mpfr_get_d(ry, MPFR_RNDN);
    vector<ComplexDouble> ref_orbit_double;
    ref_orbit_double.reserve(REF_SIZE);
    mpfr_set_ui(zr, 0, MPFR_RNDN);
    mpfr_set_ui(zi, 0, MPFR_RNDN);
    mpfr_set_ui(zr2, 0, MPFR_RNDN);
    mpfr_set_ui(zi2, 0, MPFR_RNDN);
    uint32_t ref_i = 0;

    while (ref_i < REF_SIZE - 1) {
        ComplexDouble z{mpfr_get_d(zr, MPFR_RNDN), mpfr_get_d(zi, MPFR_RNDN)};
        ref_orbit_double.push_back(z);
        mpfr_mul(tmp, zr, zi, MPFR_RNDN);
        mpfr_mul_ui(zi, tmp, 2, MPFR_RNDN);
        mpfr_add(zi, zi, ry, MPFR_RNDN);
        mpfr_sub(zr, zr2, zi2, MPFR_RNDN);
        mpfr_add(zr, zr, rx, MPFR_RNDN);
        mpfr_mul(zr2, zr, zr, MPFR_RNDN);
        mpfr_mul(zi2, zi, zi, MPFR_RNDN);

        mpfr_add(tmp, zr2, zi2, MPFR_RNDN);
        if (mpfr_cmp_d(tmp, ESCAPE_RADIUS_SQUARED) >= 0) { 
            break;
        }
        ref_i++;
    }
    ref_orbit_double.push_back({mpfr_get_d(zr, MPFR_RNDN), mpfr_get_d(zi, MPFR_RNDN)});
    uint32_t max_valid_ref_iter = static_cast<uint32_t>(ref_orbit_double.size());

    double limit_epsilon_squared;
    mpfr_exp_t current_binary_exp = mpfr_get_exp(st);
    if (current_binary_exp > -244) {
        limit_epsilon_squared = 1e-40;
    } else {
        limit_epsilon_squared = 1e-60;
    }
    mpfr_clears(rx, ry, zr, zi, zr2, zi2, tmp, sz, st, nullptr);

    std::fprintf(stderr, "Precomputing skip100 matrices...\n");
    vector<ComplexDouble> coeff_A(REF_SIZE, {1.0, 0.0});
    vector<ComplexDouble> coeff_B(REF_SIZE, {0.0, 0.0});
    vector<double> rad_R(REF_SIZE, 2.0);

    vector<double> aS_squared(REF_SIZE, 0.0);
    for (size_t i = 0; i < max_valid_ref_iter; ++i) {
        double r2 = ref_orbit_double[i].re * ref_orbit_double[i].re + ref_orbit_double[i].im * ref_orbit_double[i].im;
        aS_squared[i] = (r2 < ESCAPE_RADIUS_SQUARED) ? r2 : 0.0;
    }

    const int loop_limit = std::min(static_cast<int>(MAX_ITER), static_cast<int>(max_valid_ref_iter) - 105);

    #pragma omp parallel for
    for (int i = 0; i < loop_limit; ++i) {
        double min_r2 = ESCAPE_RADIUS_SQUARED;
        for (int k = 0; k < 100; ++k) {
            if (i + k >= (int)max_valid_ref_iter) break;
            if (aS_squared[i + k] < min_r2) min_r2 = aS_squared[i + k];
        }
        rad_R[i] = std::sqrt(min_r2);

        for (int k = 0; k < 100; ++k) {
            if (i + k >= (int)max_valid_ref_iter) break;
            double r_re = ref_orbit_double[i + k].re;
            double r_im = ref_orbit_double[i + k].im;

            double next_A_re = 2.0 * (r_re * coeff_A[i].re - r_im * coeff_A[i].im);
            double next_A_im = 2.0 * (r_re * coeff_A[i].im + r_im * coeff_A[i].re);
            double next_B_re = 2.0 * (r_re * coeff_B[i].re - r_im * coeff_B[i].im) + 1.0;
            double next_B_im = 2.0 * (r_re * coeff_B[i].im + r_im * coeff_B[i].re);
            coeff_A[i].re = next_A_re; coeff_A[i].im = next_A_im;
            coeff_B[i].re = next_B_re; coeff_B[i].im = next_B_im;
        }
    }

    std::fprintf(stderr, "Calculating Raw Map (%dx%d) with skip100 and rollback...\n", RAW_W, RAW_H);
    std::atomic<int> linesDone{0};
    const ComplexDouble* ref_ptr = ref_orbit_double.data();

    #pragma omp parallel for schedule(dynamic)
    for (size_t b = 0; b < (size_t)RAW_H; ++b) {
        for (size_t a = 0; a < (size_t)RAW_W; ++a) {
            double delta_rec = (double)((long long)a - (RAW_W / 2)) * step_d;
            double delta_imc = (double)((long long)b - (RAW_H / 2)) * step_d;
            uint32_t index = 0;
            double delta_re = 0.0; 
            double delta_im = 0.0;
            double z_re = 0.0; 
            double z_im = 0.0;
            uint32_t i = 0;
            bool has_re_based = false;

            while (i < MAX_ITER) {
                if ((z_re * z_re + z_im * z_im) >= ESCAPE_RADIUS_SQUARED) {
                    break;
                }
                if (index >= max_valid_ref_iter) {
                    if (!has_re_based) {
                        break; 
                    } else {
                        double ld_cx = ref_rec_d + delta_rec;
                        double ld_cy = ref_imc_d + delta_imc; 
                        while (i < MAX_ITER && (z_re * z_re + z_im * z_im) < ESCAPE_RADIUS_SQUARED) {
                            double old_re = z_re;
                            double old_im = z_im;
                            z_re = old_re * old_re - old_im * old_im + ld_cx;
                            z_im = 2.0 * old_re * old_im + ld_cy;
                            i++;
                        }
                        break;
                    }
                }
                
                double eps_abs2 = delta_re * delta_re + delta_im * delta_im;
                double limit_r2 = limit_epsilon_squared * rad_R[index] * rad_R[index]; 

                if (eps_abs2 < limit_r2 && (index + 100 < loop_limit) && (i + 100 < MAX_ITER)) {
                    double backup_deltaRe = delta_re; double backup_deltaIm = delta_im;
                    int backup_refIdx = index;
                    int backup_iter = i;
                    double next_eps_re = (coeff_A[index].re * delta_re - coeff_A[index].im * delta_im) + 
                                         (coeff_B[index].re * delta_rec - coeff_B[index].im * delta_imc);
                    double next_eps_im = (coeff_A[index].re * delta_im + coeff_A[index].im * delta_re) + 
                                         (coeff_B[index].re * delta_imc + coeff_B[index].im * delta_rec);
                    delta_re = next_eps_re;
                    delta_im = next_eps_im;
                    index += 100;
                    i += 100;
                    z_re = ref_ptr[index].re + delta_re;
                    z_im = ref_ptr[index].im + delta_im;
                    if (z_re * z_re + z_im * z_im >= ESCAPE_RADIUS_SQUARED) {
                        delta_re = backup_deltaRe; delta_im = backup_deltaIm;
                        index = backup_refIdx;
                        i = backup_iter;
                    } else {
                        continue; 
                    }
                }
                if ((z_re * z_re + z_im * z_im) < eps_abs2) {
                    index = 0; 
                    delta_re = z_re;
                    delta_im = z_im;
                    has_re_based = true;
                }
                const double aa = 2.0 * ref_ptr[index].re + delta_re;
                const double bb = 2.0 * ref_ptr[index].im + delta_im;
                const double nextDeltaRe = aa * delta_re - bb * delta_im + delta_rec;
                delta_im = aa * delta_im + bb * delta_re + delta_imc;
                delta_re = nextDeltaRe;
                index++;
                i++;
                z_re = ref_ptr[index].re + delta_re;
                z_im = ref_ptr[index].im + delta_im;
            }
            int final_t = MAX_ITER - i;
            iterMap[b * (size_t)RAW_W + a] = (final_t == 0) ? 255 : static_cast<uint8_t>(final_t % 254);
        }
        if (++linesDone % 100 == 0) {
            std::fprintf(stderr, "Progress: %d/%d rows (%.1f%%)\r", linesDone.load(), RAW_H, 100.0 * linesDone / RAW_H);
        }
    }

    uint8_t pal[256][3];
    for (int a = 0; a < 255; ++a) {
        pal[a][0] = (uint8_t)std::lround(127.0 + 127.0 * std::cos(2.0 * PI * a / 255.0));
        pal[a][1] = (uint8_t)std::lround(127.0 + 127.0 * std::sin(2.0 * PI * a / 255.0));
        pal[a][2] = (uint8_t)std::lround(127.0 + 127.0 * std::sin(2.0 * PI * a / 255.0));
    }
    pal[255][0] = 255; pal[255][1] = 255; pal[255][2] = 255;

    std::fprintf(stderr, "\nStep 2: Rendering frames...\n");
    int rowSize = (TARGET_W * 3 + 3) & ~3;

    for (int frame = 0; frame < 255; ++frame) {
        vector<uint8_t> frameData(static_cast<size_t>(rowSize) * TARGET_H, 0);        
        
        #pragma omp parallel for schedule(static)
        for (int y = 0; y < TARGET_H; ++y) {
            for (int x = 0; x < TARGET_W; ++x) {
                uint32_t rSum = 0, gSum = 0, bSum = 0;
                for (int j = 0; j < SCALE; ++j) {
                    size_t mapRowIdx = (size_t)(y * SCALE + j) * RAW_W;
                    for (int i = 0; i < SCALE; ++i) {
                        uint8_t t = iterMap[mapRowIdx + (x * SCALE + i)];
                        int colorIdx = (t == 255) ? 255 : (t - frame + 255) % 255;
                        bSum += pal[colorIdx][0];
                        gSum += pal[colorIdx][1];
                        rSum += pal[colorIdx][2];
                    }
                }
                int outIdx = y * rowSize + x * 3;
                frameData[outIdx + 0] = (uint8_t)(bSum / (SCALE * SCALE));
                frameData[outIdx + 1] = (uint8_t)(gSum / (SCALE * SCALE));
                frameData[outIdx + 2] = (uint8_t)(rSum / (SCALE * SCALE));
            }
        }
        char filename[64];
        std::snprintf(filename, sizeof(filename), "Mandelbrot%03d.bmp", frame);
        save_bmp(filename, frameData, TARGET_W, TARGET_H);
        std::fprintf(stderr, "Frame %d/254 saved.   \r", frame);
    }

    std::fprintf(stderr, "\nAll Done! Render completed in %.1f seconds.\n", omp_get_wtime() - startTime);
    return 0;
}
