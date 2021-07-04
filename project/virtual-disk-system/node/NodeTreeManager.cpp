#include "NodeTreeManager.h"


NodeTreeManager::NodeTreeManager()
{

}

NodeTreeManager::~NodeTreeManager()
{

}

void NodeTreeManager::Create()
{
	m_tree = new NodeTree();
	m_tree->Create();
	InitDrivers();
}

void NodeTreeManager::Destroy()
{
	if (nullptr != m_tree) //删除文件树
	{
		m_tree->Destroy();
		m_tree = nullptr;
	}
	if (m_drivers.size() != 0) //删除驱动列表里的指针
	{
		m_drivers.clear();	
	}
}

string_local NodeTreeManager::GetCurrentPath()
{
	assert(nullptr != m_tree);
	auto node = m_cur_dir;
	string_local ret = L"";
	while (node != nullptr)
	{
		auto parent = node->GetParent();
		if (parent != nullptr && parent != m_tree->GetRoot())
		{
			ret = L"/" + node->GetName() + ret;
		}
		else
		{
			ret = node->GetName() + ret;
		}
		node = parent;
	}
	return ret;
}

void NodeTreeManager::InitDrivers()
{
	assert(nullptr != m_tree);
	for (auto item : m_driver_name_vec)
	{
		auto node = new DirNode(item);
		m_tree->InsertNode(m_tree->GetRoot(), node);
		m_drivers.push_back(node);
	}
	m_cur_driver = m_drivers[0];
	m_cur_dir = m_cur_driver;
}
