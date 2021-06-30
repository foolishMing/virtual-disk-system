#include "./application.h"
#include "./command/BaseCommand.h"
int main()
{
	//app : create() -> printCurrentPath() -> exec() 
	typedef Application::RunStatus stat;
	std::wstring input;
	std::shared_ptr<Application> app = std::make_shared<Application>();
	app.get()->create();
	while (true)
	{
		app.get()->printCurrentPath();
		app.get()->readln(input);
		stat ret = app.get()->exec(input); 
		if (ret == stat::exit)	//ÍË³öÓ¦ÓÃ
		{
			break; 
		}
	}
	return 0;
}
