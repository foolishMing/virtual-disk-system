#include "CommandFactory.h"
#include <cassert>

CommandFactory::CommandFactory()
{

}

CommandFactory::~CommandFactory()
{

}

void CommandFactory::create()
{
	for (auto type = CommandType::undefine; type < CommandType::tail; type = (CommandType)(type + 1)) {
		m_command_map.insert(std::pair<CommandType,BaseCommand*>(type, CreateCommand(type)));
	}
}

void CommandFactory::destroy()
{
	auto iter = m_command_map.begin();
	while (iter != m_command_map.end()) 
	{
		delete iter->second;
		iter->second = nullptr;
		iter = m_command_map.erase(iter);
	}
	m_command_map.clear();
}

BaseCommand* CommandFactory::CreateCommand(CommandType type)
{
	assert(type != CommandType::tail);
	BaseCommand* cmd = nullptr;
	switch (type)
	{
	case quit:
		break;
	case dir:
		break;
	case md:
		break;
	case rd:
		break;
	case cd:
		break;
	case del:
		break;
	case copy:
		break;
	case ren:
		break;
	case move:
		break;
	case mklink:
		break;
	case save:
		break;
	case load:
		break;
	default:
		cmd = new UndefinedCommand();
		break;
	}
	return cmd;
}

BaseCommand* CommandFactory::GetCommand(CommandType type)
{
	assert(type != CommandType::tail);
	auto item = m_command_map.find(type);
	return item->second;
}
