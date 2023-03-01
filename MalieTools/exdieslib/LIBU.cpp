#include "LIBU.h"
#include "Tools.h"
#include "LIB_Decoder.h"
#include "LIB_Struct.h"


void LIBU_Extract(FILE* fpPack, size_t nKeyIndex, const std::string& strPrefix, size_t posOffset, size_t szDec, bool bFlatten)
{
	char dir_delim = bFlatten && strPrefix != "." ? '+' : '/';

	LIBUHDR header;
	LIB_Decoder(fpPack, nKeyIndex, &header, sizeof(header), posOffset);

	if (memcmp(header.signature, "LIB", 3) == 0)
	{
		LIBUENTRY* entries = new LIBUENTRY[header.entry_count];
		LIB_Decoder(fpPack, nKeyIndex, entries, sizeof(LIBUENTRY) * header.entry_count, posOffset + sizeof(header));

		if (!bFlatten) MakeDir(strPrefix.c_str());

		for (unsigned long iteEntry = 0; iteEntry < header.entry_count; iteEntry++)
		{

#ifdef UNICODE_ENTRIES
			std::string filename = ConvertFileName(entries[iteEntry].filename);
#else
			string filename = entries[i].filename;
#endif

			LIBU_Extract(fpPack, nKeyIndex, strPrefix + dir_delim + filename, posOffset + entries[iteEntry].offset, entries[iteEntry].length, bFlatten);
		}

		delete[] entries;
	}
	else
	{
		unsigned char* decBuffer = new unsigned char[szDec];
		LIB_Decoder(fpPack, nKeyIndex, decBuffer, szDec, posOffset);
		WriteToFile(strPrefix.c_str(), decBuffer, szDec);
		delete[] decBuffer;
	}
}