//<�ڵ����>
//��Ա���ԣ����ڵ�ָ�롢�ڵ����͡�����ʱ�䡢�ڵ��С���ڵ�����
#pragma once
#ifndef __BASENODE_H__
#define __BASENODE_H__

#include "../util/Common.h"

//�ڵ�����
enum class NodeType {
	Directory = 1,		//Ŀ¼
	File,			//�ļ�
	SymlinkD,		//��ݷ�ʽ(Ŀ¼)
	SymlinkF		//��ݷ�ʽ(�ļ�)	
};

class BaseNode{
public:
	explicit BaseNode();
	explicit BaseNode(string_local name, BaseNode* parent = nullptr);
	virtual ~BaseNode();
	
	virtual const size_t GetSize() const = 0; //��ȡ��С
	virtual const time_t GetLatestModifiedTimeStamp() = 0; //��ȡ�޸�ʱ��
	virtual void SetLatestModifiedTimeStamp(time_t ts) = 0; //�����޸�ʱ��	

	const string_local GetName();
	void SetName(const string_local& name);
	
	BaseNode* GetParent();	//��ȡ���ڵ�ָ��
	void SetParent(BaseNode* parent);	//���ø��ڵ�ָ��

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
	string_local m_name = TEXT(""); //�ڵ�����
	NodeType m_type; //�ڵ�����
	BaseNode* m_parent = nullptr; //���ڵ�

	//���һ���޸ĵ���ʱ���
	//�޸��ļ����е��ļ������ļ�����������ļ���ɾ���ļ�����ı��ļ��е��޸�ʱ�䡣
	time_t m_latest_modify_time_stamp = 0;
};

#endif // !__BASENODE_H__
