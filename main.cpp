#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdint>
#include <atomic>
#include <omp.h>
using namespace std;
const double PI = 3.14159265358979323846;
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
uint8_t asm_red_palette[256] = {
    0x00, 0x00, 0x08, 0xDC, 0x33, 0x45, 0xAB, 0x51, 0x29, 0x5F, 0x6C, 0x14, 0x79, 0x12, 0xD7, 0x0F,
    0x4B, 0xEA, 0x5E, 0xC6, 0x53, 0xB2, 0xD8, 0xB7, 0x4E, 0x29, 0x54, 0x77, 0x3F, 0xD3, 0x47, 0x7B,
    0x26, 0xDF, 0x49, 0xC5, 0xF9, 0x7E, 0xE3, 0xD3, 0x05, 0x24, 0x24, 0x80, 0x05, 0x97, 0x02, 0xC6,
    0xA6, 0xC5, 0xB5, 0x8E, 0x34, 0xAE, 0x97, 0xF4, 0xA4, 0xFF, 0x3E, 0xAD, 0x4B, 0x15, 0xC5, 0x7E,
    0xE1, 0x82, 0x92, 0xF4, 0xAF, 0x56, 0x01, 0xB4, 0xFD, 0xC6, 0xBF, 0x32, 0x24, 0xE9, 0xC8, 0x96,
    0xDD, 0xB0, 0x49, 0x17, 0xA1, 0x47, 0xE0, 0x29, 0x45, 0x8B, 0xF5, 0x2C, 0x28, 0x44, 0x2D, 0x93,
    0x7F, 0x49, 0x27, 0x9C, 0xBD, 0xBF, 0x14, 0x13, 0x0F, 0x1B, 0xCB, 0xCE, 0x61, 0x9C, 0x6F, 0xBD,
    0x7E, 0x57, 0xD1, 0xD1, 0x24, 0x17, 0x11, 0x0F, 0x31, 0xAC, 0x3A, 0x92, 0x41, 0x5D, 0xD6, 0x50,
    0x50, 0xA8, 0xB1, 0xE7, 0x51, 0x7A, 0x4B, 0xC4, 0xBD, 0x8D, 0xB9, 0x69, 0x8A, 0x98, 0xE1, 0xAB,
    0x1C, 0x78, 0x67, 0x1D, 0x0D, 0x8C, 0xAC, 0x14, 0xEB, 0xD6, 0xAE, 0xED, 0x46, 0xB3, 0xBC, 0x7F,
    0xA9, 0x28, 0x3A, 0xF2, 0x5D, 0x20, 0xFC, 0x4E, 0x0D, 0x1A, 0xDD, 0x8D, 0xB2, 0x19, 0xAB, 0x04,
    0x4F, 0xE7, 0x89, 0x53, 0x71, 0xE6, 0x58, 0x5A, 0x7C, 0x12, 0xD9, 0xC2, 0x2E, 0xEC, 0x7E, 0x20,
    0xE4, 0x67, 0x35, 0xCE, 0x96, 0x1C, 0x9F, 0xED, 0x8A, 0x53, 0x72, 0x3A, 0x2F, 0xB0, 0xFE, 0xA1,
    0xB1, 0x8A, 0x18, 0xBF, 0x26, 0x3B, 0xE1, 0xB6, 0x70, 0xFA, 0x27, 0x0A, 0x2F, 0x02, 0x60, 0x65,
    0x5D, 0x15, 0x74, 0x81, 0x75, 0xAC, 0xD1, 0x91, 0x40, 0x5F, 0x95, 0xDF, 0x9B, 0x41, 0xB2, 0x8E,
    0xE0, 0x5C, 0x5D, 0x9F, 0xC7, 0x74, 0x34, 0xB2, 0xD6, 0xC0, 0xE6, 0x2F, 0xC6, 0x44, 0x4B, 0xB3
};
int main() {
    long double absc     = -1.39966699645936;
    long double ordi     = 0.0005429083913;
    long double size_val = 0.000000000000026;
    const int horiz = 1920;
    const int vert = 1080;
    const int rowSize = (horiz * 3 + 3) & ~3; 
    BMPHeader h;
    h.width = horiz;
    h.height = vert;
    h.sizeImage = rowSize * vert;
    h.size = h.sizeImage + 54;
    uint8_t pal[256][3];
    for (int a = 0; a < 255; ++a) {
        pal[a][0] = (uint8_t)round(127 + 127 * cos(2 * PI * a / 255.0));
        pal[a][1] = (uint8_t)round(127 + 127 * sin(2 * PI * a / 255.0));
        pal[a][2] = asm_red_palette[a];
    }
    pal[255][0] = 255; pal[255][1] = 255; pal[255][2] = 255;
    long double step = size_val / (horiz << 3);
    long double absc2 = absc - step * ((horiz << 3) - 1) / 2.0;
    long double ordi2 = ordi - step * ((vert << 3) - 1) / 2.0;
    vector<uint8_t> allData(h.sizeImage, 0);    
    atomic<int> linesLeft{vert};
    cout << "Starting calculation on " << omp_get_max_threads() << " threads..." << endl;
    #pragma omp parallel for schedule(dynamic)
    for (int b = 0; b < vert; ++b) {
        int nn = b << 3;
        for (int a = 0; a < horiz; ++a) {
            int mm = a << 3;
            long z_sum[3] = {0, 0, 0};
            for (int j = 0; j < 8; ++j) {
                long double n_coord = ordi2 + (nn + j) * step;
                for (int i = 0; i < 8; ++i) {
                    long double m_coord = absc2 + (mm + i) * step;
                    long double c_re = m_coord, d_im = n_coord;
                    int t = 50000;
                    long double cc, dd;
                    do {
                        cc = c_re * c_re;
                        dd = d_im * d_im;
                        d_im = 2 * c_re * d_im + n_coord;
                        c_re = cc - dd + m_coord;
                        t--;
                    } while (t > 0 && (cc + dd <= 10000.0));
                    int colorIdx = (t == 0) ? 255 : (t % 255);
                    z_sum[0] += pal[colorIdx][0];
                    z_sum[1] += pal[colorIdx][1];
                    z_sum[2] += pal[colorIdx][2];
                }
            }
            int pixelPos = b * rowSize + a * 3;
            allData[pixelPos + 0] = (uint8_t)(z_sum[0] >> 6);
            allData[pixelPos + 1] = (uint8_t)(z_sum[1] >> 6);
            allData[pixelPos + 2] = (uint8_t)(z_sum[2] >> 6);
        }
        int current = --linesLeft;
        if (current % 10 == 0 || current < 10) {
            #pragma omp critical
            {
                cout << "Lines remaining: " << current << "    \r" << flush;
            }
        }
    }
    ofstream f("Mandelbrot.bmp", ios::binary);
    if (f.is_open()) {
        f.write(reinterpret_cast<char*>(&h), 54);
        f.write(reinterpret_cast<char*>(allData.data()), allData.size());
        f.close();
        cout << "\nFinished! Mandelbrot.bmp saved." << endl;
    } else {
        cerr << "\nError: Could not save the file." << endl;
    }
    return 0;
}
