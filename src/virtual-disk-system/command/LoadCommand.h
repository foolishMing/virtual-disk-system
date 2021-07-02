#pragma once
#include "BaseCommand.h"


class LoadCommand : public BaseCommand
{
public:
	explicit LoadCommand();
	~LoadCommand();
	virtual void Handle(std::vector<string_local>& args, NodeTreeProxy& proxy);
};
