#pragma once
#ifndef __BASECOMMAND_H__
#define __BASECOMMAND_H__

#include "../util/common.h"
#include "../node/NodeTreeManager.h"

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
	cls,		//清屏
	tail,		//结束符，无实际意义
};

interface IBaseCommand
{
	virtual void Handle() = 0;
};

class BaseCommand : IBaseCommand, Object{
public:
	explicit BaseCommand();
	~BaseCommand();
	virtual void Handle();
	virtual void Handle(std::vector<string_local>& args, NodeTreeManager& proxy);
	virtual void Create();
	virtual void Destroy();
private:
	CommandType	m_type;
};

#endif // !__BASECOMMAND_H__

