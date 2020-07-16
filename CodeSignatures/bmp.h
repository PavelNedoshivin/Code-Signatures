#ifndef BMP_H
#define BMP_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <cmath>
#include <string>
#include <algorithm>
const double PI = 3.1415926535;
typedef struct
{
	unsigned char bfType[2] = { 'B', 'M' };
	unsigned int bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned int BfOffBits;
} BITMAPFILEHEADER;
typedef struct
{
	unsigned int biSize;
	unsigned int biWidth;
	unsigned int biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned int biCompression;
	unsigned int biSizeImage;
	unsigned int biXPelsPerMeter;
	unsigned int biYPelsPerMeter;
	unsigned int biClrUsed;
	unsigned int biClrImportant;
} BITMAPINFOHEADER;
typedef struct
{
	unsigned char rgbBlue;
	unsigned char rgbGreen;
	unsigned char rgbRed;
} RGBQUADS;
class BMP
{
private:
	BITMAPFILEHEADER* bmpfh;
	BITMAPINFOHEADER* bmpih;
	std::vector<std::vector<RGBQUADS>> pixels;
	std::vector<std::vector<RGBQUADS>> yccPixels;
	std::vector<std::vector<double>> spectreY;
	std::vector<std::vector<double>> spectreCb;
	std::vector<std::vector<double>> spectreCr;	
	int clip(int value);
	double get_sqr_C(int value);
public:
	BMP();
	void load(const std::string& filename);
	void save(const std::string& filename) const;
	void transformToYCC();
	void transformFromYCC();
	void calculatePSNR(std::vector<std::vector<RGBQUADS>> pix, bool show);
	std::vector<std::vector<RGBQUADS>> getPixels();
	BITMAPINFOHEADER* getHeader();
	void DCT();
	void IDCT();
	~BMP();
};
#endif