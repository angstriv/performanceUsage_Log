#pragma once

#include <stdio.h>
#include <Windows.h>
#include<time.h>
#include <tchar.h>

#define DIV 1024
#define WIDTH 7
#define GB(x) (((x).HighPart << 2) + ((DWORD)(x).LowPart) / 1024.0 / 1024.0 / 1024.0)

#pragma warning(disable: 4996)   

#define true 1
typedef unsigned char U_CHAR;


int getSystemTime(char* buffer);
__int64 CompareFileTime2(FILETIME time1, FILETIME time2);
void GetCpuUseage();
double CpuUseage();