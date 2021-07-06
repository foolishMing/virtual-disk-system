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
		Log::LogError(L"非法操作：尝试在树上访问空指针");
		return;
	}
	if (node->GetType() != NodeType::Directory)
	{
		Log::LogError(L"非法操作：尝试在非目录节点下插入子节点");
		return;
	}
	auto dir = (DirNode*)node;
	auto child_find = dir->FindChildByName(new_child->GetName());
	if (nullptr != child_find)
	{
		Log::LogWarn(L"非法操作：尝试插入已存在的子节点");
		return;
	}
	dir->AppendChild(new_child);
	new_child->SetParent(node);
}

bool NodeTree::FindNode(std::vector<BaseNode*>& node_path_vec, std::vector<string_local>& name_list)
{
	assert(0 == node_path_vec.size());
	auto parrent = m_root;
	int len = static_cast<int>(name_list.size());
	for (int i = 0; i < len; i++)
	{
		bool is_find = false;
		auto name = name_list[i];
		//回溯到父节点
		if (name == L"..")
		{
			//udpate ... 
		}
		//停留在当前节点
		else if (name == L".")
		{
			//update ...
		}
		//parent是文件节点
		else if (NodeType::Directory != parrent->GetType())
		{
			//update ...
		}
		//parent是目录节点
		else
		{
			//update ...
		}
		if (!is_find)
		{
			return false;
		}
	}
	return true;
}
