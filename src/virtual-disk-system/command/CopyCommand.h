#pragma once
#include "BaseCommand.h"


class CopyCommand : public BaseCommand
{
public:
	explicit CopyCommand();
	~CopyCommand();
	virtual void handle();
};