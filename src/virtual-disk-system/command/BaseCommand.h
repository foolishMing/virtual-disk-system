#pragma once
#include "../util/common.h"

interface IBaseCommand
{

};

class BaseCommand : IBaseCommand {
public:
	static enum CommandType {
		undefine,	//未定义类型
		exit,		//结束程序
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
	};
};


