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
	explicit BaseNode(string_local& name, BaseNode* parent = nullptr);
	virtual ~BaseNode();
	
	NodeType GetType();

	string_local GetName();
	void SetName(const string_local& name);

	virtual uint64_t GetSize() = 0;
	
	BaseNode* GetParent();	//获取父节点指针
	void SetParent(BaseNode* parent);	//设置父节点指针

	bool IsNameEqualsTo(const string_local& name);	//判断该字符串是否与节点名称相同
protected:
	uint64_t size;						
	string_local m_name;
	NodeType m_type;
	BaseNode* m_parent = nullptr;
};

#endif // !__BASENODE_H__
