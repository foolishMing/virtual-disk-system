#include "DirNode.h"


DirNode::DirNode(string_local name, BaseNode* parent)
{
	m_type = NodeType::Directory;
	m_parent = parent;
	m_name = name;
}

DirNode::~DirNode()
{

}

BaseNode* DirNode::FindChildByName(const string_local& node_name)	//���ݽڵ����ƻ�ȡ�ӽڵ�
{
	BaseNode* ret = nullptr;
	for (auto item : m_children)
	{
		if (item->IsNameEqualsTo(node_name))
		{
			ret = item;
			break;
		}
	}
	return ret;
}

void DirNode::AppendChild(BaseNode* node)	//���б�ĩβ׷��(�ǿ�)�ӽڵ�
{
	assert(nullptr != node);
	if (nullptr == node)
	{
		//udpate : ��ӡ��־�����棩
		return;
	}
	m_children.push_back(node);
}

uint64_t DirNode::GetSize()
{
	//update : ��Ҫ��д
	return 0;
}

void DirNode::DisposeChildByName(const string_local& node_name)
{
	//update
}

void DirNode::DeleteChildByName(const string_local& node_name)
{
	//update
}

bool DirNode::ContainsChild(const string_local& node_name)
{
	for (auto item : m_children)
	{
		if (item->IsNameEqualsTo(node_name))
		{
			return true;
		}
	}
	return false;
}
