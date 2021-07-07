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


bool NodeTree::InsertNode(BaseNode* node, BaseNode* new_child) 
{
	assert(nullptr != node && nullptr != new_child);
	assert(node->GetType() == NodeType::Directory);
	if (nullptr == node || nullptr == new_child)
	{
		Log::LogError(L"�Ƿ����������������Ϸ��ʿ�ָ��");
		return false;
	}
	if (node->GetType() != NodeType::Directory)
	{
		Log::LogError(L"�Ƿ������������ڷ�Ŀ¼�ڵ��²����ӽڵ�");
		return false;
	}
	auto dir = (DirNode*)node;
	auto child_find = dir->FindChildByName(new_child->GetName());
	if (nullptr != child_find)
	{
		Log::LogWarn(L"�Ƿ����������Բ����Ѵ��ڵ��ӽڵ�");
		return false;
	}
	dir->AppendChild(new_child);
	new_child->SetParent(node);
	return true;
}
