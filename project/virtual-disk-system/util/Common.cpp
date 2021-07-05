#include "Common.h"








namespace PathTools
{
	bool IsDiskPathExist(const string_local& str)
	{
		std::filesystem::path p(str);
		if (exists(p)) {
			return true;
		}
		return false;
	}
}


