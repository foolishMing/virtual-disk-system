//<节点基类>
//成员属性：父节点指针、节点类型、创建时间、节点大小、节点名称
#pragma once
#ifndef __BASENODE_H__
#define __BASENODE_H__

#include "../util/Common.h"

//节点类型
enum class NodeType {
	Directory = 1,		//目录
	File,			//文件
	SymlinkD,		//快捷方式(目录)
	SymlinkF		//快捷方式(文件)	
};

class BaseNode{
public:
	explicit BaseNode();
	explicit BaseNode(string_local name, BaseNode* parent = nullptr);
	virtual ~BaseNode();
	
	virtual const size_t GetSize() const = 0; //获取大小
	virtual const time_t GetLatestModifiedTimeStamp() = 0; //获取修改时间
	virtual void SetLatestModifiedTimeStamp(time_t ts) = 0; //设置修改时间	

	const string_local GetName();
	void SetName(const string_local& name);
	
	BaseNode* GetParent();	//获取父节点指针
	void SetParent(BaseNode* parent);	//设置父节点指针

	const uint64_t GetId() const { return m_id; }
	void SetId(uint64_t id) { m_id = id; }

	const NodeType GetType();

	bool IsNameEqualsTo(const string_local& token) { return StringTools::IsEqual(m_name, token); }	
	bool IsDirectory() { return (NodeType::Directory == m_type) ? true : false; }
	bool IsFile() { return (NodeType::File == m_type) ? true : false; }
	bool IsDirectoryLink() { return (NodeType::SymlinkD == m_type) ? true : false; }
	bool IsFileLink() { return (NodeType::SymlinkF == m_type) ? true : false; }

protected:
	static uint64_t g_id;
	uint64_t m_id;
	string_local m_name = TEXT(""); //节点名称
	NodeType m_type; //节点类型
	BaseNode* m_parent = nullptr; //父节点

	//最近一次修改的秒时间戳
	//修改文件夹中的文件、在文件夹里添加新文件和删除文件都会改变文件夹的修改时间。
	time_t m_latest_modify_time_stamp = 0;
};

#endif // !__BASENODE_H__
