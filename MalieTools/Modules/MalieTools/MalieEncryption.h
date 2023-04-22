#pragma once
#include "MalieEncryption_Key.h"


namespace MalieTools
{
	namespace MalieEncryption
	{
		uint32_t DecryptBlock(const uint32_t nOffset, const uint32_t* pKeyTable, uint8_t* pBlock);
	}
}
