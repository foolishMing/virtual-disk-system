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

	//�Ӵ��̸����ļ����ڴ�
	void CopyFileFromDiskToMemory(const Path& src_path, const Path& dst_path, NodeTreeManager& manager, OptionSwitch option_switch);
	
	//���ڴ渴���ļ����ڴ�
	void CopyFileFromMemoryToMemory(const Path& src_path, const Path& dst_path, NodeTreeManager& manager, OptionSwitch option_switch);
};

#endif // !__COPYCOMMAND_H__
