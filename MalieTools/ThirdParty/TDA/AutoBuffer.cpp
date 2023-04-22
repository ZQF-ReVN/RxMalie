#include "AutoBuffer.h"
#include "FileX.h"

#include <fstream>

namespace TDA
{
	AutoBuffer::AutoBuffer() : m_pBuffer(nullptr), m_szMaxAlloc(0)
	{

	}

	AutoBuffer::AutoBuffer(const AutoBuffer& buffer)
	{
		if (buffer.m_pBuffer != nullptr)
		{
			this->m_szMaxAlloc = buffer.m_szMaxAlloc;
			this->m_pBuffer = new uint8_t[buffer.m_szMaxAlloc];
			memcpy(m_pBuffer, buffer.m_pBuffer, m_szMaxAlloc);
		}
		else
		{
			this->m_pBuffer = nullptr;
			this->m_szMaxAlloc = 0;
		}
	}

	AutoBuffer::AutoBuffer(AutoBuffer&& buffer) noexcept
	{
		this->m_pBuffer = buffer.m_pBuffer;
		this->m_szMaxAlloc = buffer.m_szMaxAlloc;

		buffer.m_pBuffer = nullptr;
		buffer.m_szMaxAlloc = 0;
	}

	AutoBuffer::AutoBuffer(const std::wstring& wsFile) : m_pBuffer(nullptr), m_szMaxAlloc(0)
	{
		LoadFile(wsFile);
	}

	AutoBuffer::AutoBuffer(const std::wstring& wsFile, size_t szFile) : m_pBuffer(nullptr), m_szMaxAlloc(0)
	{
		LoadFile(wsFile, szFile);
	}

	AutoBuffer::~AutoBuffer()
	{
		Clear();
	}

	uint8_t* AutoBuffer::ReSize(size_t szRes)
	{
		if (!m_szMaxAlloc)
		{
			m_pBuffer = new uint8_t[szRes];
			m_szMaxAlloc = szRes;
		}

		if (szRes > m_szMaxAlloc)
		{
			delete[] m_pBuffer;
			m_pBuffer = new uint8_t[szRes];
			m_szMaxAlloc = szRes;
		}

		return m_pBuffer;
	}

	void AutoBuffer::Clear()
	{
		m_szMaxAlloc = 0;

		if (m_pBuffer)
		{
			delete[] m_pBuffer;
		}

		m_pBuffer = nullptr;
	}

	uint8_t* AutoBuffer::LoadFile(const std::wstring& wsFile)
	{
		return LoadFile(wsFile, -1);
	}

	uint8_t* AutoBuffer::LoadFile(const std::wstring& wsFile, size_t szFile)
	{
		std::ifstream ifs(wsFile, std::ios::binary);
		if (!ifs.is_open()) return nullptr;

		if (szFile == -1) { szFile = static_cast<size_t>(FileX::GetFileSize(ifs)); }

		ifs.read(reinterpret_cast<char*>(ReSize(szFile)), szFile);

		return GetPointer();
	}

	bool AutoBuffer::SaveToFile(const std::wstring& wsFile)
	{
		std::ofstream ofs(wsFile, std::ios::binary);
		if (!ofs.is_open()) return false;

		ofs.write(reinterpret_cast<char*>(GetPointer()), GetMaxSize());

		ofs.flush();
		return true;
	}
}