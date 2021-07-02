#pragma once
#include "BaseCommand.h"


class CopyCommand : public BaseCommand
{
public:
	explicit CopyCommand();
	~CopyCommand();
	virtual void Handle(std::vector<string_local>& args, NodeTreeManager& proxy);
};