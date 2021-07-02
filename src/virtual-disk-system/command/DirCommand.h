#pragma once
#include "BaseCommand.h"


class DirCommand : public BaseCommand
{
public:
	explicit DirCommand();
	~DirCommand();
	virtual void Handle(std::vector<string_local>& args, NodeTreeProxy& proxy);
};