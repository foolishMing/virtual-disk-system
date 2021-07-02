#include "./application.h"
#include "./command/BaseCommand.h"
int main()
{
	//app : create() -> printCurrentPath() -> exec() 
	typedef Application::RunStatus stat;
	string_local input;
	std::shared_ptr<Application> app = std::make_shared<Application>();
	app.get()->Create();
	while (true)
	{
		app.get()->PrintCurrentPath();
		app.get()->ReadLine(input);
		stat ret = app.get()->Exec(input); 
		if (ret == stat::exit)	//ÍË³öÓ¦ÓÃ
		{
			break; 
		}
	}
	app.get()->Destroy();
	return 0;
}
