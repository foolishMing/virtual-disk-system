#pragma once
#include "BaseCommand.h"


class MoveCommand : public BaseCommand
{
public:
	explicit MoveCommand();
	~MoveCommand();
	virtual void handle();
};