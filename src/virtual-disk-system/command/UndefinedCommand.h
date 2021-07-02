#pragma once
#include "BaseCommand.h"

class UndefinedCommand : public BaseCommand
{
public:
	explicit UndefinedCommand();
	virtual ~UndefinedCommand();
	virtual void Handle(std::vector<string_local>& args, NodeTreeProxy& proxy);
};
