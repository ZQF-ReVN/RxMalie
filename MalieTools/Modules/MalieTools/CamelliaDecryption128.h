#pragma once
#include <cstdint>

#include "CamelliaDecryption128_Key.h"


namespace MalieTools
{
	namespace CamelliaDecryption
	{
		void DecryptBlock(const uint32_t nOffset, const uint32_t* pKeyTable, uint8_t* pBlock);
	}
}
