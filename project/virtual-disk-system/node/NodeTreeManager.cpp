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
	if (!m_drivens.empty()) //ɾ�������б����ָ��
	{
		m_drivens.clear();	
	}
	Log::LogInfo(L"node tree manager is destroyed.");
}


string_local NodeTreeManager::GetCurrentPath() const
{
	assert(nullptr != m_tree);
	BaseNode* node = m_working_dir;
	auto ret = GetPathByNode(node);
	return ret;
}


void NodeTreeManager::PrintDirectoryNodeInfo(BaseNode* node)
{
	assert(node != nullptr);
	assert(node->GetType() == NodeType::Directory);
	Console::Write::PrintLine(
		StringTools::TimeStampToDateTimeString(node->GetLatestModifiedTimeStamp()) +
		L"    <DIR>          " +
		node->GetName());
}


void NodeTreeManager::PrintFileNodeInfo(BaseNode* node)
{
	assert(node != nullptr);
	assert(node->GetType() != NodeType::Directory);
	Console::Write::PrintLine(
		StringTools::TimeStampToDateTimeString(node->GetLatestModifiedTimeStamp()) +
		L"                   " +
		node->GetName());
}


//<udpate ...>
//δ��ɣ�������ݸ�ʽ��
//δ��ɣ�������ֵ�����޸�ΪDirInfo
//is_ad == true,ֻ��ӡ��Ŀ¼�ڵ�
void NodeTreeManager::PrintDirectoryInfo(BaseNode* dir, bool is_ad)//��ӡĿ¼��Ϣ
{
	assert(nullptr != dir);
	assert(NodeType::Directory == dir->GetType());
	DirNode* cur_dir = static_cast<DirNode*>(dir);
	Console::Write::PrintLine(L"");
	Console::Write::PrintLine(GetPathByNode(cur_dir) + L" ��Ŀ¼");
	Console::Write::PrintLine(L"");
	int file_cnt = 0;//�ļ�����
	int dir_cnt = 0;//Ŀ¼����
	size_t tot_size = 0;//�ܴ�С
	//�����ӽڵ㣬ͳ�Ʋ���ӡ�ڵ���Ϣ{�޸�ʱ�䡢�ڵ����͡��ڵ��С���ڵ�����}
	for (BaseNode* cur_node : cur_dir->Children())
	{
		tot_size += cur_node->GetSize();
		//��ӡ��Ŀ¼�ڵ�
		if (NodeType::Directory == cur_node->GetType())
		{
			dir_cnt++;
			PrintDirectoryNodeInfo(cur_node);
		}
		//��ӡ���ļ��ڵ�
		else if(!is_ad)
		{
			file_cnt++;
			PrintFileNodeInfo(cur_node);
		}
	}
	//��ӡͳ����Ϣ{�ļ�������Ŀ¼�������ܴ�С}
	Console::Write::Print(std::to_wstring(file_cnt) + L" ���ļ� ");
	Console::Write::PrintLine(std::to_wstring(tot_size) + L" �ֽ�");
	Console::Write::PrintLine(std::to_wstring(dir_cnt) + L" ��Ŀ¼");
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
//1������·���ĵ�һ��tokenһ�����̷���C:��
bool NodeTreeManager::IsAbsolutePath(const std::vector<string_local>& tokens)
{
	if (tokens.empty())
	{
		return false;
	}
	if (m_cur_driven->GetName() == tokens[0])	
	{
		return true;
	}
	return false;
}


string_local NodeTreeManager::GetPathByNode(BaseNode* node)const
{
	string_local ret = {};
	auto cur_node = node;
	while (cur_node != nullptr && cur_node != m_cur_driven)
	{
		ret = CharSet::char_backslash + cur_node->GetName() + ret;
		cur_node = cur_node->GetParent();
	}
	ret = m_cur_driven->GetName() + ret;
	return ret;
}


bool NodeTreeManager::IsPathExist(const std::vector<string_local>& tokens)
{
	BaseNode* target_node = FindNodeByTokensInternal(tokens);
	return (target_node != nullptr) ? true : false;
}


bool NodeTreeManager::isAncestor(BaseNode* pre_node, BaseNode* next_node)
{
	auto cur_node = next_node;
	while (cur_node != nullptr && cur_node != m_tree->GetRoot())
	{
		if (cur_node == pre_node) return true;
		cur_node = cur_node->GetParent();
	}
	return false;
}


//<update ...>
//-constraints
//1�������Ǿ���·�������·������Ҫ�����ж�
//2��tokenΪ.��..�ǺϷ���
//3��tokenΪ..ʱ����Ҫ�жϿ�ָ��
//4����������ҵ�β�ڵ���.��..Ӧ���������⴦��
//5�������ݣ�������������ڵ���������ڵ�
BaseNode* NodeTreeManager::FindNodeByTokensInternal(const std::vector<string_local>& tokens)
{
	BaseNode* target_node = nullptr;
	auto cur_dir = static_cast<DirNode*>((IsAbsolutePath(tokens)) ? m_tree->GetRoot() : m_working_dir);
	const int tail = static_cast<int>(tokens.size());
	//����Ŀ¼�ڵ�
	for (int index = 0; index < tail - 1; index++)
	{
		string_local cur_name = tokens[index];
		DirNode* next_dir = nullptr;
		if (Constant::gs_cur_dir_token == cur_name) continue;//.
		if (Constant::gs_parent_dir_token == cur_name)//..
		{
			BaseNode* parent = cur_dir->GetParent();
			if (nullptr == parent || m_tree->GetRoot() == parent) return nullptr;//���ڵ㲻����
			next_dir = static_cast<DirNode*>(parent);
		}
		//���Һ�̽ڵ�
		for (BaseNode* child : cur_dir->Children())
		{
			if (child->GetName() != cur_name) continue;
			if (NodeType::Directory != child->GetType()) return nullptr;//���ҵ���ͬ���ļ����޷������������
			next_dir = static_cast<DirNode*>(child);
			break;
		}
		if (nullptr == next_dir) return nullptr; //Ŀ��ڵ㲻����
		//���µ�ǰ����Ŀ¼
		cur_dir = next_dir;
	}
	auto cur_name = tokens.back();
	//Ŀ��ڵ�Ϊ��ǰ�ڵ�.������֮
	if (Constant::gs_cur_dir_token == cur_name)
	{
		return cur_dir;
	}
	//Ŀ��ڵ�Ϊ���ڵ�..������֮
	if (Constant::gs_parent_dir_token == cur_name)
	{
		auto parent = cur_dir->GetParent();
		if (nullptr == parent || m_tree->GetRoot() == parent)
		{
			return nullptr;
		}
		return parent;
	}
	//Ŀ��ڵ�Ϊ�ӽڵ㣬����֮
	for (auto child : cur_dir->Children())
	{
		if (child->GetName() == cur_name)
		{
			return child;
		}
	}
	return nullptr;
}


//���ж��ܷ���cur_dir�»�ȡ����tokenΪ���Ľڵ�
//���Ŀ��ڵ����ļ��ڵ㣬��mkdirʧ�ܣ�����ѭ��
//��cur_dir�´�����tokenΪ���Ľڵ�
//������ֱ���ڸ�Ŀ¼root�´����½ڵ㣨��Ϊ��Ŀ¼����������Ŀ¼������Ӧ����ֻ���ģ�
//��Ҫ��.��..token�������⴦��
bool NodeTreeManager::MkdirByTokens(const std::vector<string_local>& tokens)
{
	DirNode* cur_dir = static_cast<DirNode*>((IsAbsolutePath(tokens)) ? m_tree->GetRoot() : m_working_dir);
	std::queue<string_local, std::deque<string_local>> q(std::deque<string_local>(tokens.begin(),tokens.end()));
	while (!q.empty())
	{
		string_local token = q.front();
		q.pop();
		BaseNode* next_node = nullptr;
		//��ǰĿ¼
		if (Constant::gs_cur_dir_token == token) continue;
		//�ϼ�Ŀ¼
		if (Constant::gs_parent_dir_token == token)
		{
			next_node = cur_dir->GetParent();
			if (nullptr == next_node) return false;
		}
		//��cur_dir�²���Ŀ��ڵ�
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
		//��cur_dir�²���Ŀ��ڵ㣬������cur_dir
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


bool NodeTreeManager::ChangeDirByTokens(const std::vector<string_local>& tokens)
{
	//��ʾ����Ŀ¼
	if (tokens.empty())
	{
		Console::Write::PrintLine(GetCurrentPath());
		return true;
	}
	//Ѱ��Ŀ��ڵ�
	BaseNode* target_node = FindNodeByTokensInternal(tokens);
	assert(nullptr != target_node);
	//Ŀ��ڵ��Ŀ¼�ڵ�
	if (NodeType::Directory != target_node->GetType())
	{
		return false;
	}
	//�л�·��
	m_working_dir = static_cast<DirNode*>(target_node);
	return true;
}


//������Թ���Ŀ¼�ϵĽڵ㣨�������ڵ�������ڵ㣩������
//�ж�dst_name�Ƿ�����
bool NodeTreeManager::RenameNodeByTokens(const std::vector<string_local>& tokens, string_local dst_name)
{
	assert(PathTools::IsTokensFormatLegal({ dst_name }) == true);//dst_name��֤�﷨��ȷ
	//����Ŀ��ڵ�
	BaseNode* target_node = FindNodeByTokensInternal(tokens);
	assert(nullptr != target_node);//target_node��֤��Ϊ��
	//�жϽڵ��Ƿ��ڵ�ǰ����·����
	if (isAncestor(target_node, m_working_dir))
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryPathAccessDenied);//error : �Ƿ�����
		return false;
	}
	//����Ŀ��ڵ�����Ŀ¼�µ������ڵ�
	DirNode* parent_dir = static_cast<DirNode*>(target_node->GetParent());
	for (const auto item : parent_dir->Children())
	{
		//����Ŀ��ڵ�
		if (item == target_node) continue;
		//�ж��Ƿ����ͬ���ڵ�
		if (item->GetName() == dst_name)
		{
			Console::Write::PrintLine(ErrorTips::gsMemoryFileIsExist);//error : ���������ļ�
			return false;
		}
	}
	target_node->SetName(dst_name);
	return true;
}


//<udpate>
//δ��ɣ�ͳ�Ƶݹ��ӡ����Ŀ¼�������ļ���
//���tokensΪ�գ���ȼ���dir .
//����Ŀ��ڵ�
//���Ŀ��ڵ����ļ������ӡ�ļ���Ϣ
//ѡ��/ad��ӡ��Ŀ¼��Ϣ
//ѡ��/s�ݹ��ӡ��Ŀ¼���ļ���Ϣ
bool NodeTreeManager::DisplayDirNodeByTokensAndOptions(const std::vector<string_local>& tokens, const OptionSwitch& option_switch)	//�г�Ŀ¼�е��ļ�����Ŀ¼�б�
{
	BaseNode* target_node = target_node = FindNodeByTokensInternal(tokens);
	//target_node��֤��Ϊ��
	assert(nullptr != target_node);
	//��ӡĿ���ļ��ڵ���Ϣ
	if (NodeType::Directory != target_node->GetType())
	{
		PrintFileNodeInfo(target_node);
		return true;
	}
	//��ӡĿ��Ŀ¼�ڵ���Ŀ¼���ļ���Ϣ
	DirNode* cur_dir_node = static_cast<DirNode*>(target_node);
	std::queue<DirNode*> q = {};
	q.push(cur_dir_node);
	while (!q.empty())
	{
		DirNode* node = q.front();
		q.pop();
		//��ӡ��ǰĿ¼�µ���Ŀ¼���ļ���Ϣ
		PrintDirectoryInfo(node, option_switch._ad);
		//����Ŀ¼���ڴ���ӡĿ¼�ڵ������
		if (true == option_switch._s)
		{
			for (auto child : node->Children())
			{
				if (NodeType::Directory == child->GetType())
				{
					DirNode* child_dir = static_cast<DirNode*>(child);
					q.push(child_dir);
				}
			}
		}
	}
	return true;
}
