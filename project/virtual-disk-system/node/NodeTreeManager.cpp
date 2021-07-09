#include "NodeTreeManager.h"
#include <deque>
#include <queue>

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
	assert(nullptr != node);
	assert(node->IsDirectory());
	Console::Write::PrintLine(
		StringTools::TimeStampToDateTimeString(node->GetLatestModifiedTimeStamp()) +
		L"    <DIR>          " +
		node->GetName());
}


void NodeTreeManager::PrintFileNodeInfo(BaseNode* node)
{
	assert(nullptr != node);
	assert(!node->IsDirectory());
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
	assert(dir->IsDirectory());
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
		if (cur_node->IsDirectory())
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
	if (m_cur_driven->IsNameEqualsTo(tokens[0]))
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


bool NodeTreeManager::IsAncestor(BaseNode* pre_node, BaseNode* next_node)
{
	auto cur_node = next_node;
	while (cur_node != nullptr && cur_node != m_tree->GetRoot())
	{
		if (cur_node == pre_node) return true;
		cur_node = cur_node->GetParent();
	}
	return false;
}

bool NodeTreeManager::IsParentDirToken(string_local& token)
{
	assert(!token.empty());
	return StringTools::IsEqual(token, Constant::gs_parent_dir_token);
}

bool NodeTreeManager::IsCurrentDirToken(string_local& token)
{
	assert(!token.empty());
	return StringTools::IsEqual(token, Constant::gs_cur_dir_token);
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
		string_local token = tokens[index];
		DirNode* next_dir = nullptr;
		//.
		if (IsCurrentDirToken(token))
		{
			continue;
		}
		//..
		if(IsParentDirToken(token))
		{
			BaseNode* parent = cur_dir->GetParent();
			if (nullptr == parent || m_tree->GetRoot() == parent) return nullptr;//���ڵ㲻����
			next_dir = static_cast<DirNode*>(parent);
		}
		//���Һ�̽ڵ�
		for (BaseNode* child : cur_dir->Children())
		{
			if (child->GetName() != token) continue;
			if (!child->IsDirectory()) // Ŀ��ڵ㲻��Ŀ¼���޷���������
			{
				return nullptr;
			}
			next_dir = static_cast<DirNode*>(child);
			break;
		}
		if (nullptr == next_dir) return nullptr; //Ŀ��ڵ㲻����
		//���µ�ǰ����Ŀ¼
		cur_dir = next_dir;
	}
	auto last_token = tokens.back();
	//Ŀ��ڵ�Ϊ��ǰ�ڵ�.������֮
	if (IsCurrentDirToken(last_token))
	{
		return cur_dir;
	}
	//Ŀ��ڵ�Ϊ���ڵ�..������֮
	if (IsParentDirToken(last_token))
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
		if (child->IsNameEqualsTo(last_token))
		{
			return child;
		}
	}
	return nullptr;
}


