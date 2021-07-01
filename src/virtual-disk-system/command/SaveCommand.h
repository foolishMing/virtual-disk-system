#pragma once
#include "BaseCommand.h"

class SaveCommand : public BaseCommand
{
public:
	explicit SaveCommand();
	~SaveCommand();
	virtual void handle();
};