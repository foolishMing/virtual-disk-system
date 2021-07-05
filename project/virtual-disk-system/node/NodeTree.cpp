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
		//udpate.. д������־,��ָ��
		return;
	}
	if (node->GetType() != NodeType::Directory)
	{
		//udpate.. д������־����Ŀ¼�ڵ�
		return;
	}
	auto dir = (DirNode*)node;
	auto child_find = dir->FindChildByName(new_child->GetName());
	if (nullptr != child_find)
	{
		//update.. д������־���ӽڵ��Ѵ���
	}
	dir->AppendChild(new_child);
	new_child->SetParent(node);
}
