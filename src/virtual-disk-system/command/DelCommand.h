#pragma once
#include "BaseCommand.h"


class DelCommand : public BaseCommand
{
public:
	explicit DelCommand();
	~DelCommand();
	virtual void handle();
};