#ifndef CONVERTTOBMP_H
#define CONVERTTOBMP_H

#include <iostream>
#include <ctime>
#include <vector>
#include <fstream>
#include <stdint.h>
#include <sstream>

using namespace std;
void ConvertToBMP(vector < vector < char> > &MapArray, int Height, int Width, int Seed);
void GetColors (unsigned char &R, unsigned char &G, unsigned char &B, char x);

#endif