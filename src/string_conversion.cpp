#include "string_conversion.h"
namespace Utils
{
	std::string ConvertWstringToString(const String& str)
	{
#if (_WIN32)
		return { str.begin(), str.end() };
#elif (__LINUX__ || __gnu_linux__ || __linux__ || __linux || linux)
		return str;
#endif
	}
}