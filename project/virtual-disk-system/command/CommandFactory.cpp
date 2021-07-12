#include "CommandFactory.h"
#include <cassert>
#include <sstream>
#include <windows.h>
CommandFactory::CommandFactory()
{

}

CommandFactory::~CommandFactory()
{

}

void CommandFactory::Create()
{
	//创建token映射表
	m_cmd_token_map.insert(std::pair<string_local, CommandType>(TEXT("quit"), CommandType::quit));
	m_cmd_token_map.insert(std::pair<string_local, CommandType>(TEXT("dir"), CommandType::dir));
	m_cmd_token_map.insert(std::pair<string_local, CommandType>(TEXT("md"), CommandType::md));
	m_cmd_token_map.insert(std::pair<string_local, CommandType>(TEXT("rd"), CommandType::rd));
	m_cmd_token_map.insert(std::pair<string_local, CommandType>(TEXT("cd"), CommandType::cd));
	m_cmd_token_map.insert(std::pair<string_local, CommandType>(TEXT("del"), CommandType::del));
	m_cmd_token_map.insert(std::pair<string_local, CommandType>(TEXT("copy"), CommandType::copy));
	m_cmd_token_map.insert(std::pair<string_local, CommandType>(TEXT("ren"), CommandType::ren));
	m_cmd_token_map.insert(std::pair<string_local, CommandType>(TEXT("move"), CommandType::move));
	m_cmd_token_map.insert(std::pair<string_local, CommandType>(TEXT("mklink"), CommandType::mklink));
	m_cmd_token_map.insert(std::pair<string_local, CommandType>(TEXT("save"), CommandType::save));
	m_cmd_token_map.insert(std::pair<string_local, CommandType>(TEXT("load"), CommandType::load));
	m_cmd_token_map.insert(std::pair<string_local, CommandType>(TEXT("cls"), CommandType::cls));
	//创建单例映射表
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
	case cls:
		instance = new ClsCommand();
		break;
	default:
		instance = new UndefinedCommand();
		break;
	}
	instance->Create();
	return instance;
}

//-constraints
//tail是不可达的CommandType
BaseCommand* CommandFactory::GetCommandInstance(CommandType type)
{
	assert(CommandType::tail != type);
	auto item = m_cmd_instance_map.find(type);
	return item->second;
}

//-constraints
CommandType CommandFactory::GetCommandTypeByToken(string_local& token)
{
	//查找指令
	for (const auto& item : m_cmd_token_map)
	{
		auto cmd_str = item.first;
		if (StringTools::IsEqual(token, cmd_str))
		{
			return item.second;
		}
	}
	return CommandType::undefine;
}



