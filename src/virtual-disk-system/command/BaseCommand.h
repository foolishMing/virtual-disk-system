#pragma once
#ifndef __BASECOMMAND_H__
#define __BASECOMMAND_H__

#include "../util/common.h"
#include "../node/NodeTreeProxy.h"

enum CommandType {
	undefine = 0,	//未定义类型
	quit,		//结束程序
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
	tail,		//结束符，无实际意义
};

interface IBaseCommand
{
	virtual void handle() = 0;
	virtual void destroy() = 0;
};

class BaseCommand : IBaseCommand {
public:
	explicit BaseCommand();
	~BaseCommand();
	virtual void handle();
	virtual void handle(std::vector<string_local>& args, NodeTreeProxy& proxy);
	virtual void destroy();
private:
	CommandType	m_type;
	std::vector<string_local> m_args;
};

#endif // !__BASECOMMAND_H__

