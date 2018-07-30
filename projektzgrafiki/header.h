#pragma once


 #pragma pack(1)
//BMP HEADERS
struct pixel
{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
};

struct BITMAPFILEHEADER {
	char bfType[2];           
	unsigned int   bfSize;         
	unsigned short bfReserved1;      
	unsigned short bfReserved2;     
	unsigned int   bfOffBits;    
};

struct BITMAPINFOHEADER {       
	unsigned int biSize;         
	unsigned int biWidth;
	unsigned int biHeight;         
	unsigned short biPlanes;          
	unsigned short biBitCount;        
	unsigned int biCompression;      
	unsigned int biSizeImage;         
	int			 biXPelsPerMeter;    
	int			 biYPelsPerMeter;     
	unsigned int biClrUsed;         
	unsigned int biClrImportant;      
};

//WWZ HEADERS

struct WWZFILEHEADER {
	char fileId[3] = { 'W','W','Z' };
	unsigned int fileSize;
	unsigned int colorOffset;
	unsigned int pixelOffset; 

};

struct WWZINFOHEADER {
	unsigned int imgWidth;
	unsigned int imgHeight;
	char paletteType;
};


#pragma pack()

template <class type>type ***createTwoDimArray(int rows, int columns)
{
	type*** t = nullptr;
	try {
		t = new type**[rows];
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
			t[i] = new type*[columns];
		}
		catch (bad_alloc)
		{
			cout << "Brak miejsca na utworzenie tablicy";
			getchar();
			exit(0);
		}
		for (int j = 0; j < columns; j++)
		{
			try {
				t[i][j] = new type;
			}
			catch (bad_alloc)
			{
				cout << "Brak miejsca na utworzenie tablicy";
				getchar();
				exit(0);
			}
		}
	}
	return t;
}

