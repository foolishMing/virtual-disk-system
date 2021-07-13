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
		Log::LogError(TEXT("�Ƿ����������������Ϸ��ʿ�ָ��"));
		return false;
	}
	if (!node->IsDirectory())
	{
		Log::LogError(TEXT("�Ƿ������������ڷ�Ŀ¼�ڵ��²����ӽڵ�"));
		return false;
	}
	auto dir = (DirNode*)node;
	auto child_find = dir->FindChildByName(new_child->GetName());
	if (nullptr != child_find)
	{
		Log::LogWarn(TEXT("�Ƿ����������Բ����Ѵ��ڵ��ӽڵ�"));
		return false;
	}
	dir->AppendChild(new_child);
	new_child->SetParent(node);
	return true;
}


//1��ʹ��˫�˶���ά��
//2��ʹ�����ѱ������нڵ�
//3����ǰ�������ȡ���ڵ㣨����һ��ʼ����Ҷ�ӽڵ㣩
//	 3-1������͸��ڵ�֮��Ĺ�ϵ
//   3-2��ɾ���ýڵ�
bool NodeTree::DeleteNode(BaseNode* node)
{
	assert(nullptr != node);
	Log::LogInfo(TEXT("delete node ") + node->GetName());
	//�����Ŀ¼�ڵ���ɾ��
	if (!node->IsDirectory())
	{
		//����͸��ڵ�Ĺ�ϵ
		RemoveButNotDeleteNode(node);
		delete node;
		return true;
	}
	DirNode* cur_dir = static_cast<DirNode*>(node);
	std::vector<BaseNode*> vec = cur_dir->Children();
	//ɾ�������б�
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
	//����͸��ڵ�Ĺ�ϵ
	RemoveButNotDeleteNode(node);
	//ɾ����ǰ�ڵ�
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
