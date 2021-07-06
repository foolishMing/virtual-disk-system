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
	if (nullptr != m_tree) //ɾ���ļ���
	{
		m_tree->Destroy();
		m_tree = nullptr;
	}
	if (m_drivens.size() != 0) //ɾ�������б����ָ��
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

void NodeTreeManager::PrintDirectoryInfo(BaseNode* node)//��ӡĿ¼��Ϣ
{
	assert(nullptr != node);
	assert(NodeType::Directory == node->GetType());
	if (NodeType::Directory != node->GetType())
	{
		Log::LogWarn(L"�Ƿ����������Դ�ӡ��Ŀ¼�ڵ��Ŀ¼��Ϣ");
		return;
	}
	auto dir = static_cast<DirNode*>(node);
	int file_cnt = 0;//�ļ�����
	int dir_cnt = 0;//Ŀ¼����
	size_t tot_size;//�ܴ�С
	//�����ӽڵ㣬ͳ�Ʋ���ӡ�ڵ���Ϣ{�޸�ʱ�䡢�ڵ����͡��ڵ��С���ڵ�����}
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
	//��ӡͳ����Ϣ{�ļ�������Ŀ¼�������ܴ�С}
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
