#pragma once
#include "BaseCommand.h"


class CdCommand : public BaseCommand
{
public:
	explicit CdCommand();
	~CdCommand();
	virtual void Handle(std::vector<string_local>& args, NodeTreeManager& proxy);
};
