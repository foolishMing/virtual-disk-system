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
	assert(root);
	m_root = root;
	Log::Info(TEXT("node tree is created."));
}

void NodeTree::Destroy()
{
	Log::Info(TEXT("--destroy node tree start--"));
	if (m_root)
	{
		bool ok = DeleteNode(m_root);
	}
	Log::Info(TEXT("--destroy node tree complete--"));
}

//注意：
//在软链接下插入节点，相关逻辑需要在外层完成，
//因为其所持有的引用路径为动态解析，依赖于系统当前工作路径，
//如果写在NodeTree内，就需要将NodeTree与工作路径耦合在一起，会增加系统的复杂度
bool NodeTree::InsertNode(BaseNode* node, BaseNode* new_child) 
{
	assert(node && new_child);
	assert(node->IsDirectory());
	if (nullptr == node || nullptr == new_child)
	{
		Log::Error(TEXT("非法操作：尝试在树上访问空指针"));
		return false;
	}
	auto dir = (DirNode*)node;
	auto child_find = dir->FindChildByName(new_child->GetName());
	if (nullptr != child_find)
	{
		Log::Warn(TEXT("非法操作：尝试插入已存在的子节点"));
		return false;
	}
	dir->AppendChild(new_child);
	new_child->SetParent(node);
	Log::Info(TEXT("Insert Node : ") + node->GetName() + TEXT("->") + new_child->GetName());
	return true;
}


//使用深搜遍历所有节点
//若有儿子节点，删除所有儿子节点
//与父节点解除关系
//最后删掉这个孤儿节点
bool NodeTree::DeleteNode(BaseNode* node)
{
	assert(nullptr != node);
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
	Log::Info(TEXT("delete node ") + node->GetName());
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
