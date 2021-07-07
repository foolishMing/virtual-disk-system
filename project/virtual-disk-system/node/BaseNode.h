#pragma once
#ifndef __BASENODE_H__
#define __BASENODE_H__

#include "../util/Common.h"

//节点类型
enum NodeType {
	Directory,		//目录
	File,			//文件
	SymlinkD,		//快捷方式(目录)
	SymlinkF		//快捷方式(文件)	
};

class BaseNode{
public:
	explicit BaseNode();
	explicit BaseNode(string_local name, BaseNode* parent = nullptr);
	virtual ~BaseNode();
	
	const NodeType GetType();

	const string_local GetName();
	void SetName(const string_local& name);

	virtual size_t GetSize() = 0;
	
	BaseNode* GetParent();	//获取父节点指针
	void SetParent(BaseNode* parent);	//设置父节点指针

	const time_t GetLatestModifiedTimeStamp(); //获取修改时间
	void SetLatestModifiedTimeStamp(time_t ts); //设置修改时间	

	bool IsNameEqualsTo(const string_local& name);	//判断该字符串是否与节点名称相同
protected:
	size_t m_size; //节点大小						
	string_local m_name; //节点名称
	NodeType m_type; //节点类型
	BaseNode* m_parent = nullptr;
	time_t m_latest_modify_time_stamp = 0;//最近一次修改的秒时间戳
};

#endif // !__BASENODE_H__
