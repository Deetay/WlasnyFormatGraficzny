#pragma once
#include "rle.h"
#include <vector>
using namespace std;


void filter(char** pixel3, unsigned int height, unsigned int width)
{
	for (unsigned int i = 0; i < height;i++)
	{
		for (unsigned int j = width-1;j>0 ;j--)
		{
			pixel3[i][j] = pixel3[i][j] - pixel3[i][j - 1];
		}
	}
}

void unFilter(char** pixel3, unsigned int height, unsigned int width)
{
	for (unsigned int i = 0; i < height;i++)
	{
		for (unsigned int j = 1; j< width; j++)
		{
			pixel3[i][j] = pixel3[i][j] + pixel3[i][j - 1];
		}
	}
}

vector<char> rleEncode(char** pixel3, unsigned int height, unsigned int width)
{
	vector<char> code;
	unsigned char cnt = 1;
	for (unsigned int i = 0; i < height;i++)
	{
		for (unsigned int j = 0;j < width;j++)
		{
			while (j != width - 1 && cnt < 254 && pixel3[i][j] == pixel3[i][j + 1])
			{
				cnt++;
				j++;
			}

			code.push_back(cnt);
			code.push_back(pixel3[i][j]);
			cnt = 1;
		}
	}
	return code;
}

void rleDecode(char** pixel3, unsigned int height, unsigned int width, vector<char> code)
{
	unsigned int rowNum = 0, i = 0, codeLength;
	unsigned char pixelCount;

	vector<char> decoded;
	codeLength = code.size();
	while (i < codeLength)
	{
		pixelCount = code[i];
		for (unsigned char j = 0; j < pixelCount; j++)
		{
			decoded.push_back(code[i + 1]);
		}
		i = i + 2;
	}
	i = 0;
	for (unsigned int j = 0; j < height; j++)
	{
		for (unsigned int k = 0; k < width;k++)
		{
			pixel3[j][k] = decoded[i];
			i++;

		}
	}
}