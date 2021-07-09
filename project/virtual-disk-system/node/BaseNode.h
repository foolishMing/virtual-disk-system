#pragma once
#ifndef __BASENODE_H__
#define __BASENODE_H__

#include "../util/Common.h"

//�ڵ�����
enum NodeType {
	Directory,		//Ŀ¼
	File,			//�ļ�
	SymlinkD,		//��ݷ�ʽ(Ŀ¼)
	SymlinkF		//��ݷ�ʽ(�ļ�)	
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
	
	BaseNode* GetParent();	//��ȡ���ڵ�ָ��
	void SetParent(BaseNode* parent);	//���ø��ڵ�ָ��

	const time_t GetLatestModifiedTimeStamp(); //��ȡ�޸�ʱ��
	void SetLatestModifiedTimeStamp(time_t ts); //�����޸�ʱ��	

	bool IsNameEqualsTo(const string_local& token) { return StringTools::IsEqual(m_name, token); }	
	bool IsDirectory() { return (NodeType::Directory == m_type) ? true : false; }
	bool IsFile() { return (NodeType::File == m_type) ? true : false; }
	bool IsDirectoryLink() { return (NodeType::SymlinkD == m_type) ? true : false; }
	bool IsFileLink() { return (NodeType::SymlinkF == m_type) ? true : false; }

protected:
	size_t m_size = 0; //�ڵ��С						
	string_local m_name = L""; //�ڵ�����
	NodeType m_type; //�ڵ�����
	BaseNode* m_parent = nullptr; //���ڵ�
	time_t m_latest_modify_time_stamp = 0;//���һ���޸ĵ���ʱ���
};

#endif // !__BASENODE_H__
