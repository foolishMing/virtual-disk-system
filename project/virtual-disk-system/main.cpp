#include "./application.h"
#include "Windows.h"
//#include "spdlog/spdlog.h"
//#include "spdlog/common.h"
//
//#include "spdlog/sinks/stdout_color_sinks.h"

int main(int argc, char* argv[])
{	
	_wsystem(TEXT("title �������ϵͳ"));
	std::shared_ptr<Application> app = std::make_shared<Application>();
	app.get()->Create();
	app.get()->Run();
	app.get()->Destroy();
	Console::Write::Print(TEXT("���������ַ����˳�:"));
	string_local str;
	Console::Read::ReadLine(str);
	return 0;
}
