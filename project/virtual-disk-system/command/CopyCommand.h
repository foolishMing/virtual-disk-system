#pragma once

#ifndef __COPYCOMMAND_H__
#define __COPYCOMMAND_H__

#include "BaseCommand.h"

class CopyCommand : public BaseCommand
{
public:
	explicit CopyCommand();
	~CopyCommand();
	virtual void Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager);

	void CopyFromDiskToMemory(const string_local& src_path, const std::vector<string_local>& dst_path_tokens, NodeTreeManager& node_tree_manager, OptionSwitch option_switch);
	void CopyFromMemoryToMemory(const CommandArg& arg, const std::vector<string_local>& dst_path_tokens, NodeTreeManager& node_tree_manager, const OptionSwitch& option_switch);


	void CopyFromMemoryToDisk();
	void CopyFromDiskToDisk();
};

#endif // !__COPYCOMMAND_H__


