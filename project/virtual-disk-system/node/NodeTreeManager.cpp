#include "NodeTreeManager.h"
#include <queue>
#include <deque>

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
	InitDrivens();
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

//<udpate ...>
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
}

void NodeTreeManager::InitDrivens()
{
	assert(nullptr != m_tree);
	for (auto item : m_driver_tokens)
	{
		auto node = new DirNode(item);
		m_tree->InsertNode(m_tree->GetRoot(), node);
		m_drivens.push_back(node);
	}
	m_cur_driven = m_drivens[0];
	m_working_dir = m_cur_driven;
}


//-constraints:
//1������·���ĵ�һ��tokenһ�����̷�
bool NodeTreeManager::IsAbsolutePath(const std::vector<string_local>& tokens)
{
	if (0 == tokens.size())
	{
		return false;
	}
	if (m_cur_driven->GetName() == tokens[0])	
	{
		return true;
	}
	return false;
}


//<update ...>
//-constraints
//1�������Ǿ���·�������·������Ҫ�����ж�
//2��tokenΪ.��..�ǺϷ���
//3��tokenΪ..ʱ����Ҫ�жϿ�ָ��
//4����������ҵ�β�ڵ���.��..Ӧ���������⴦��
bool NodeTreeManager::IsPathExist(const std::vector<string_local>& tokens)
{
	bool ret = FindNodeByTokens(tokens);
	return ret;
}


bool NodeTreeManager::FindNodeByTokens(const std::vector<string_local>& tokens, BaseNode* target_node)
{
	auto cur_dir = static_cast<DirNode*>((IsAbsolutePath(tokens)) ? m_tree->GetRoot() : m_working_dir);
	int tail = tokens.size();
	//����Ŀ¼�ڵ�
	for (int index = 0; index < tail - 1; index++)
	{
		string_local cur_name = tokens[index];
		DirNode* next_dir = nullptr;
		if (Constant::gs_cur_dir_token == cur_name) continue;//.
		if (Constant::gs_parent_dir_token == cur_name)//..
		{
			BaseNode* parent = cur_dir->GetParent();
			if (nullptr == parent) return false;//���ڵ㲻����
			next_dir = static_cast<DirNode*>(parent);
		}
		//���Һ�̽ڵ�
		for (BaseNode* child : cur_dir->Children())
		{
			if (child->GetName() != cur_name) continue;
			if (NodeType::Directory != child->GetType()) return false;//���ҵ���ͬ���ļ����޷������������
			next_dir = static_cast<DirNode*>(child);
			break;
		}
		if (nullptr == next_dir) return false; //Ŀ��ڵ㲻����
		//���µ�ǰ����Ŀ¼
		cur_dir = next_dir;
	}
	//����β�ڵ�
	auto cur_name = tokens.back();
	if (Constant::gs_cur_dir_token == cur_name)return true;//.
	if (Constant::gs_parent_dir_token == cur_name)//..
	{
		auto parent = cur_dir->GetParent();
		return (nullptr == parent) ? false : true;
	}
	bool is_find = false;
	for (auto child : cur_dir->Children())
	{
		if (child->GetName() == cur_name)
		{
			target_node = child;//�ҵ���Ŀ��ڵ㣬����֮
			is_find = true;
			break;
		}
	}
	return is_find;
}


//���ж��ܷ���cur_dir�»�ȡ����tokenΪ���Ľڵ�
//���Ŀ��ڵ����ļ��ڵ㣬��mkdirʧ�ܣ�����ѭ��
//��cur_dir�´�����tokenΪ���Ľڵ�
//������ֱ���ڸ�Ŀ¼root�´����½ڵ㣨��Ϊ��Ŀ¼����������Ŀ¼������Ӧ����ֻ���ģ�
bool NodeTreeManager::MkdirByTokens(const std::vector<string_local>& tokens)
{
	DirNode* cur_dir = static_cast<DirNode*>((IsAbsolutePath(tokens)) ? m_tree->GetRoot() : m_working_dir);
	std::queue<string_local, std::deque<string_local>> q(std::deque<string_local>(tokens.begin(),tokens.end()));
	while (!q.empty())
	{
		string_local token = q.front();
		q.pop();
		BaseNode* next_node = nullptr;
		//����.
		if (Constant::gs_cur_dir_token == token) continue;
		//����..
		if (Constant::gs_parent_dir_token == token)
		{
			next_node = cur_dir->GetParent();
			if (nullptr == next_node) return false;
		}
		//���жϴ�����ڵ��Ƿ���cur_dir��
		for (auto child : cur_dir->Children())
		{
			if (child->GetName() == token)
			{
				next_node = child;
				break;
			}
		}
		//��Ŀ��ڵ��Ѵ���
		if (nullptr != next_node)
		{
			//���Ŀ��ڵ���Ŀ¼�ڵ㣬����µ�ǰ�ڵ㣬������ѯ
			if (NodeType::Directory == next_node->GetType())
			{
				cur_dir = static_cast<DirNode*>(next_node);
				continue;
			}
			//���Ŀ��ڵ����ļ��ڵ㣬��mkdirʧ�ܣ�����ѭ��
			else
			{
				return false;
			}
		}
		//��cur_dir�´�����tokenΪ���Ľڵ㣬�����µ�ǰ�ڵ㣬������󴴽�
		else
		{
			if (cur_dir == m_tree->GetRoot()) //�������ڸ�Ŀ¼�´����µĽڵ�
			{
				return false;
			}
			next_node = new DirNode(token);
			bool ok = m_tree->InsertNode(cur_dir, next_node);
			if (!ok) return false;
			cur_dir = static_cast<DirNode*>(next_node);
		}
	}
	return true;
}


//���tokensΪ�գ���ȼ���dir .
//��ѯtokens����ӦĿ¼�ڵ��Ƿ����
//
bool NodeTreeManager::DisplayDirNodeByTokensAndOptions(const std::vector<string_local>& tokens, const OptionSwitch& option_switch)	//�г�Ŀ¼�е��ļ�����Ŀ¼�б�
{
	return false;
}
