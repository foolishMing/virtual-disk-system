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
	Log::LogInfo(L"node tree manager is created.");
}

void NodeTreeManager::Destroy()
{
	if (nullptr != m_tree) //删除文件树
	{
		m_tree->Destroy();
		m_tree = nullptr;
	}
	if (m_drivens.size() != 0) //删除驱动列表里的指针
	{
		m_drivens.clear();	
	}
	Log::LogInfo(L"node tree manager is destroyed.");
}

string_local NodeTreeManager::GetCurrentPath() const
{
	assert(nullptr != m_tree);
	BaseNode* node = m_working_dir;
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

void NodeTreeManager::PrintDirectoryInfo(BaseNode* node)//打印目录信息
{
	assert(nullptr != node);
	assert(NodeType::Directory == node->GetType());
	if (NodeType::Directory != node->GetType())
	{
		Log::LogWarn(L"非法操作：尝试打印非目录节点的目录信息");
		return;
	}
	auto dir = static_cast<DirNode*>(node);
	int file_cnt = 0;//文件数量
	int dir_cnt = 0;//目录数量
	size_t tot_size;//总大小
	//遍历子节点，统计并打印节点信息{修改时间、节点类型、节点大小、节点名称}
	for (BaseNode* node : dir->Children())
	{
		if (NodeType::Directory == node->GetType())
		{
			dir_cnt++;
			Console::Write::PrintLine(L"<DIR>          " + node->GetName());
		}
		else
		{
			file_cnt++;
		}
	}
	//打印统计信息{文件数量、目录数量、总大小}
	//udpate ...
}

void NodeTreeManager::InitDrivers()
{
	assert(nullptr != m_tree);
	for (auto item : m_driver_name_vec)
	{
		auto node = new DirNode(item);
		m_tree->InsertNode(m_tree->GetRoot(), node);
		m_drivens.push_back(node);
	}
	m_cur_driven = m_drivens[0];
	m_working_dir = m_cur_driven;
}
