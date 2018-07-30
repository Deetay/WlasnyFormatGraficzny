#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "header.h"
#include "rle.h"
#include "mediancut.h"

using namespace std;

void getFileHeader(BITMAPFILEHEADER *header, ifstream& file)
{
	file.read((char*)header, 14);
}

void getInfoHeader(BITMAPINFOHEADER *header, ifstream& file)
{
	file.read((char*)header, 40);
}

pixel* getPixel(ifstream& file)
{
	pixel* read = new pixel;
	file.read((char*)read, 3);
	return read;
}

void getPixelArray(pixel*** pixArr, ifstream& file, unsigned int width, unsigned int height, unsigned int offset)
{
	int rowPadding = 4 - (width * 3) % 4;
	if (rowPadding == 4) rowPadding = 0; 
	file.seekg(offset, ios::beg);

	for (unsigned int i = 0; i < height;i++)
	{
		for (unsigned int j = 0;j < width;j++)
		{
			pixArr[i][j] = getPixel(file);
		}
		file.seekg(rowPadding, ios::cur); 
	}

}

void saveAsBmp(BITMAPFILEHEADER* fileHeader, BITMAPINFOHEADER* infoHeader, ofstream& outFile, pixel*** pixelArr)
{
	outFile.write((char*)fileHeader, 14);
	outFile.write((char*)infoHeader, 40);
	char padding[3] = { 0, 0, 0 };

	int rowPadding = 4 - (infoHeader->biWidth * 3) % 4;
	if (rowPadding == 4) rowPadding = 0;

	for (unsigned int i = 0; i < infoHeader->biHeight;i++)
	{
		for (unsigned int j = 0;j < infoHeader->biWidth;j++)
		{
			outFile.write((char*)pixelArr[i][j], 3);
		}
		outFile.write(padding, rowPadding); 
	}

	outFile.close();

}
char **createCharArray(int rows, int columns)
{
	char** t = nullptr;
	try {
		t = new char*[rows];
	}
	catch (bad_alloc)
	{
		cout << "Brak miejsca na utworzenie tablicy";
		getchar();
		exit(0);
	}
	for (int i = 0; i < rows; i++)
	{
		try {
			t[i] = new char[columns];
		}
		catch (bad_alloc)
		{
			cout << "Brak miejsca na utworzenie tablicy";
			getchar();
			exit(0);
		}
	}
	return t;
}
void zeroArray(char** pixel3, int rows, int cols)
{
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			pixel3[i][j] = 0;
		}
	}
}

void bitCut(pixel*** pixel24, char** pixel3, char paletteType, unsigned int height, unsigned int width)
{
	if (paletteType == '0') 
	{
		int pixelValue, arrIndex;
		unsigned char greyArray[8] = { 0, 36, 72, 108, 144, 181, 218, 255 };
		for (unsigned int i = 0; i < height; i++)
		{
			for (unsigned int j = 0; j < width; j++)
			{
				pixelValue = (0.299 * pixel24[i][j]->red + 0.587 * pixel24[i][j]->green + 0.144 * pixel24[i][j]->blue);
				int min = abs(greyArray[0] - pixelValue);
				arrIndex = 0;
				for (int k = 1; k < 8; k++)
				{
					if (abs(greyArray[k] - pixelValue) < min)
					{
						arrIndex = k;
						min = abs(greyArray[k] - pixelValue);
					}

				}
				pixel3[i][j] = arrIndex;
			}
		}
	}
	else 
		for (unsigned int i = 0; i < height; i++)
		{
			for (unsigned int j = 0; j < width; j++)
			{
				if (pixel24[i][j]->blue & 0x80) pixel3[i][j] = pixel3[i][j] | 1;
				if (pixel24[i][j]->green & 0x80) pixel3[i][j] = pixel3[i][j] | 2;
				if (pixel24[i][j]->red & 0x80) pixel3[i][j] = pixel3[i][j] | 4;
			}
		}

}

