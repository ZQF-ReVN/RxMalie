#include "MalieEncryption.h"
#include "defs.h"

static unsigned char aStaticTable[32] =
{
	0xA4,0xA7,0xA6,0xA1,0xA0,0xA3,0xA2,0xAC,
	0xAF,0xAE,0xA9,0xA8,0xAB,0xAA,0xB4,0xB7,
	0xB6,0xB1,0xB0,0xB3,0xB2,0xBC,0xBF,0xBE,
	0xB9,0xB8,0xBB,0xBA,0xA1,0xA9,0xB1,0xB9,
};

unsigned int DecBlock_DIRA(unsigned int nOffset, unsigned char* pEncBuffer , unsigned int* pKeyTable)
{
	//Round 1
	unsigned char szBlock = nOffset & 0xF;
	unsigned char xorKey = pEncBuffer[szBlock];
	for (unsigned char iteBlock = 0; iteBlock < 0x10; iteBlock++)
	{
		unsigned char read = pEncBuffer[iteBlock];
		if (szBlock != iteBlock) read ^= xorKey;
		pEncBuffer[iteBlock] = read;
	}

	//Round 2
	unsigned int* pDecBuffer = (unsigned int*)pEncBuffer;
	unsigned char shrOffset = nOffset >> 4;
	pDecBuffer[0] = __ROR4__
	(
		pDecBuffer[0] ^ __ROR4__(pKeyTable[0], aStaticTable[(shrOffset + 3 * 0) & 0x1F] ^ 0xA5),
		aStaticTable[(shrOffset + 0x0C + 0x3 * 0x0) & 0x1F] ^ 0xA5
	);

	pDecBuffer[1] = __ROL4__
	(
		pDecBuffer[1] ^ __ROL4__(pKeyTable[1], aStaticTable[(shrOffset + 3 * 1) & 0x1F] ^ 0xA5),
		aStaticTable[(shrOffset + 0x0C + 0x3 * 0x1) & 0x1F] ^ 0xA5
	);

	pDecBuffer[2] = __ROR4__
	(
		pDecBuffer[2] ^ __ROR4__(pKeyTable[2], aStaticTable[(shrOffset + 3 * 2) & 0x1F] ^ 0xA5),
		aStaticTable[(shrOffset + 0x0C + 0xE0 + 0x3 * 0x2) & 0x1F] ^ 0xA5
	);

	pDecBuffer[3] = __ROL4__
	(
		pDecBuffer[3] ^ __ROL4__(pKeyTable[3], aStaticTable[(shrOffset + 3 * 3) & 0x1F] ^ 0xA5),
		aStaticTable[(shrOffset + 0x0C + 0xE0 + 0x3 * 0x3) & 0x1F] ^ 0xA5
	);

	return pDecBuffer[3];
}
