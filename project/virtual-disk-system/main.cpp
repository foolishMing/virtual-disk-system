#include "./application.h"
#include "Windows.h"
//#include "spdlog/spdlog.h"
//#include "spdlog/common.h"
//
//#include "spdlog/sinks/stdout_color_sinks.h"

int main(int argc, char* argv[])
{	
	_wsystem(TEXT("title �������ϵͳ"));
	Application app;
	app.Create();
	app.Run();
	app.Destroy();
	Console::Write::Print(TEXT("���������ַ����˳�:"));
	string_local str;
	Console::Read::ReadLine(str);
	return 0;
}