void reassignColor(pixel*** pixel24, char** pixel3, char paletteType, unsigned int height, unsigned int width)
{
	if (paletteType == '0') 
	{
		for (unsigned int i = 0; i < height; i++)
		{
			for (unsigned int j = 0; j < width; j++)
			{
				switch (pixel3[i][j])
				{
				case 0:
					pixel24[i][j]->blue = 0;
					pixel24[i][j]->green = 0;
					pixel24[i][j]->red = 0;
					break;
				case 1:
					pixel24[i][j]->blue = 0x24;
					pixel24[i][j]->green = 0x24;
					pixel24[i][j]->red = 0x24;
					break;
				case 2:
					pixel24[i][j]->blue = 0x48;
					pixel24[i][j]->green = 0x48;
					pixel24[i][j]->red = 0x48;
					break;
				case 3:
					pixel24[i][j]->blue = 0x6c;
					pixel24[i][j]->green = 0x6c;
					pixel24[i][j]->red = 0x6c;
					break;
				case 4:
					pixel24[i][j]->blue = 0x90;
					pixel24[i][j]->green = 0x90;
					pixel24[i][j]->red = 0x90;
					break;
				case 5:
					pixel24[i][j]->blue = 0xb5;
					pixel24[i][j]->green = 0xb5;
					pixel24[i][j]->red = 0xb5;
					break;
				case 6:
					pixel24[i][j]->blue = 0xda;
					pixel24[i][j]->green = 0xda;
					pixel24[i][j]->red = 0xda;
					break;
				case 7:
					pixel24[i][j]->blue = 0xFF;
					pixel24[i][j]->green = 0xFF;
					pixel24[i][j]->red = 0xFF;
					break;
				default:
					break;
				}
			}
		}
	}
	else
		for (unsigned int i = 0; i < height; i++)
		{
			for (unsigned int j = 0; j < width; j++)
			{
				if (pixel3[i][j] & 1) pixel24[i][j]->blue = 0xFF;
				else pixel24[i][j]->blue = 0;
				if (pixel3[i][j] & 2) pixel24[i][j]->green = 0xFF;
				else pixel24[i][j]->green = 0;
				if (pixel3[i][j] & 4) pixel24[i][j]->red = 0xFF;
				else pixel24[i][j]->red = 0;

			}
		}

}
void saveAsWwz(char paletteType, unsigned int height, unsigned int width, vector<char> codedArray, pixel** colorsArray, string rawBmpFileName)
{
	rawBmpFileName = rawBmpFileName + ".wwz";
	ofstream outFile(rawBmpFileName, ios::out | ios::binary);
	WWZFILEHEADER *fileHeader = new WWZFILEHEADER;
	WWZINFOHEADER *infoHeader = new WWZINFOHEADER;
	if (paletteType == '2')
	{
		fileHeader->fileSize = 48 + width*height;
		fileHeader->colorOffset = 24;
		fileHeader->pixelOffset = 48;
		infoHeader->imgHeight = height;
		infoHeader->imgWidth = width;
		infoHeader->paletteType = paletteType;

		outFile.write((char*)fileHeader, 15);
		outFile.write((char*)infoHeader, 9);
		for (int i = 0;i < 8;i++)
		{
			outFile.write((char*)colorsArray[i], 3);
		}
		outFile.write(&codedArray[0], codedArray.size());

	}
	else
	{
		fileHeader->fileSize = 24 + width * height;
		fileHeader->colorOffset = 0;
		fileHeader->pixelOffset = 24;
		infoHeader->imgHeight = height;
		infoHeader->imgWidth = width;
		infoHeader->paletteType = paletteType;

		outFile.write((char*)fileHeader, 15);
		outFile.write((char*)infoHeader, 9);
		outFile.write(&codedArray[0], codedArray.size());
	}
	delete fileHeader;
	delete infoHeader;
	outFile.close();
}

