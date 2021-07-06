#include "Common.h"
#include "time.h"

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

namespace DateTimeTools
{
	int StampToStandardDateTime(time_t stc, string_local& dst)
	{
		return 0;
	}
}


