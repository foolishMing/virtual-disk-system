#pragma once
#include "BaseCommand.h"


class DirCommand : public BaseCommand
{
public:
	explicit DirCommand();
	~DirCommand();
	virtual void handle();
};