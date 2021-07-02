#include "CommandFactory.h"
#include <cassert>
#include <sstream>

CommandFactory::CommandFactory()
{

}

CommandFactory::~CommandFactory()
{

}

void CommandFactory::Create()
{
	for (auto type = CommandType::undefine; type < CommandType::tail; type = (CommandType)(type + 1)) {
		m_cmd_instance_map.insert(std::pair<CommandType,BaseCommand*>(type, CreateCommandInstance(type)));
	}
}

void CommandFactory::Destroy()
{
	auto iter = m_cmd_instance_map.begin();
	while (iter != m_cmd_instance_map.end()) 
	{
	    iter->second->Destroy();
		iter->second = nullptr;
		iter = m_cmd_instance_map.erase(iter);
	}
	m_cmd_instance_map.clear();
}

BaseCommand* CommandFactory::CreateCommandInstance(CommandType type)
{
	BaseCommand* instance = nullptr;
	switch (type)
	{
	case dir:
		instance = new DirCommand();
		break;
	case md:
		instance = new MdCommand();
		break;
	case rd:
		instance = new RdCommand();
		break;
	case cd:
		instance = new CdCommand();
		break;
	case del:
		instance = new DelCommand();
		break;
	case copy:
		instance = new CopyCommand();
		break;
	case ren:
		instance = new RenCommand();
		break;
	case move:
		instance = new MoveCommand();
		break;
	case mklink:
		instance = new MklinkCommand();
		break;
	case save:
		instance = new SaveCommand();
		break;
	case load:
		instance = new LoadCommand();
		break;
	default:
		instance = new UndefinedCommand();
		break;
	}
	instance->Create();
	return instance;
}

BaseCommand* CommandFactory::GetCommandInstance(CommandType type)
{
	assert(type != CommandType::tail);
	auto item = m_cmd_instance_map.find(type);
	return item->second;
}


