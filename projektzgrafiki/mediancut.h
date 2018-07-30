#pragma once
#include <iostream>
#include "header.h"

using namespace std;


template <class type>type ** createColorArray(int rows)
{
	type **t = nullptr;

	try {
		t = new type*[rows];
	}
	catch (bad_alloc)
	{
		cout << "Brak miejsca na tablice";
		exit(0);
	}
	return t;
}
/**
* Funkcja wylicza wartosc dla piksela na podstawie wszystkich pikseli w kubelku
*
* @param pixels		vector wskaznikow na strukture pixel
* @return			wskaznik na strukture zawierajaca kolor piksela wyliczony na podstawie sredniej z kubelka 
*/
pixel* getAvgColor(vector<pixel*> pixels);


/**
* Algorytm median-cut wyznacza 8 kolorow na podstawie wsyzstkich pikseli pobranych z grafiki
*
* @param pixel24		tablica o wymiarach height*width zawierajaca w strukturze 'pixel' wartosci wszystkich pikseli z grafiki
* @param colorsArray	tablica wskaznikow na strukture pixel, do ktorej zapiszemy kolory wyznaczone przez algorytm median cut
* @param height			wysokosc grafiki
* @param width			szerokosc grafiki
*/
void medianCut(pixel*** pixel24, pixel** colorsArray, unsigned int height, unsigned int width);


/**
* Funkcja na podstawie sredniej z wartosci RGB kazdego piksela zapisanego na 24bitach wyznacza odpowiadajaca mu wartosc (indeks koloru) z palety dedykowanej,
* nastepnie zapisuje ten indeks do tablicy pixel3
* @param pixel24		tablica o wymiarach height*width zawierajaca w strukturze 'pixel' wartosci wszystkich pikseli z grafiki
* @param colorsArray	tablica wskaznikow na strukture pixel, zawierajaca dedykowana palete
* @param pixel3			tablica pikseli o wymiarach height*width, w ktorej kazdy piksel jest indeksem palety dedykowanej
* @param height			wysokosc grafiki
* @param width			szerokosc grafiki
*/
void replaceColors(pixel*** pixel24, pixel** colorsArray,char** pixel3, unsigned int height, unsigned int width);


/**
* Funkcja przypisuje kazdemu pikselowi kolor z palety dedykowanej na podstawie jego indeksu z tablicy pikseli
* @param pixel24		tablica pikseli o wymiarach height*width, do ktorej zostanie zapisana wartosc kazdego piksela na podstawie indeksu z tablicy pixel3
* @param colorsArray	tablica wskaznikow na strukture pixel, zawierajaca dedykowana palete
* @param pixel3			tablica pikseli o wymiarach height*width, w ktorej kazdy piksel jest indeksem palety dedykowanej
* @param height			wysokosc grafiki
* @param width			szerokosc grafiki
*/
void reassignColorsDedicated(pixel*** pixel24, pixel** colorsArray, char** pixel3, unsigned int height, unsigned int width);
