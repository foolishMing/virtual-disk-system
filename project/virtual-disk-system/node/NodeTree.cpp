#include "NodeTree.h"


NodeTree::NodeTree()
{

}

NodeTree::~NodeTree()
{
	Destroy();
}

void NodeTree::Create()
{
	string_local nil_str = L"";
	m_root = new DirNode(nil_str);
	Log::LogInfo(L"node tree is created.");
}

void NodeTree::Destroy()
{
	if (nullptr != m_root)
	{
		//udpate... 递归删除整棵树
	}
}


void NodeTree::InsertNode(BaseNode* node, BaseNode* new_child) 
{
	assert(nullptr != node && nullptr != new_child);
	assert(node->GetType() == NodeType::Directory);
	if (nullptr == node || nullptr == new_child)
	{
		//udpate.. 写警告日志,空指针
		return;
	}
	if (node->GetType() != NodeType::Directory)
	{
		//udpate.. 写警告日志，非目录节点
		return;
	}
	auto dir = (DirNode*)node;
	auto child_find = dir->FindChildByName(new_child->GetName());
	if (nullptr != child_find)
	{
		//update.. 写警告日志，子节点已存在
	}
	dir->AppendChild(new_child);
	new_child->SetParent(node);
}
