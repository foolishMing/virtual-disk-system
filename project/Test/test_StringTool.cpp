#include "includes.h"

TEST_CASE("IsStringFuzzyEqualTo(const string_local& text, const string_local& pattern)")
{
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
				string_local text_str = L"";
				string_local pattern_str = L"";
				bool ret = StringTools::IsStringFuzzyEqualTo(text_str, pattern_str);
				REQUIRE(ret == true);
			}

			SECTION("require false")
			{

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