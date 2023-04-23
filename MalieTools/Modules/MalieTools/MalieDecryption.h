#pragma once
#include "MalieDecryption_Key.h"


namespace MalieTools
{
	namespace MalieDecryption
	{
		uint32_t DecryptBlock(const uint32_t nOffset, const uint32_t* pKeyTable, uint8_t* pBlock);
	}
}
