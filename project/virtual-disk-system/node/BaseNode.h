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
	
	virtual size_t GetSize() = 0; //获取大小
	virtual const time_t GetLatestModifiedTimeStamp() = 0; //获取修改时间
	virtual void SetLatestModifiedTimeStamp(time_t ts) = 0; //设置修改时间	


	const NodeType GetType();

	const string_local GetName();
	void SetName(const string_local& name);

	
	BaseNode* GetParent();	//获取父节点指针
	void SetParent(BaseNode* parent);	//设置父节点指针
	

	bool IsNameEqualsTo(const string_local& token) { return StringTools::IsEqual(m_name, token); }	
	bool IsDirectory() { return (NodeType::Directory == m_type) ? true : false; }
	bool IsFile() { return (NodeType::File == m_type) ? true : false; }
	bool IsDirectoryLink() { return (NodeType::SymlinkD == m_type) ? true : false; }
	bool IsFileLink() { return (NodeType::SymlinkF == m_type) ? true : false; }

protected:
	string_local m_name = L""; //节点名称
	NodeType m_type; //节点类型
	BaseNode* m_parent = nullptr; //父节点
	time_t m_latest_modify_time_stamp = 0;//最近一次修改的秒时间戳
};

#endif // !__BASENODE_H__
