#include "./application.h"
#include "./command/BaseCommand.h"

int main()
{
	std::shared_ptr<Application> app = std::make_shared<Application>();
	while (true)
	{
		app.get()->printCurrentPath();
		std::string input;
		std::getline(std::cin, input);
		Application::RunStatus ret = app.get()->exec(input);
		if (ret == Application::RunStatus::normal) 
		{
			continue;
		}
		if (ret == Application::RunStatus::exit)
		{
			break;
		}
	}
	return 0;
}
