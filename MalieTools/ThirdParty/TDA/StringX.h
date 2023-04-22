#pragma once
#include <string>


namespace TDA
{
	class StringX
	{
	public:
		static std::locale&  GetCVT_UTF8();

		static std::wstring StrToWStr(const std::string& msString, size_t uCodePage);
		static std::string  WStrToStr(const std::wstring& wsString, size_t uCodePage);
		static size_t        StrToWStr(const std::string& msString, std::wstring& wsString, size_t uCodePage);
		static size_t        WStrToStr(const std::wstring& wsString, std::string& msString, size_t uCodePage);

		static std::wstring StrToWStr_CVT(const std::string& msString,size_t uCodePage);
		static std::string  WStrToStr_CVT(const std::wstring& wsString, size_t uCodePage);
		static void          StrToWStr_CVT(const std::string& msString, std::wstring& wsString, size_t uCodePage);
		static void          WStrToStr_CVT(const std::wstring& wsString, std::string& msString, size_t uCodePage);

	};
}