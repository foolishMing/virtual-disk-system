#include "includes.h"
#include <windows.h>
TEST_CASE("IsStringFuzzyEqualTo(const string_local& text, const string_local& pattern)")
{
	string_local text_str;
	string_local pattern_str;
	SECTION("异常处理")
	{
		//update ...

	}
	SECTION("strictly match")
	{
		SECTION("only alphabet")
		{
			SECTION("require true")
			{
				text_str = L"ajkfbvw";
				pattern_str = L"ajkfbvw";
				bool ret = StringTools::IsStringFuzzyEqualTo(text_str, pattern_str);
				REQUIRE(ret == true);
			}

			SECTION("require false")
			{
				text_str = L"ajkfbvw";
				pattern_str = L"ajkfbv";
				bool ret = StringTools::IsStringFuzzyEqualTo(text_str, pattern_str);
				REQUIRE(ret == false);
			}			
		}
		SECTION("alphabet + number")
		{

		}
		SECTION("exist space")
		{

		}
		SECTION("exist other character")
		{

		}
	}
	SECTION("只有?的匹配")
	{

	}
	SECTION("只有*的匹配")
	{

	}
}