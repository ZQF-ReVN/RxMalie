#include "Decoder.h"

#include "../../ThirdParty/TDA/FileX.h"


namespace MalieTools
{
	namespace Decoder
	{
		const static size_t ALIGN_SIZE = 0x400;
		const static size_t BLOCK_SIZE = 0x10;

		bool Pack_Decodeer::InitPack(const std::wstring& wsPack, std::ifstream& ref_IStream, std::ofstream& ref_OStream)
		{
			ref_IStream.open(wsPack, std::ios::binary);
			if (!ref_IStream.is_open()) { return false; }

			ref_OStream.open(wsPack + L".dec", std::ios::binary);
			if (!ref_OStream.is_open()) { return false; }

			return true;
		}

		bool Pack_Decodeer::DecodePack(std::wstring wsPack, uint32_t nSizeBuffer)
		{
			uint8_t* pBuffer = m_Buffer.ReSize(nSizeBuffer);

			std::ifstream istream;
			std::ofstream ostream;
			InitPack(wsPack, istream, ostream);

			uint32_t pack_size    = static_cast<uint32_t>(TDA::FileX::GetFileSize(istream));
			uint32_t pack_offset  = 0;
			uint32_t remain_size  = pack_size % nSizeBuffer;
			uint32_t buffer_count = (pack_size - remain_size) / nSizeBuffer;

			for (size_t iteBuffer = 0; iteBuffer < buffer_count; iteBuffer++)
			{
				istream.read((char*)pBuffer, nSizeBuffer);
				DecodeBuffer(pack_offset, pBuffer, nSizeBuffer);
				ostream.write((char*)pBuffer, nSizeBuffer);
			}

			if (remain_size != 0)
			{
				istream.read((char*)pBuffer, remain_size);
				DecodeBuffer(pack_offset, pBuffer, remain_size);
				ostream.write((char*)pBuffer, remain_size);
			}

			return true;
		}

		void Pack_Decodeer::DecodeBlock(uint32_t nOffset, uint8_t* pBlock)
		{
			switch (m_nType)
			{
			case 0: { CamelliaDecryption::DecryptBlock(nOffset, m_pKeyTable, pBlock); }  break;
			case 1: { MalieEncryption::DecryptBlock(nOffset, m_pKeyTable, pBlock); }    break;
			}
		}

		bool Pack_Decodeer::DecodeBuffer(uint32_t& nOffset, uint8_t* pBuffer, uint32_t nBufferSize)
		{
			if (nBufferSize % BLOCK_SIZE) { return false; }

			for (size_t iteBlock = 0; iteBlock < (nBufferSize / BLOCK_SIZE); iteBlock++)
			{
				DecodeBlock(nOffset, (uint8_t*)pBuffer + BLOCK_SIZE * iteBlock);
				nOffset += BLOCK_SIZE;
			}

			return true;
		}

		bool Pack_Decodeer::OpenPack(const std::wstring& wsPack)
		{
			m_ifsPack.open(wsPack, std::ios::binary);
			if (!m_ifsPack.is_open()) { return false; }

			return true;
		}

		uint8_t* Pack_Decodeer::DecodeBuffer(uint32_t nOffset, uint32_t nSize)
		{
			uint32_t remain_size = nSize % ALIGN_SIZE;
			uint32_t align_size = nSize - remain_size + ALIGN_SIZE;

			m_ifsPack.seekg(nOffset, std::ios::beg);

			uint8_t* pBuffer = m_Buffer.ReSize(align_size);
			m_ifsPack.read((char*)pBuffer, align_size);

			DecodeBuffer(nOffset, pBuffer, align_size);

			return pBuffer;
		}
	}
}

