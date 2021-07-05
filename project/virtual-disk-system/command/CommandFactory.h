#pragma once
#include "BaseCommand.h"
#include "CdCommand.h"
#include "CopyCommand.h"
#include "DelCommand.h"
#include "DirCommand.h"
#include "LoadCommand.h"
#include "MdCommand.h"
#include "MklinkCommand.h"
#include "MoveCommand.h"
#include "RdCommand.h"
#include "RenCommand.h"
#include "SaveCommand.h"
#include "ClsCommand.h"
#include "UndefinedCommand.h"

enum CommandType {
	undefine = 0,	//未定义类型
	quit,		//退出程序
	dir,		//列出当前目录下的目录和文件
	md,			//创建目录
	rd,			//删除目录
	cd,			//切换当前目录
	del,		//删除文件
	copy,		//拷贝文件
	ren,		//重命名
	move,		//移动
	mklink,		//创建符号链接
	save,		//序列化
	load,		//反序列化
	cls,		//清屏
	tail,		//结束符，无实际意义
};

class CommandFactory : Object{
public:
	explicit CommandFactory();
	~CommandFactory();
	void Create();
	void Destroy();
	BaseCommand* GetCommandInstance(CommandType type);	//获取命令实例
	CommandType GetCommandTypeByToken(string_local& token);	//获取命令类型


private:
	std::map<CommandType, BaseCommand*> m_cmd_instance_map;
	std::map<string_local, CommandType> m_cmd_token_map;
	
	BaseCommand* CreateCommandInstance(CommandType type);
};
