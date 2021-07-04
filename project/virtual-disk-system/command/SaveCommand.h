#pragma once
#include "BaseCommand.h"

class SaveCommand : public BaseCommand
{
public:
	explicit SaveCommand();
	~SaveCommand();
	virtual void Handle(std::vector<string_local>& args, NodeTreeManager& proxy);
};