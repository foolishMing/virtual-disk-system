#pragma once
#include "BaseCommand.h"


class DelCommand : public BaseCommand
{
public:
	explicit DelCommand();
	~DelCommand();
	virtual void Handle(std::vector<string_local>& args, NodeTreeProxy& proxy);
};