vector<char> openWwz(WWZFILEHEADER *fileHeader, WWZINFOHEADER *infoHeader, pixel** colorsArray, string fileName)
{
	fileName += ".wwz";
	ifstream inFile(fileName, ios::out | ios::binary);
	inFile.read((char*)fileHeader, 15);
	inFile.read((char*)infoHeader, 9);
	vector<char> codedArray;
	if (infoHeader->paletteType == '2')
	{
		
		for (int i = 0;i < 8;i++)
		{
			colorsArray[i] = getPixel(inFile);
		}
		
	}
	char inputChar;
	while (inFile.read(&inputChar, 1)) 
	{
		codedArray.push_back(inputChar);
	}
	return codedArray;
}



int main()
{
	BITMAPFILEHEADER  *fileHeader = new BITMAPFILEHEADER;
	BITMAPINFOHEADER  *infoHeader = new BITMAPINFOHEADER;

	pixel*** pixelArr24bit = nullptr;
	char** pixelArr3bit = nullptr;
	pixel** colorsArray = nullptr;

	int method=0;
	cout << "Konwersja z formatu BMP do nowego formatu wwz[1]" << endl;
	cout << "Konwersja z nowego formatu wwz do formatu BMP[2]" << endl;
	cin >> method;

	if (method == 1)
	{
		char palleteType;
		cout << "Wybierz metode: " << endl;
		cout << "[0] 8 stopniowa skala szarosci " << endl;
		cout << "[1] 8 narzuconych barw " << endl;
		cout << "[2] 8 dedykowanych barw " << endl;
		cin >> palleteType;


		string bmpFileName = "", rawBmpFileName;
		cout << "\nPodaj nazwe pliku do konwersji, bez rozszerzenia .bmp: ";
		cin >> rawBmpFileName;
		bmpFileName = rawBmpFileName + ".bmp";


		ifstream inFile(bmpFileName, ios::in | ios::binary);

		getFileHeader(fileHeader, inFile);
		getInfoHeader(infoHeader, inFile);
		if (palleteType == '2')
		{
			pixelArr24bit = createTwoDimArray<pixel>(infoHeader->biHeight, infoHeader->biWidth);
			pixelArr3bit = createCharArray(infoHeader->biHeight, infoHeader->biWidth);
			colorsArray = createColorArray<pixel>(8);
			zeroArray(pixelArr3bit, infoHeader->biHeight, infoHeader->biWidth);

			getPixelArray(pixelArr24bit, inFile, infoHeader->biWidth, infoHeader->biHeight, fileHeader->bfOffBits);
			medianCut(pixelArr24bit, colorsArray, infoHeader->biHeight, infoHeader->biWidth);
			replaceColors(pixelArr24bit, colorsArray, pixelArr3bit, infoHeader->biHeight, infoHeader->biWidth);

			filter(pixelArr3bit, infoHeader->biHeight, infoHeader->biWidth); 
			vector<char> rleEncoded;

			rleEncoded = rleEncode(pixelArr3bit, infoHeader->biHeight, infoHeader->biWidth);
			saveAsWwz(palleteType, infoHeader->biHeight, infoHeader->biWidth, rleEncoded, colorsArray, rawBmpFileName);	
			cout << "Zakonczono pomyslnie, plik " << bmpFileName << " zostal skonwertowany do " << rawBmpFileName << ".wwz" << endl;
		}
		else
		{

			pixelArr24bit = createTwoDimArray<pixel>(infoHeader->biHeight, infoHeader->biWidth);
			pixelArr3bit = createCharArray(infoHeader->biHeight, infoHeader->biWidth);
			zeroArray(pixelArr3bit, infoHeader->biHeight, infoHeader->biWidth);

			getPixelArray(pixelArr24bit, inFile, infoHeader->biWidth, infoHeader->biHeight, fileHeader->bfOffBits);
			bitCut(pixelArr24bit, pixelArr3bit, palleteType, infoHeader->biHeight, infoHeader->biWidth);

			filter(pixelArr3bit, infoHeader->biHeight, infoHeader->biWidth);

			vector<char> rleEncoded;

			rleEncoded = rleEncode(pixelArr3bit, infoHeader->biHeight, infoHeader->biWidth);
			saveAsWwz(palleteType, infoHeader->biHeight, infoHeader->biWidth, rleEncoded, colorsArray, rawBmpFileName);	
			cout << "Zakonczono pomyslnie, plik " << bmpFileName << " zostal skonwertowany do " << rawBmpFileName << ".wwz" << endl;
		}
	}
	if (method == 2)
	{
		string wwzFileName = "", rawWwzFileName;
		cout << "Podaj nazwe pliku do zdekodowania, bez rozszerzenia .wwz: ";
		cin >> rawWwzFileName;
		wwzFileName = rawWwzFileName + "Decoded.bmp";

		ofstream outFile(wwzFileName, ios::out | ios::binary);


		vector<char>  gotCodedArr;
		WWZFILEHEADER *WfileHeader = new WWZFILEHEADER;
		WWZINFOHEADER *WinfoHeader = new WWZINFOHEADER;

		colorsArray = createColorArray<pixel>(8);
		gotCodedArr = openWwz(WfileHeader, WinfoHeader, colorsArray, rawWwzFileName);
		pixelArr24bit = createTwoDimArray<pixel>(WinfoHeader->imgHeight, WinfoHeader->imgWidth);
		pixelArr3bit = createCharArray(WinfoHeader->imgHeight, WinfoHeader->imgWidth);
		
		zeroArray(pixelArr3bit, WinfoHeader->imgHeight, WinfoHeader->imgWidth);

		rleDecode(pixelArr3bit, WinfoHeader->imgHeight, WinfoHeader->imgWidth, gotCodedArr); 
		unFilter(pixelArr3bit, WinfoHeader->imgHeight, WinfoHeader->imgWidth);
		if (WinfoHeader->paletteType == '2')
		{
			
			reassignColorsDedicated(pixelArr24bit, colorsArray, pixelArr3bit, WinfoHeader->imgHeight, WinfoHeader->imgWidth);
		}
		else
		{
			reassignColor(pixelArr24bit, pixelArr3bit, WinfoHeader->paletteType, WinfoHeader->imgHeight, WinfoHeader->imgWidth);
		}
		fileHeader->bfOffBits = 54;
		fileHeader->bfReserved1 = 0;
		fileHeader->bfReserved2 = 0;
		fileHeader->bfSize = WfileHeader->fileSize;
		fileHeader->bfType[0] = 'B';
		fileHeader->bfType[1] = 'M';

		infoHeader->biSize = 40;
		infoHeader->biWidth = WinfoHeader->imgWidth;
		infoHeader->biHeight = WinfoHeader->imgHeight;
		infoHeader->biPlanes = 1;
		infoHeader->biCompression = 0;
		infoHeader->biClrUsed = 0;
		infoHeader->biXPelsPerMeter = 0;
		infoHeader->biYPelsPerMeter = 0;
		infoHeader->biBitCount = 24;
		infoHeader->biClrImportant = 0;

		int rowPadding = 4 - (infoHeader->biWidth * 3) % 4;
		if (rowPadding == 4) rowPadding = 0;

		infoHeader->biSizeImage = infoHeader->biWidth * infoHeader->biHeight * 3 + (rowPadding* infoHeader->biHeight);

		saveAsBmp(fileHeader, infoHeader, outFile, pixelArr24bit);
		cout << "Zakonczono pomyslnie, plik " << rawWwzFileName << ".wwz zostal odkodowany do " << wwzFileName << endl;
	}

	
	getchar();
	getchar();
	return 0;
}