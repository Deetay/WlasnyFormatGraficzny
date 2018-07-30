#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include "mediancut.h"

struct compareByRed
{
	bool operator()(pixel* const & pixel1, pixel* const & pixel2)
	{
		return pixel1->red < pixel2->red;
	}
};
struct compareByGreen
{
	bool operator()(pixel* const & pixel1, pixel* const & pixel2)
	{
		return pixel1->green < pixel2->green;
	}
};
struct compareByBlue
{
	bool operator()(pixel* const & pixel1, pixel* const & pixel2)
	{
		return pixel1->blue < pixel2->blue;
	}
};

using namespace std;

pixel* getAvgColor(vector<pixel*> pixels)
{

	double avgRed=0, avgGreen=0, avgBlue=0;

	for (unsigned int i = 0; i < pixels.size(); i++)
	{
		avgRed += pixels[i]->red;
		avgGreen += pixels[i]->green;
		avgBlue += pixels[i]->blue;
	}

	pixel *color = new pixel;
	color->red = avgRed / pixels.size();
	color->green = avgGreen / pixels.size();
	color->blue = avgBlue / pixels.size();

	return color;
}



void medianCut(pixel *** pixel24, pixel ** colorsArray, unsigned int height, unsigned int width)
{
	vector<pixel*> bucket;
	unsigned char redRange, greenRange, blueRange, range;
	unsigned char redMin = pixel24[0][0]->red, greenMin = pixel24[0][0]->green, blueMin = pixel24[0][0]->blue;
	unsigned char redMax = 0, greenMax = 0, blueMax = 0;
	
	for (unsigned int j = 0; j < height; j++)
	{
		for (unsigned int k = 0; k < width;k++)
		{
			redMin = min(redMin, pixel24[j][k]->red);
			greenMin = min(greenMin, pixel24[j][k]->green);
			blueMin = min(blueMin, pixel24[j][k]->blue);
			redMax = max(redMax, pixel24[j][k]->red);
			greenMax = max(greenMax, pixel24[j][k]->green);
			blueMax = max(blueMax, pixel24[j][k]->blue);
			
			bucket.push_back(pixel24[j][k]);
			
		}
	}
	redRange = redMax - redMin;
	greenRange = greenMax - greenMin;
	blueRange = blueMax - blueMin;
	range = max(max(redRange, greenRange), blueRange);
	if (range == redRange)
		sort(bucket.begin(), bucket.end(),compareByRed());
	else if (range == greenRange)
		sort(bucket.begin(), bucket.end(), compareByGreen());
	else if (range == blueRange)
		sort(bucket.begin(), bucket.end(), compareByBlue());
	size_t chunkSize = bucket.size() / 8;

	for (int i = 0;i < 8;i++)
	{
		vector<pixel*> chunk(bucket.begin()+chunkSize*i, bucket.begin() + chunkSize*i+chunkSize);
		colorsArray[i] = getAvgColor(chunk);
	}

}

void replaceColors(pixel*** pixel24, pixel** colorsArray,char** pixel3, unsigned int height, unsigned int width)
{

	int pixelValue, arrIndex;
	int avgCol[8] = {0};

	for (int i = 0;i < 8;i++)
	{
		avgCol[i] = (colorsArray[i]->blue + colorsArray[i]->red + colorsArray[i]->green) / 3;
	}
	for (unsigned int i = 0; i < height;i++)
	{
		for (unsigned int j = 0;j < width;j++)
		{
			pixelValue = static_cast<int>(pixel24[i][j]->red + pixel24[i][j]->green + pixel24[i][j]->blue) / 3;
			int min = abs(avgCol[0] - pixelValue);
			arrIndex = 0;
			for (int k = 1;k < 8;k++)
			{
				if (abs(avgCol[k] - pixelValue) < min)
				{
					arrIndex = k;
					min = abs(avgCol[k] - pixelValue);
				}
			}
			pixel3[i][j] = arrIndex;
		}
	}

}
void reassignColorsDedicated(pixel*** pixel24, pixel** colorsArray, char** pixel3, unsigned int height, unsigned int width)
{
	for (unsigned int i = 0; i < height;i++)
	{
		for (unsigned int j = 0;j < width;j++)
		{
			pixel24[i][j] = colorsArray[pixel3[i][j]];
		}
	}
}