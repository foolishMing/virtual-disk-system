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


bool NodeTree::InsertNode(BaseNode* node, BaseNode* new_child) 
{
	assert(nullptr != node && nullptr != new_child);
	assert(node->GetType() == NodeType::Directory);
	if (nullptr == node || nullptr == new_child)
	{
		Log::LogError(L"非法操作：尝试在树上访问空指针");
		return false;
	}
	if (node->GetType() != NodeType::Directory)
	{
		Log::LogError(L"非法操作：尝试在非目录节点下插入子节点");
		return false;
	}
	auto dir = (DirNode*)node;
	auto child_find = dir->FindChildByName(new_child->GetName());
	if (nullptr != child_find)
	{
		Log::LogWarn(L"非法操作：尝试插入已存在的子节点");
		return false;
	}
	dir->AppendChild(new_child);
	new_child->SetParent(node);
	return true;
}
