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
		//udpate... �ݹ�ɾ��������
	}
}


void NodeTree::InsertNode(BaseNode* node, BaseNode* new_child) 
{
	assert(nullptr != node && nullptr != new_child);
	assert(node->GetType() == NodeType::Directory);
	if (nullptr == node || nullptr == new_child)
	{
		Log::LogError(L"�Ƿ����������������Ϸ��ʿ�ָ��");
		return;
	}
	if (node->GetType() != NodeType::Directory)
	{
		Log::LogError(L"�Ƿ������������ڷ�Ŀ¼�ڵ��²����ӽڵ�");
		return;
	}
	auto dir = (DirNode*)node;
	auto child_find = dir->FindChildByName(new_child->GetName());
	if (nullptr != child_find)
	{
		Log::LogWarn(L"�Ƿ����������Բ����Ѵ��ڵ��ӽڵ�");
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
		//���ݵ����ڵ�
		if (name == L"..")
		{
			//udpate ... 
		}
		//ͣ���ڵ�ǰ�ڵ�
		else if (name == L".")
		{
			//update ...
		}
		//parent���ļ��ڵ�
		else if (NodeType::Directory != parrent->GetType())
		{
			//update ...
		}
		//parent��Ŀ¼�ڵ�
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
