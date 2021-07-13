#include "NodeTree.h"
#include <stack>
#include <queue>
NodeTree::NodeTree()
{

}

NodeTree::~NodeTree()
{
	Destroy();
}

void NodeTree::Create(BaseNode* root)
{
	if (root)
	{
		m_root = root;
	}
	else
	{
		string_local nil_str = TEXT("root");
		m_root = new DirNode(nil_str);
		Log::LogInfo(TEXT("node tree is created."));
	}
}

void NodeTree::Destroy()
{
	Log::LogInfo(TEXT("--destroy node tree--"));
	if (nullptr != m_root)
	{
		bool ok = DeleteNode(m_root);
	}
}


bool NodeTree::InsertNode(BaseNode* node, BaseNode* new_child) 
{
	assert(nullptr != node && nullptr != new_child);
	assert(node->IsDirectory());
	if (nullptr == node || nullptr == new_child)
	{
		Log::LogError(TEXT("非法操作：尝试在树上访问空指针"));
		return false;
	}
	if (!node->IsDirectory())
	{
		Log::LogError(TEXT("非法操作：尝试在非目录节点下插入子节点"));
		return false;
	}
	auto dir = (DirNode*)node;
	auto child_find = dir->FindChildByName(new_child->GetName());
	if (nullptr != child_find)
	{
		Log::LogWarn(TEXT("非法操作：尝试插入已存在的子节点"));
		return false;
	}
	dir->AppendChild(new_child);
	new_child->SetParent(node);
	return true;
}


//1、使用双端队列维护
//2、使用深搜遍历所有节点
//3、从前向后依次取出节点（队首一定始终是叶子节点）
//	 3-1、解除和父节点之间的关系
//   3-2、删除该节点
bool NodeTree::DeleteNode(BaseNode* node)
{
	assert(nullptr != node);
	Log::LogInfo(TEXT("delete node ") + node->GetName());
	//如果非目录节点则删除
	if (!node->IsDirectory())
	{
		//解除和父节点的关系
		RemoveButNotDeleteNode(node);
		delete node;
		return true;
	}
	DirNode* cur_dir = static_cast<DirNode*>(node);
	std::vector<BaseNode*> vec = cur_dir->Children();
	//删除儿子列表
	if (!vec.empty())
	{
		auto iter = vec.begin();
		while (vec.end() != iter)
		{
			DeleteNode(*iter);
			*iter = nullptr;
			iter++;
		}
		vec.clear();
	}
	//解除和父节点的关系
	RemoveButNotDeleteNode(node);
	//删除当前节点
	delete node;
	return true;
}


bool NodeTree::RemoveButNotDeleteNode(BaseNode* node)
{
	if (node == m_root)
	{
		return true;
	}
	auto parent = static_cast<DirNode*>(node->GetParent());
	node->SetParent(nullptr);
	auto children = parent->Children();
	for (int i = 0; i < children.size(); i++)
	{
		if (children[i]->GetName() == node->GetName())
		{
			parent->RemoveChildByIndex(i);
			return true;
		}
	}
	return false;
}

BaseNode* NodeTree::FindNodeById(uint64_t id)
{
	std::queue<BaseNode*> q = {};
	q.push(m_root);
	while (!q.empty())
	{
		auto node = q.front();
		q.pop();
		if (id == node->GetId())
		{
			return node;
		}
		if (node->IsDirectory())
		{
			auto dir = static_cast<DirNode*>(node);
			auto children = dir->Children();
			for (const auto& item : children)
			{
				q.push(item);
			}
		}
	}
	return nullptr;
}
