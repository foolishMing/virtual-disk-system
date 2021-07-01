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

interface ICommandFactory {
	void create();
	void destroy();
};

class CommandFactory : ICommandFactory{
public:
	explicit CommandFactory();
	~CommandFactory();
	void create();
	void destroy();
	BaseCommand* GetCommand(CommandType type);

private:
	std::map<CommandType, BaseCommand*> m_command_map;
	BaseCommand* CreateCommand(CommandType type);
};
