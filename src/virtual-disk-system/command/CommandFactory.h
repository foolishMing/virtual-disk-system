#pragma once
#include "BaseCommand.h"
#include "CdCommand.h"
#include "CopyCommand.h"
#include "DelCommand.h"
#include "DirCommand.h"
#include "LoadCommand.h"
#include "MdCommand.h"
#include "MklinkCommand.h"
#include "MoveCommand.h"
#include "RdCommand.h"
#include "RenCommand.h"
#include "SaveCommand.h"
#include "UndefinedCommand.h"



class CommandFactory : Object{
public:
	explicit CommandFactory();
	~CommandFactory();
	void Create();
	void Destroy();
	BaseCommand* GetCommandInstance(CommandType type);
private:
	std::map<CommandType, BaseCommand*> m_cmd_instance_map;
	BaseCommand* CreateCommandInstance(CommandType type);
};
