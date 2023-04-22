#pragma once
#include <string>
#include <vector>
#include <fstream>

#include "LIBP_Struct.h"

namespace MalieTools
{
	namespace LIBP
	{
		struct CusEntry
		{
			std::string msPath;
			uint32_t uiOffset;
			uint32_t uiSize;
		};

		class Editor
		{
		private:
			std::wstring m_wsPack;
			std::vector<EleEntry> m_vecEleIndex;
			std::vector<ResEntry> m_vecResIndex;
			std::vector<CusEntry> m_vecCusIndex;
		public:
			Editor(const std::wstring wsPack) : m_wsPack(wsPack) {}
			bool LoadPackIndex();
			bool LoadCustIndex();
		private:

		};
	}
}