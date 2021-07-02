#pragma once
#include "BaseCommand.h"

class RenCommand : public BaseCommand
{
public:
	explicit RenCommand();
	~RenCommand();
	virtual void Handle(std::vector<string_local>& args, NodeTreeManager& proxy);
};