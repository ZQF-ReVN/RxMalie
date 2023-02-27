#include "DecBlock_DIRA.h"
#include "defs.h"

static unsigned char aStaticTable[32] =
{
	0xa4,0xa7,0xa6,0xa1,0xa0,0xa3,0xa2,0xac,
	0xaf,0xae,0xa9,0xa8,0xab,0xaa,0xb4,0xb7,
	0xb6,0xb1,0xb0,0xb3,0xb2,0xbc,0xbf,0xbe,
	0xb9,0xb8,0xbb,0xba,0xa1,0xa9,0xb1,0xb9,
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