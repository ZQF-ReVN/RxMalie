#pragma once
#include <fstream>
#include <string>

#include "MalieDecryption.h"
#include "CamelliaDecryption128.h"
#include "../../ThirdParty/TDA/AutoBuffer.h"


namespace MalieTools
{
	namespace Pack
	{
		class Decoder
		{
		private:
			const uint32_t  m_nType;
			const uint32_t* m_pKeyTable;
			std::ifstream m_ifsPack;
			TDA::AutoBuffer m_Buffer;

		public:
			Decoder(const uint32_t nType, const uint32_t* pKeyTable) :m_nType(nType), m_pKeyTable(pKeyTable) {}

			bool DecodePack(std::wstring wsPack, uint32_t nSizeBuffer);

			void DecodeBlock(uint32_t nOffset, uint8_t* pBlock);
			bool DecodeBuffer(uint32_t& nOffset, uint8_t* pBuffer, uint32_t nSizeBuffer);

			bool OpenPack(const std::wstring& wsPack);
			uint8_t* DecodeBuffer(uint32_t nOffset, uint32_t nSize);

		private:
			bool InitPack(const std::wstring& wsPack, std::ifstream& ref_IStream, std::ofstream& ref_OStream);

		};
	}
}
