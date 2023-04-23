#pragma once
#include <string>
#include <vector>
#include <fstream>

#include "LIBP_Struct.h"

namespace MalieTools
{
	namespace Pack
	{
		struct LIBP_CusEntry
		{
			std::string msPath;
			std::uint32_t uiOffset;
			std::uint32_t uiSize;
		};

		class LIBP
		{
		private:
			std::wstring m_wsPack;
			std::vector<LIBP_EleEntry> m_vecEleIndex;
			std::vector<LIBP_ResEntry> m_vecResIndex;
			std::vector<LIBP_CusEntry> m_vecCusIndex;

		public:
			LIBP(const std::wstring wsPack) : m_wsPack(wsPack) {}
			bool LoadPackIndex();
			bool LoadCustIndex();

		private:

		};
	}
}