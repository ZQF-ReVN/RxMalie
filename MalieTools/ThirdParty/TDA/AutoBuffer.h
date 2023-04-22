#pragma once
#include <cstdint>
#include <string>

namespace TDA
{
	class AutoBuffer
	{
	private:
		uint8_t* m_pBuffer;
		size_t   m_szMaxAlloc;

	public:
		AutoBuffer();
		AutoBuffer(const AutoBuffer& buffer);
		AutoBuffer(AutoBuffer&& buffer) noexcept;
		AutoBuffer(const std::wstring& wsFile);
		AutoBuffer(const std::wstring& wsFile, size_t szFile);
		~AutoBuffer();

		uint8_t* ReSize(size_t szRes);
		uint8_t* LoadFile(const std::wstring& wsFile);
		uint8_t* LoadFile(const std::wstring& wsFile, size_t szFile);
		bool     SaveToFile(const std::wstring& wsFile);

		size_t   GetMaxSize() { return m_szMaxAlloc; }
		uint8_t* GetPointer() { return m_pBuffer; }

		void Clear();
	};
}