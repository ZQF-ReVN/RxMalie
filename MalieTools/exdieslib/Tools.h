#pragma once
#include <string>


bool PutConsole(const wchar_t* pString);

void MakeDir(const char* pDirName);

std::string ConvertFileName(wchar_t* pFileName);

void WriteToFile(const char* pFileName, unsigned char* pBuffer, size_t szWrite);