#pragma once
#include "LIB_Struct.h"

#include <fstream>


class LIBP
{
private:
	bool m_bFlatten;

	FILE* m_fpPack;
	size_t m_nKeyIndex;
	size_t m_posBaseOffset;


	LIBPENTRY1* m_pEntries1;
	LIBPENTRY2* m_pEntries2;

public:
	LIBP(FILE* fpPack, size_t nKeyIndex, bool bFlatten);
	~LIBP();

private:
	void Extract(const std::string& strPreFix, size_t posEntry1Offset, size_t nEntryCount);
};

void LIBU_Extract(FILE* fpPack, size_t nKeyIndex, const std::string& strPrefix, size_t posOffset, size_t szDec, bool bFlatten);