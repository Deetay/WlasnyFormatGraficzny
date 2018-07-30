#pragma once
#include <iostream>
#include <vector>
using namespace std;

void filter(char** pixel3, unsigned int height, unsigned int width);
void unFilter(char** pixel3, unsigned int height, unsigned int width);
vector<char> rleEncode(char** pixel3, unsigned int height, unsigned int width);
void rleDecode(char** pixel3, unsigned int height, unsigned int width, vector<char> code);


