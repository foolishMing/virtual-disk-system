#pragma once
#include "BaseCommand.h"


class MdCommand : public BaseCommand
{
public:
	explicit MdCommand();
	~MdCommand();
	virtual void handle();
};