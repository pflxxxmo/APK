#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <bitset>
#include <string>

using namespace std;

string read(HANDLE);
bool write(HANDLE, string);
DWORD setBaudrate();



const string flag = "01111110";
const string stuffed = "011111110";

string encode(string);
string decode(string);
string packData(string);
string unpackData(string);