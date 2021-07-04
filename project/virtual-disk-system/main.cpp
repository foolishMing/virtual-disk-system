#include "./application.h"
#include "./command/BaseCommand.h"

int main(int argc, char* argv[])
{	
	std::shared_ptr<Application> app = std::make_shared<Application>();
	app.get()->Create();
	app.get()->Run();
	app.get()->Destroy();
	return 0;
}
