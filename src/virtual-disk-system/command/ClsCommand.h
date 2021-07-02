#pragma once
#ifndef __CLSCOMMAND_H__
#define __CLSCOMMAND_H__

#include "BaseCommand.h"

class ClsCommand : public BaseCommand
{
public:
	explicit ClsCommand();
	virtual ~ClsCommand();
	virtual void Handle(std::vector<string_local>& args, NodeTreeProxy& proxy);
};

#endif // !__CLSCOMMAND_H__
