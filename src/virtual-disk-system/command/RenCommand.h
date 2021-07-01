#pragma once
#include "BaseCommand.h"

class RenCommand : public BaseCommand
{
public:
	explicit RenCommand();
	~RenCommand();
	virtual void handle();
};