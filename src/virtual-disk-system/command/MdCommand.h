#pragma once
#include "BaseCommand.h"


class MdCommand : public BaseCommand
{
public:
	explicit MdCommand();
	~MdCommand();
	virtual void Handle(std::vector<string_local>& args, NodeTreeProxy& proxy);
};