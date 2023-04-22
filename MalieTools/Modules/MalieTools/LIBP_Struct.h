#pragma once
#include <cstdint>


namespace MalieTools
{
	namespace LIBP
	{
		struct Header
		{
			uint8_t  aSignature[0x4];     // "LIBP5"
			uint32_t uiEleEntryCount;
			uint32_t uiResEntryCount;
			uint32_t uiUn0;
		};

		struct EleEntry
		{
			union
			{
				uint8_t  aFolderName[20];
				uint8_t  aResName[20];
			};

			uint32_t uiType;              // 0x0 -> Folder, 0x10000 -> Res
			uint32_t uiSequence;

			union
			{
				uint32_t uiSubCount;
				uint32_t uiSize;
			};
		};

		struct ResEntry
		{
			uint32_t uiResOffset;         // uiResOffset * alignment:0x400
		};
	}
}

