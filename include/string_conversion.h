#pragma once

#include <string>
#include <locale>
#include <codecvt>

#if (_WIN32)
#define String std::wstring
#elif (__LINUX__ || __gnu_linux__ || __linux__ || __linux || linux)
#define String std::string
#endif

namespace Utils
{
	std::string ConvertWstringToString(const String& str);
}