#pragma once
#include "BaseCommand.h"


class RdCommand : public BaseCommand
{
public:
	explicit RdCommand();
	~RdCommand();
	virtual void Handle(std::vector<string_local>& args, NodeTreeProxy& proxy);
};