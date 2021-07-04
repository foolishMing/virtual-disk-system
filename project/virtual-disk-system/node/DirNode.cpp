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

BaseNode* DirNode::FindChildByName(const string_local& node_name)	//根据节点名称获取子节点
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

void DirNode::AppendChild(BaseNode* node)	//向列表末尾追加(非空)子节点
{
	assert(nullptr != node);
	if (nullptr == node)
	{
		//udpate : 打印日志（警告）
		return;
	}
	m_children.push_back(node);
}

uint64_t DirNode::GetSize()
{
	//update : 需要重写
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
