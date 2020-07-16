#include "bmp.h"
using namespace std;
BMP::BMP()
{
	bmpfh = new BITMAPFILEHEADER;
	bmpih = new BITMAPINFOHEADER;
}
BMP::~BMP()
{
	delete bmpfh;
	delete bmpih;
}
void BMP::load(const string& filename)
{
	ifstream fin(filename, ios_base::binary | ios_base::in);
	fin.exceptions(ios_base::failbit | ios_base::badbit);
	fin.seekg(ios_base::beg);
	fin.read((char*)(&bmpfh->bfType), sizeof(bmpfh->bfType));
	fin.read((char*)(&bmpfh->bfSize), sizeof(bmpfh->bfSize));
	fin.read((char*)(&bmpfh->bfReserved1), sizeof(bmpfh->bfReserved1));
	fin.read((char*)(&bmpfh->bfReserved2), sizeof(bmpfh->bfReserved2));
	fin.read((char*)(&bmpfh->BfOffBits), sizeof(bmpfh->BfOffBits));
	fin.read((char*)(&bmpih->biSize), sizeof(bmpih->biSize));
	fin.read((char*)(&bmpih->biWidth), sizeof(bmpih->biWidth));
	fin.read((char*)(&bmpih->biHeight), sizeof(bmpih->biHeight));
	fin.read((char*)(&bmpih->biPlanes), sizeof(bmpih->biPlanes));
	fin.read((char*)(&bmpih->biBitCount), sizeof(bmpih->biBitCount));
	fin.read((char*)(&bmpih->biCompression), sizeof(bmpih->biCompression));
	fin.read((char*)(&bmpih->biSizeImage), sizeof(bmpih->biSizeImage));
	fin.read((char*)(&bmpih->biXPelsPerMeter), sizeof(bmpih->biXPelsPerMeter));
	fin.read((char*)(&bmpih->biYPelsPerMeter), sizeof(bmpih->biYPelsPerMeter));
	fin.read((char*)(&bmpih->biClrUsed), sizeof(bmpih->biClrUsed));
	fin.read((char*)(&bmpih->biClrImportant), sizeof(bmpih->biClrImportant));
	pixels.resize(bmpih->biHeight);
	for (int i = 0; i < bmpih->biHeight; i++)
	{
		pixels[i].resize(bmpih->biWidth);
		for (int j = 0; j < bmpih->biWidth; j++)
		{
			fin.read((char*)(&pixels[i][j].rgbBlue), sizeof(pixels[i][j].rgbBlue));
			fin.read((char*)(&pixels[i][j].rgbGreen), sizeof(pixels[i][j].rgbGreen));
			fin.read((char*)(&pixels[i][j].rgbRed), sizeof(pixels[i][j].rgbRed));
		}
		int flag = (bmpih->biWidth * bmpih->biBitCount / 8) % 4;
		while (flag != 0)
		{
			flag = (flag + 1) % 4;
			unsigned char x = 0;
			fin.read((char*)(&x), sizeof(x));
		}
	}
	fin.close();
}
void BMP::save(const string& filename) const
{
	ofstream fout(filename, ios_base::binary | ios_base::out);
	fout.exceptions(ios_base::failbit | ios_base::badbit);
	fout.seekp(ios_base::beg);
	fout.write((char*)(bmpfh->bfType), sizeof(bmpfh->bfType));
	fout.write((char*)(&bmpfh->bfSize), sizeof(bmpfh->bfSize));
	fout.write((char*)(&bmpfh->bfReserved1), sizeof(bmpfh->bfReserved1));
	fout.write((char*)(&bmpfh->bfReserved2), sizeof(bmpfh->bfReserved2));
	fout.write((char*)(&bmpfh->BfOffBits), sizeof(bmpfh->BfOffBits));
	fout.write((char*)(&bmpih->biSize), sizeof(bmpih->biSize));
	fout.write((char*)(&bmpih->biWidth), sizeof(bmpih->biWidth));
	fout.write((char*)(&bmpih->biHeight), sizeof(bmpih->biHeight));
	fout.write((char*)(&bmpih->biPlanes), sizeof(bmpih->biPlanes));
	fout.write((char*)(&bmpih->biBitCount), sizeof(bmpih->biBitCount));
	fout.write((char*)(&bmpih->biCompression), sizeof(bmpih->biCompression));
	fout.write((char*)(&bmpih->biSizeImage), sizeof(bmpih->biSizeImage));
	fout.write((char*)(&bmpih->biXPelsPerMeter), sizeof(bmpih->biXPelsPerMeter));
	fout.write((char*)(&bmpih->biYPelsPerMeter), sizeof(bmpih->biYPelsPerMeter));
	fout.write((char*)(&bmpih->biClrUsed), sizeof(bmpih->biClrUsed));
	fout.write((char*)(&bmpih->biClrImportant), sizeof(bmpih->biClrImportant));
	for (int i = 0; i < bmpih->biHeight; i++)
	{
		for (int j = 0; j < bmpih->biWidth; j++)
		{
			fout.write((char*)(&pixels[i][j].rgbBlue), sizeof(pixels[i][j].rgbBlue));
			fout.write((char*)(&pixels[i][j].rgbGreen), sizeof(pixels[i][j].rgbGreen));
			fout.write((char*)(&pixels[i][j].rgbRed), sizeof(pixels[i][j].rgbRed));
		}
		int flag = (bmpih->biWidth * bmpih->biBitCount / 8) % 4;
		while (flag != 0)
		{
			flag = (flag + 1) % 4;
			unsigned char x = 0;
			fout.write((char*)(&x), sizeof(x));
		}
	}
	fout.close();
}
void BMP::transformToYCC()
{
	yccPixels.resize(bmpih->biHeight);
	for (int i = 0; i < bmpih->biHeight; i++)
	{
		yccPixels[i].resize(bmpih->biWidth);
		for (int j = 0; j < bmpih->biWidth; j++)
		{
			int Y = 0.299 * pixels[i][j].rgbRed + 0.587 * pixels[i][j].rgbGreen + 0.114 * pixels[i][j].rgbBlue;
			if (Y < 0)
			{
				Y = 0;
			}
			else
			{
				if (Y > 255)
				{
					Y = 255;
				}
			}
			int Cb = 0.5643 * ((double)pixels[i][j].rgbBlue - Y) + 128;
			if (Cb < 0)
			{
				Cb = 0;
			}
			else
			{
				if (Cb > 255)
				{
					Cb = 255;
				}
			}
			int Cr = 0.7132 * ((double)pixels[i][j].rgbRed - Y) + 128;
			if (Cr < 0)
			{
				Cr = 0;
			}
			else
			{
				if (Cr > 255)
				{
					Cr = 255;
				}
			}
			yccPixels[i][j].rgbRed = Y;
			yccPixels[i][j].rgbGreen = Cb;
			yccPixels[i][j].rgbBlue = Cr;
		}
		int flag = (bmpih->biWidth * bmpih->biBitCount / 8) % 4;
		while (flag != 0)
		{
			flag = (flag + 1) % 4;
		}
	}
}
void BMP::transformFromYCC()
{
	for (int i = 0; i < bmpih->biHeight; i++)
	{
		for (int j = 0; j < bmpih->biWidth; j++)
		{
			int R = yccPixels[i][j].rgbRed + 1.402 * (yccPixels[i][j].rgbBlue - 128);
			int G = yccPixels[i][j].rgbRed - 0.714 * (yccPixels[i][j].rgbBlue - 128) - 0.334 * (yccPixels[i][j].rgbGreen - 128);
			int B = yccPixels[i][j].rgbRed + 1.772 * (yccPixels[i][j].rgbGreen - 128);
			if (R < 0)
			{
				R = 0;
			}
			else
			{
				if (R > 255)
				{
					R = 255;
				}
			}
			if (G < 0)
			{
				G = 0;
			}
			else
			{
				if (G > 255)
				{
					G = 255;
				}
			}
			if (B < 0)
			{
				B = 0;
			}
			else
			{
				if (B > 255)
				{
					B = 255;
				}
			}
			pixels[i][j].rgbRed = R;
			pixels[i][j].rgbGreen = G;
			pixels[i][j].rgbBlue = B;
		}
		int flag = (bmpih->biWidth * bmpih->biBitCount / 8) % 4;
		while (flag != 0)
		{
			flag = (flag + 1) % 4;
		}
	}
}
void BMP::calculatePSNR(vector<vector<RGBQUADS>> pix, bool show)
{
	double psnrR = 0;
	double psnrG = 0;
	double psnrB = 0;
	for (int i = 0; i < bmpih->biHeight - 3; i++)
	{
		for (int j = 0; j < bmpih->biWidth - 3; j++)
		{
			int val = abs(pixels[i][j].rgbRed - pix[i][j].rgbRed);
			psnrR += pow(val, 2);
			val = abs(pixels[i][j].rgbGreen - pix[i][j].rgbGreen);
			psnrG += pow(val, 2);
			val = abs(pixels[i][j].rgbBlue - pix[i][j].rgbBlue);
			psnrB += pow(val, 2);
		}
	}
	if (psnrR != 0)
	{
		psnrR = 10 * log10((double)bmpih->biHeight * (double)bmpih->biWidth * pow(pow(2, 8) - 1, 2) / psnrR);
	}
	if (psnrG != 0)
	{
		psnrG = 10 * log10((double)bmpih->biHeight * (double)bmpih->biWidth * pow(pow(2, 8) - 1, 2) / psnrG);
	}
	if (psnrB != 0)
	{
		psnrB = 10 * log10((double)bmpih->biHeight * (double)bmpih->biWidth * pow(pow(2, 8) - 1, 2) / psnrB);
	}
	if (show)
	{
		cout << "PSNR R: " << psnrR << endl;
		cout << "PSNR G: " << psnrG << endl;
		cout << "PSNR B: " << psnrB << endl;
	}
}
vector<vector<RGBQUADS>> BMP::getPixels()
{
	return pixels;
}
BITMAPINFOHEADER* BMP::getHeader()
{
	return bmpih;
}
double BMP::get_sqr_C(int value)
{
	if (value == 0)
	{
		return 1.0 / 8.0;
	}
	return 1.0 / 4.0;
}
int BMP::clip(int value)
{
	if (value < 0)
	{
		return 0;
	}
	if (value > 255)
	{
		return 255;
	}
	return value;
}
void BMP::DCT()
{
	transformToYCC();
	int N = 8;
	spectreY.resize(bmpih->biHeight);
	spectreCb.resize(bmpih->biHeight);
	spectreCr.resize(bmpih->biHeight);
	for (int i = 0; i < bmpih->biHeight; i++)
	{
		spectreY[i].resize(bmpih->biWidth);
		spectreCb[i].resize(bmpih->biWidth);
		spectreCr[i].resize(bmpih->biWidth);
	}
	for (int i = 0; i < bmpih->biHeight - N; i += N)
	{
		for (int j = 0; j < bmpih->biWidth - N; j += N)
		{
			for (int k = 0; k < N; k++)
			{
				for (int l = 0; l < N; l++)
				{
					spectreY[i + k][j + l] = 0;
					spectreCb[i + k][j + l] = 0;
					spectreCr[i + k][j + l] = 0;
					for (int x = i; x < i + N; x++)
					{
						for (int z = j; z < j + N; z++)
						{
							spectreY[i + k][j + l] += yccPixels[x][z].rgbRed * cos((2.0 * (x % 8) + 1.0) * PI * k / 16.0) * cos((2.0 * (z % 8) + 1.0) * PI * l / 16.0);
							spectreCb[i + k][j + l] += yccPixels[x][z].rgbGreen * cos((2.0 * (x % 8) + 1.0) * PI * k / 16.0) * cos((2.0 * (z % 8) + 1.0) * PI * l / 16.0);
							spectreCr[i + k][j + l] += yccPixels[x][z].rgbBlue * cos((2.0 * (x % 8) + 1.0) * PI * k / 16.0) * cos((2.0 * (z % 8) + 1.0) * PI * l / 16.0);
						}
					}
					double Ck = 0;
					if (k == 0)
					{
						Ck = 1 / (double)N;
					}
					else
					{
						Ck = 2 / (double)N;
					}
					double Cl = 0;
					if (l == 0)
					{
						Cl = 1 / (double)N;
					}
					else
					{
						Cl = 2 / (double)N;
					}
					spectreY[i + k][j + l] *= sqrt(Ck) * sqrt(Cl);
					spectreCb[i + k][j + l] *= sqrt(Ck) * sqrt(Cl);
					spectreCr[i + k][j + l] *= sqrt(Ck) * sqrt(Cl);
				}
			}
		}
	}
}
void BMP::IDCT()
{
	transformToYCC();
	int N = 8;
	for (int i = 0; i < bmpih->biHeight - N; i += N)
	{
		for (int j = 0; j < bmpih->biWidth - N; j += N)
		{
			for (int k = 0; k < N; k++)
			{
				for (int l = 0; l < N; l++)
				{
					yccPixels[i + k][j + l].rgbRed = 0;
					yccPixels[i + k][j + l].rgbGreen = 0;
					yccPixels[i + k][j + l].rgbBlue = 0;
					double valY = 0;
					double valCb = 0;
					double valCr = 0;
					for (int x = i; x < i + 8; x++)
					{
						for (int z = j; z < j + 8; z++)
						{
							valY += spectreY[x][z] * cos((2.0 * (x % 8) + 1.0) * PI * k / 16.0) * cos((2.0 * (z % 8) + 1.0) * PI * l / 16.0);
							valCb += spectreCb[x][z] * cos((2.0 * (x % 8) + 1.0) * PI * k / 16.0) * cos((2.0 * (z % 8) + 1.0) * PI * l / 16.0);
							valCr += spectreCr[x][z] * cos((2.0 * (x % 8) + 1.0) * PI * k / 16.0) * cos((2.0 * (z % 8) + 1.0) * PI * l / 16.0);
						}
					}
					double Ck = 0;
					if (k == 0)
					{
						Ck = 1 / (double)N;
					}
					else
					{
						Ck = 2 / (double)N;
					}
					double Cl = 0;
					if (l == 0)
					{
						Cl = 1 / (double)N;
					}
					else
					{
						Cl = 2 / (double)N;
					}
					valY *= sqrt(Ck) * sqrt(Cl);
					valCb *= sqrt(Ck) * sqrt(Cl);
					valCr *= sqrt(Ck) * sqrt(Cl);
					if (valY < 0)
					{
						valY = 0;
					}
					else if (valY > 255)
					{
						valY = 255;
					}
					if (valCb < 0)
					{
						valCb = 0;
					}
					else if (valCb > 255)
					{
						valCb = 255;
					}
					if (valCr < 0)
					{
						valCr = 0;
					}
					else if (valCr > 255)
					{
						valCr = 255;
					}
					yccPixels[i + k][j + l].rgbRed = valY;
					yccPixels[i + k][j + l].rgbGreen = valCb;
					yccPixels[i + k][j + l].rgbBlue = valCr;
				}
			}
		}
	}
	transformFromYCC();
}