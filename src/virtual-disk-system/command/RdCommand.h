#pragma once
#include "BaseCommand.h"


class RdCommand : public BaseCommand
{
public:
	explicit RdCommand();
	~RdCommand();
	virtual void handle();
};