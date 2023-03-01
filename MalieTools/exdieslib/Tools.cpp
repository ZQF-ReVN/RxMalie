#include "Tools.h"

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <direct.h>

bool PutConsole(const wchar_t* pString)
{
	static HANDLE hSTD_OUT = GetStdHandle(STD_OUTPUT_HANDLE);
	return WriteConsoleW(hSTD_OUT, pString, lstrlenW(pString), NULL, NULL);
}

std::string ConvertFileName(wchar_t* pFileName)
{
	char filename[4096] = { 0 };
	size_t i;
	wcstombs_s(&i, filename, 4096, pFileName, sizeof(filename));
	return filename;
}

void MakeDir(const char* pDirName)
{
	(void)_mkdir(pDirName);
}

void WriteToFile(const char* pFileName, unsigned char* pBuffer, size_t szWrite)
{
	std::fstream file(pFileName, std::ios::binary | std::ios::out);
	if (!file.is_open())
	{
		std::cerr << "Could not open " << pFileName << " (Permission denied)" << std::endl;
		exit(1);
	}

	file.write((const char*)pBuffer, szWrite);
	file.flush();
	file.close();
}