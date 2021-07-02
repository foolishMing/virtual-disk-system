#pragma once
#include "BaseCommand.h"


class MklinkCommand : public BaseCommand
{
public:
	explicit MklinkCommand();
	~MklinkCommand();
	virtual void Handle(std::vector<string_local>& args, NodeTreeManager& proxy);
};