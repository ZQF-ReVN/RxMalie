#include "MalieEncryption.h"
#include <stdlib.h>


namespace MalieTools
{
	namespace MalieEncryption
	{
		inline uint32_t __ROR4__(uint32_t value, int32_t count) { return _rotl(value, -count); }
		inline uint32_t __ROL4__(uint32_t value, int32_t count) { return _rotl(value, count); }

		static uint8_t g_aRotateTable[32] =
		{
			0xA4,0xA7,0xA6,0xA1,0xA0,0xA3,0xA2,0xAC,
			0xAF,0xAE,0xA9,0xA8,0xAB,0xAA,0xB4,0xB7,
			0xB6,0xB1,0xB0,0xB3,0xB2,0xBC,0xBF,0xBE,
			0xB9,0xB8,0xBB,0xBA,0xA1,0xA9,0xB1,0xB9,
		};

		uint32_t DecryptBlock(const uint32_t nOffset, const uint32_t* pKeyTable, uint8_t* pBlock)
		{
			//Round 1
			uint8_t szBlock = nOffset & 0xF;
			uint8_t xorKey = pBlock[szBlock];
			for (uint8_t iteBlock = 0; iteBlock < 0x10; iteBlock++)
			{
				uint8_t read = pBlock[iteBlock];
				if (szBlock != iteBlock) read ^= xorKey;
				pBlock[iteBlock] = read;
			}

			//Round 2
			uint32_t* pDecBuffer = (uint32_t*)pBlock;
			uint8_t   shrOffset  = nOffset >> 4;
			pDecBuffer[0] = __ROR4__
			(
				pDecBuffer[0] ^ __ROR4__(pKeyTable[0], g_aRotateTable[(shrOffset + 3 * 0) & 0x1F] ^ 0xA5),
				g_aRotateTable[(shrOffset + 0x0C + 0x3 * 0x0) & 0x1F] ^ 0xA5
			);

			pDecBuffer[1] = __ROL4__
			(
				pDecBuffer[1] ^ __ROL4__(pKeyTable[1], g_aRotateTable[(shrOffset + 3 * 1) & 0x1F] ^ 0xA5),
				g_aRotateTable[(shrOffset + 0x0C + 0x3 * 0x1) & 0x1F] ^ 0xA5
			);

			pDecBuffer[2] = __ROR4__
			(
				pDecBuffer[2] ^ __ROR4__(pKeyTable[2], g_aRotateTable[(shrOffset + 3 * 2) & 0x1F] ^ 0xA5),
				g_aRotateTable[(shrOffset + 0x0C + 0xE0 + 0x3 * 0x2) & 0x1F] ^ 0xA5
			);

			pDecBuffer[3] = __ROL4__
			(
				pDecBuffer[3] ^ __ROL4__(pKeyTable[3], g_aRotateTable[(shrOffset + 3 * 3) & 0x1F] ^ 0xA5),
				g_aRotateTable[(shrOffset + 0x0C + 0xE0 + 0x3 * 0x3) & 0x1F] ^ 0xA5
			);

			return pDecBuffer[3];
		}
	}
}
