#pragma once
#include "BaseCommand.h"

class UndefinedCommand : public BaseCommand
{
public:
	explicit UndefinedCommand();
	virtual ~UndefinedCommand();
	virtual void handle();
};
