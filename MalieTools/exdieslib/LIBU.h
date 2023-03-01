#pragma once
#include <fstream>


void LIBU_Extract(FILE* fpPack, size_t nKeyIndex, const std::string& strPrefix, size_t posOffset, size_t szDec, bool bFlatten);