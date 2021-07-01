#pragma once
#include "BaseCommand.h"


class LoadCommand : public BaseCommand
{
public:
	explicit LoadCommand();
	~LoadCommand();
	virtual void handle();
};