//<update>
//����ɣ���·������ʧ�ܣ�����Ҫɾ�����β����½������нڵ�
//���ж��ܷ���cur_dir�»�ȡ����tokenΪ���Ľڵ�
//���Ŀ��ڵ����ļ��ڵ㣬��mkdirʧ�ܣ�����ѭ��
//��cur_dir�´�����tokenΪ���Ľڵ�
//������ֱ���ڸ�Ŀ¼root�´����½ڵ㣨��Ϊ��Ŀ¼����������Ŀ¼������Ӧ����ֻ���ģ�
//��Ҫ��.��..token�������⴦��
//�����ڵ�ʱ����Ҫ�ж�token�Ƿ�Ϸ�
bool NodeTreeManager::MkdirByTokens(const std::vector<string_local>& tokens)
{
	DirNode* cur_dir = static_cast<DirNode*>((IsAbsolutePath(tokens)) ? m_tree->GetRoot() : m_working_dir);
	std::queue<string_local, std::deque<string_local>> q(std::deque<string_local>(tokens.begin(),tokens.end()));
	BaseNode* temp_node_root = nullptr;//���β��������ĵ�һ��(��ʱ)�ڵ�
	while (!q.empty())
	{
		string_local token = q.front();
		q.pop();
		BaseNode* next_node = nullptr;
		//��ǰĿ¼
		if (IsCurrentDirToken(token))
		{
			continue;
		}
		//�ϼ�Ŀ¼
		if (IsParentDirToken(token))
		{
			next_node = cur_dir->GetParent();
			if (nullptr == next_node) return false;
		}
		//��cur_dir�²���Ŀ��ڵ�
		for (auto child : cur_dir->Children())
		{
			if(child->IsNameEqualsTo(token))
			{
				next_node = child;
				break;
			}
		}
		//��Ŀ��ڵ��Ѵ���
		if (nullptr != next_node)
		{
			//���Ŀ��ڵ���Ŀ¼�ڵ㣬����µ�ǰ�ڵ㣬������ѯ
			if (next_node->IsDirectory())
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
			if (!PathTools::IsTokensFormatLegal({ token }))
			{
				Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//�ļ���Ŀ¼��������﷨����
				//����ʧ�ܣ�ɾ�����β����½������нڵ�
				if (nullptr != temp_node_root)
				{
					m_tree->DeleteNode(temp_node_root);
				}
				return false;
			}
			next_node = new DirNode(token);
			bool ok = m_tree->InsertNode(cur_dir, next_node);
			if (!ok)
			{
				return false;
			}
			if (nullptr == temp_node_root)
			{
				temp_node_root = next_node;
			}
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
	if (!target_node->IsDirectory())
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
	assert(PathTools::IsTokensFormatLegal({ dst_name }));//dst_name��֤�﷨��ȷ
	//����Ŀ��ڵ�
	BaseNode* target_node = FindNodeByTokensInternal(tokens);
	assert(nullptr != target_node);//target_node��֤��Ϊ��
	//�жϽڵ��Ƿ��ڵ�ǰ����·����
	if (IsAncestor(target_node, m_working_dir))
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
		if(item->IsNameEqualsTo(dst_name))
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
bool NodeTreeManager::DisplayDirNodeByTokensAndOptions(const std::vector<string_local>& tokens, const OptionSwitch& option_switch)	
{
	BaseNode* target_node = target_node = FindNodeByTokensInternal(tokens);
	//target_node��֤��Ϊ��
	assert(nullptr != target_node);
	//Ŀ��ڵ�Ϊ�ļ��ڵ㣬��ӡ��Ϣ
	if (!target_node->IsDirectory())
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
		if (option_switch._s)
		{
			for (auto child : node->Children())
			{
				if (child->IsDirectory())
				{
					DirNode* child_dir = static_cast<DirNode*>(child);
					q.push(child_dir);
				}
			}
		}
	}
	return true;
}


//ѡ��/s:�ݹ��Ƴ�Ŀ¼�µ�������Ŀ¼���ļ�
//����Ŀ¼�ϵĽڵ㲻�ܱ�ɾ��
//��pathָ���Ŀ¼�ڵ㣬���޷�ɾ��
ReturnType NodeTreeManager::RemoveDirByTokensAndOptions(const std::vector<string_local>& tokens, const OptionSwitch& option_switch)
{
	BaseNode* target_node = FindNodeByTokensInternal(tokens);
	assert(target_node != nullptr);
	//��Ŀ¼�ڵ��޷�ɾ��
	if (!target_node->IsDirectory())
	{
		return ReturnType::DirNameIsInvalid;
	}
	//����Ŀ¼�ϵĽڵ��޷�ɾ��
	if (IsAncestor(target_node, m_working_dir))
	{
		return ReturnType::AccessDenied;
	}
	DirNode* cur_dir = static_cast<DirNode*>(target_node);
	//��Ŀ¼��ֱ��ɾ��
	if (cur_dir->Children().empty())
	{
		bool ok = m_tree->DeleteNode(cur_dir);
		return ok ? ReturnType::Success : ReturnType::UnExpectedException;
	}
	//�ǿ�Ŀ¼���ж��Ƿ���/s����
	if (true != option_switch._s)
	{
		return ReturnType::MemoryDirIsNotEmpty;
	}
	//�ݹ�ɾ��
	else
	{
		bool ok = m_tree->DeleteNode(cur_dir);
		return ok ? ReturnType::Success : ReturnType::UnExpectedException;
	}
}


//<update>
//-output
//��ӡ�����Ƶ��ļ�·��
//ͳ�Ƹ��Ƶ������ļ�
ReturnType NodeTreeManager::CopyFromDisk(const string_local& src_path, const std::vector<string_local>& dst_path_tokens, const OptionSwitch& option_switch)
{
	stat_local stat;
	if (0 != StatLocal(src_path, &stat))
	{
		return ReturnType::DiskPathIsNotFound;//��ʵ����·��������
	}
	BaseNode* target_node = FindNodeByTokensInternal(dst_path_tokens);
	assert(nullptr != target_node);
	bool is_cover_all = false;
	//�����Ƿ�Ĭ(ȫ��)����
	if (option_switch._y == true)
	{
		is_cover_all = true;
	}
	//����Ŀ¼�µ��ļ����������
	if (stat.st_mode & S_IFDIR)
	{

	}
	//�����ļ����������
	else if(stat.st_mode & S_IFREG)
	{
		
	}
	else
	{
		return ReturnType::DiskPathIsNotDirectoyOrFile;//��ʵ����·���Ȳ���Ŀ¼Ҳ�����ļ�
	}
	return ReturnType::Success;
}


//<update>
//-output
//��ӡ�����Ƶ��ļ�·��
//ͳ�Ƹ��Ƶ������ļ�
ReturnType NodeTreeManager::CopyFromMemory(const std::vector<string_local>& src_path_tokens, const std::vector<string_local>& dst_path_tokens, const OptionSwitch& option_switch)
{
	BaseNode* src_node = FindNodeByTokensInternal(src_path_tokens);
	BaseNode* dst_node = FindNodeByTokensInternal(dst_path_tokens);
	assert(nullptr != src_node && nullptr != dst_node);
	int32_t copy_count = 0;
	bool is_cover_all = false;
	bool is_fuzzy_match = false;
	//�����Ƿ�Ĭ(ȫ��)����
	if (option_switch._y == true)
	{
		is_cover_all = true;
	}
	//�����Ƿ�ģ��ƥ��
	if(StringTools::HasWildcard(src_path_tokens))

	return ReturnType::Success;
}
