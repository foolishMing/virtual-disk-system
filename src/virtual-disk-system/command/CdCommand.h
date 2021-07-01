#pragma once
#include "BaseCommand.h"


class CdCommand : public BaseCommand
{
public:
	explicit CdCommand();
	~CdCommand();
	virtual void handle();
};
