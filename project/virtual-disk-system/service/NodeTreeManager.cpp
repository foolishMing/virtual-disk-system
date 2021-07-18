#include "NodeTreeManager.h"
#include <deque>
#include <queue>
#include <fstream>



NodeTreeManager::NodeTreeManager()
{

}

NodeTreeManager::~NodeTreeManager()
{

}


void NodeTreeManager::Create()
{
	m_tree = new NodeTree();
	m_tree->Create(new DirNode(TEXT("root")));
	InitDrivens();
	Log::Info(TEXT("node tree manager is created."));
}


void NodeTreeManager::Destroy()
{
	if (nullptr != m_tree) //ɾ���ļ���
	{
		m_tree->Destroy();
		m_tree = nullptr;
	}
	Log::Info(TEXT("node tree manager is destroyed."));
}


bool NodeTreeManager::Load(NodeTree* tree)
{
	assert(tree);
	auto old_tree = m_tree;
	//��������
	m_tree = tree;
	//�����̷�
	bool init_success = InitDrivens();
	if (!init_success)
	{
		Console::Write::PrintLine(TEXT("������̼���ʧ��,�Ҳ���C:�̷�"));
		//�ָ�ԭ��
		m_tree = old_tree;
		return false;
	}
	//ɾ��ԭ��
	if (old_tree)
	{
		old_tree->Destroy();
	}
	Log::Info(TEXT("node tree manager is reloaded."));
	return true;
}


string_local NodeTreeManager::GetCurrentPath() const
{
	const auto& working_stack = m_working_stack;
	assert(m_tree && !working_stack.empty());
	string_local ret = {};
	for (const auto& item : working_stack)
	{
		ret += item->GetName();
		if (item != working_stack.back())
		{
			ret += CharSet::char_backslash;
		}
	}
	if (working_stack.size() == 1)
	{
		ret += CharSet::char_backslash;
	}
	return ret;
}


void NodeTreeManager::PrintFileNodeInfo(BaseNode* cur_node)
{
	assert(cur_node);
	//��ӡ�ļ��ڵ���Ϣ
	if (cur_node->IsFile())
	{
		auto file_node = dynamic_cast<FileNode*>(cur_node);
		std::wcout << StringTools::TimeStampToDateTimeString(file_node->GetLatestModifiedTimeStamp())
			<< TEXT("    ")
			<< std::right << std::setw(14) << std::to_wstring(file_node->GetSize())
			<< TEXT(" ") << file_node->GetName() << std::endl;
	}
	//��ӡ�ļ�������Ϣ
	else if(cur_node->IsFileLink())
	{
		auto link_node = dynamic_cast<SymlinkNode*>(cur_node);
		std::wcout << StringTools::TimeStampToDateTimeString(link_node->GetLatestModifiedTimeStamp())
			<< TEXT("    ")
			<< std::left << std::setw(15) << TEXT("<SYMLINK>")
			<< link_node->GetName() 
			<< TEXT(" [") << link_node->GetTargetPath() << TEXT("]") << std::endl;
	}
	//��ӡĿ¼������Ϣ
	else
	{
		auto link_node = dynamic_cast<SymlinkNode*>(cur_node);
		std::wcout << StringTools::TimeStampToDateTimeString(link_node->GetLatestModifiedTimeStamp())
			<< TEXT("    ")
			<< std::left << std::setw(15) << TEXT("<SYMLINKD>")
			<< link_node->GetName() 
			<< TEXT(" [") << link_node->GetTargetPath() << TEXT("]") << std::endl;
	}
}


//<udpate ...>
//δ��ɣ�������ֵ�����޸�ΪDirInfo
//is_ad == true,ֻ��ӡ��Ŀ¼�ڵ�
//����Ŀ¼�������̷�������Ҫ��ӡ.��..
void NodeTreeManager::PrintDirectoryInfo(BaseNode* dir, StatisticInfo& g_info, const OptionSwitch& option)//��ӡĿ¼��Ϣ
{
	assert(dir && dir->IsDirectory());
	StatisticInfo info;
	DirNode* cur_dir = dynamic_cast<DirNode*>(dir);
	Console::Write::PrintLine(TEXT(""));
	Console::Write::PrintLine(GetTruePathOfNode(cur_dir) + TEXT(" ��Ŀ¼"));
	Console::Write::PrintLine(TEXT(""));
	auto left_margin = 15;
	//���ڷ��̷��ڵ㣬��ӡ.��..Ŀ¼
	if (!IsSameNode(cur_dir, m_cur_driven))
	{
		std::wcout << StringTools::TimeStampToDateTimeString(dir->GetLatestModifiedTimeStamp())
			<< TEXT("    ")
			<< std::left << std::setw(left_margin) << TEXT("<DIR>")
			<< Constant::gs_cur_dir_token << std::endl;
		std::wcout << StringTools::TimeStampToDateTimeString(dir->GetLatestModifiedTimeStamp())
			<< TEXT("    ")
			<< std::left << std::setw(left_margin) << TEXT("<DIR>")
			<< Constant::gs_parent_dir_token << std::endl;
	}

	auto children = cur_dir->Children();
	info.tot_cnt = children.size();
	//�����ӽڵ㣬ͳ�Ʋ���ӡ�ڵ���Ϣ{�޸�ʱ�䡢�ڵ����͡��ڵ��С���ڵ�����}
	for (BaseNode* cur_node : children)
	{
		//ͳ��Ŀ¼��ռ�ռ��С
		info.tot_size += cur_node->GetSize();
		//��ӡĿ¼�ڵ���Ϣ
		if (cur_node->IsDirectory())
		{
			info.dir_cnt++;
			auto dir = dynamic_cast<DirNode*>(cur_node);
			std::wcout << StringTools::TimeStampToDateTimeString(dir->GetLatestModifiedTimeStamp()) 
				<< TEXT("    ") 
				<< std::left << std::setw(left_margin) << TEXT("<DIR>")
				<< dir->GetName() << std::endl;
			continue;
		}
		if (option._ad) continue;
		//��ӡ�ļ��ڵ���Ϣ
		PrintFileNodeInfo(cur_node);
	}
	g_info += info;
	//��ӡ��ǰĿ¼ʱ����Ҫ��.��..��Ŀ¼��Ϣͳ������
	if (!IsSameNode(cur_dir, m_cur_driven))
	{
		info.tot_cnt += 2;
		info.dir_cnt += 2;
	}
	PrintStatisticInfo(info, option);
}


// /s�ݹ�ѡ���£�����ӡĿ¼��ͳ����Ϣ
void NodeTreeManager::PrintStatisticInfo(StatisticInfo& info, const OptionSwitch& option)
{
	const int left_margin = 15;
	//{�ļ����������ֽ�}
	int64_t file_cnt = (option._ad) ? static_cast<int64_t>(0) : (info.tot_cnt - info.dir_cnt);
	std::wcout << std::right << std::setw(left_margin) << std::to_wstring(file_cnt) << TEXT(" ���ļ�")
		<< std::right << std::setw(left_margin) << std::to_wstring(info.tot_size) << TEXT("  �ֽ�") << std::endl;
	if (!option._s)
	{
		//{Ŀ¼�����������ֽ�}
		std::wcout << std::right << std::setw(left_margin) << std::to_wstring(info.dir_cnt) << TEXT(" ��Ŀ¼")
			<< std::right << std::setw(left_margin) << std::to_wstring(getTotalSystemMemory()) << TEXT("  �����ֽ�") << std::endl;
	}	
}


bool NodeTreeManager::InitDrivens()
{
	assert(m_tree);
	auto root_dir = dynamic_cast<DirNode*>(m_tree->GetRoot());
	//��ʼ���̷�
	for (const auto& driven_name : m_driven_tokens)
	{
		if (root_dir->ContainsChild(driven_name))continue;
		m_tree->InsertNode(root_dir, new DirNode(driven_name));
	}
	m_cur_driven = dynamic_cast<DirNode*>(root_dir->FindChildByName(m_driven_tokens[0]));
	//��ʼ������·��
	SetMyWorkingStack({ m_cur_driven });
	return (m_cur_driven) ? true : false;
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


string_local NodeTreeManager::GetTruePathOfNode(BaseNode* node)const
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


bool NodeTreeManager::IsTrueAncestor(BaseNode* pre_node, BaseNode* next_node)
{
	auto cur_node = next_node;
	while (nullptr != cur_node && !IsSameNode(cur_node, m_tree->GetRoot()))
	{
		if (IsSameNode(cur_node,pre_node)) return true;
		cur_node = cur_node->GetParent();
	}
	return false;
}

DirNode* NodeTreeManager::GetTrueDirNode(BaseNode* node)
{
	if (nullptr == node || node->IsFile() || node->IsFileLink())
	{
		return nullptr;
	}
	if (node->IsDirectory())
	{
		return dynamic_cast<DirNode*>(node);
	}
	auto dir = FindSymlinkedDirectory(dynamic_cast<SymlinkNode*>(node));
	Log::Info(TEXT("symlink : ") + node->GetName() + TEXT("->") + dir->GetName());
	return dir;
}

FileNode* NodeTreeManager::GetTrueFileNode(BaseNode* node)
{
	if (nullptr == node || node->IsDirectory() || node->IsDirectoryLink())
	{
		return nullptr;
	}
	if (node->IsFile())
	{
		return dynamic_cast<FileNode*>(node);
	}
	auto file = FindSymlinkedFile(dynamic_cast<SymlinkNode*>(node));
	return file;
}


bool NodeTreeManager::IsDirOrDirLink(BaseNode* node)
{
	if (nullptr == node || node->IsFile() || node->IsFileLink())
	{
		return false;
	}
	return true;
}



bool NodeTreeManager::IsParentDir(const string_local& token)
{
	assert(!token.empty());
	return StringTools::IsEqual(token, Constant::gs_parent_dir_token);
}


bool NodeTreeManager::IsCurrentDir(const string_local& token)
{
	assert(!token.empty());
	return StringTools::IsEqual(token, Constant::gs_cur_dir_token);
}

void NodeTreeManager::SetMyWorkingStack(const std::deque<BaseNode*> working_stack)
{
	assert(!working_stack.empty());
	m_working_stack = working_stack;
} 

std::deque<BaseNode*> NodeTreeManager::GetTemporaryWorkingStackByTokens(const std::vector<string_local>& tokens)
{
	assert(!tokens.empty());
	std::deque<BaseNode*> working_stack;
	if (IsAbsolutePath(tokens))
	{
		working_stack = { m_cur_driven };
	}
	else
	{
		working_stack = GetMyWorkingStack();
	}
	return working_stack;
}

//���������ļ�
FileNode* NodeTreeManager::FindSymlinkedFile(SymlinkNode* node)
{
	return nullptr;
}

//���ڼ��߼����ǣ�link_path�����Ǿ���·��
DirNode* NodeTreeManager::FindSymlinkedDirectory(SymlinkNode* node)
{
	if (nullptr == node || !node->IsDirectoryLink())
	{
		return nullptr;	//error : �Ҳ���Ŀ¼����
	}
	Path path(node->GetTargetPath());
	auto tokens = path.Tokens();
	assert(IsAbsolutePath(tokens));
	//�Ӹ��ڵ㿪ʼ����
	DirNode* cur_dir = dynamic_cast<DirNode*>(m_tree->GetRoot());
	std::queue<string_local, std::deque<string_local>> q(std::deque<string_local>(tokens.begin(), tokens.end()));
	while (!q.empty())
	{
		if (nullptr == cur_dir)
		{
			return nullptr;//error : �Ҳ������ӵ�Ŀ¼
		}
		const string_local token = q.front();
		q.pop();
		//ƥ�䵱ǰĿ¼
		if (IsCurrentDir(token))
		{
			continue;
		}
		//ƥ��(����Ŀ¼��)��һ��Ŀ¼
		if (IsParentDir(token))
		{
			auto parent = cur_dir->GetParent();
			if (nullptr == parent)
			{
				return nullptr;
			}
			cur_dir = dynamic_cast<DirNode*>(parent);
			continue;
		}
		//ƥ����һ��Ŀ¼(������.����..)
		BaseNode* next_node = cur_dir->FindChildByName(token);
		//�սڵ���߸�Ŀ¼�ǲ��Ϸ���
		if (nullptr == next_node || IsSameNode(next_node, m_tree->GetRoot()))
		{
			return nullptr;//error : �Ҳ���Ŀ¼����
		}
		//��Ŀ¼����Ŀ¼���ӣ��ڵ��ǲ��Ϸ���
		if (!IsDirOrDirLink(next_node))
		{
			return nullptr; //error : �Ҳ���Ŀ¼����
		}
		//���뵽��һ��Ŀ¼�ڵ�
		if (next_node->IsDirectory())
		{
			cur_dir = dynamic_cast<DirNode*>(next_node);
			continue;
		}
		//��ȡĿ¼���Ӷ�Ӧ��Ŀ¼�ڵ�
		if (next_node->IsDirectoryLink())
		{
			cur_dir = FindSymlinkedDirectory(dynamic_cast<SymlinkNode*>(next_node));
		}
	}
	return cur_dir;
}


//<update ...>
//-constraints
//1�������Ǿ���·�������·������Ҫ�����ж�
//2��tokenΪ.��..�ǺϷ���
//3��tokenΪ..ʱ����Ҫ�жϿ�ָ��
//4����������ҵ�β�ڵ���.��..Ӧ���������⴦��
//5���������ӽڵ㣬��Ҫ�����������ӵ���ʵ�ڵ�
//6������Ǿ���·��������ʱ����ջ��ʼ��Ϊ�̷��������ʼ��Ϊ��ǰ����ջ
BaseNode* NodeTreeManager::FindTrueNodeByTokensInternal(const std::vector<string_local>& tokens, std::deque<BaseNode*>& working_stack)
{
	assert(!tokens.empty());
	std::queue<string_local, std::deque<string_local>> q(std::deque<string_local>(tokens.begin(), tokens.end()));
	//�����ȫ��·������Ĭ�ϲ�ƥ���̷�
	if (StringTools::IsEqual(q.front(), m_cur_driven->GetName()))
	{
		q.pop();
	}
	//���ع���·��
	if (q.empty())
	{
		return GetTrueDirNode(working_stack.back());
	}
	while (q.size() > 1)
	{
		if (working_stack.empty())
		{
			return nullptr; //error : �Ҳ���·��
		}
		const string_local token = q.front();
		q.pop();
		//ƥ�䵱ǰĿ¼
		if (IsCurrentDir(token))
		{
			continue;
		}
		//ƥ����һ��Ŀ¼
		if (IsParentDir(token))
		{
			working_stack.pop_back();
			if (working_stack.empty())
			{
				return nullptr; //error : �Ҳ���·��
			}
			continue;
		}
		//ƥ����һ��Ŀ¼
		DirNode* working_dir = GetTrueDirNode(working_stack.back());
		if (nullptr == working_dir)
		{
			return nullptr; //error : �Ҳ���·��
		}
		auto next_dir = working_dir->FindChildByName(token);
		if (!IsDirOrDirLink(next_dir))
		{
			return nullptr; // error : �Ҳ���·��
		}
		working_stack.emplace_back(next_dir);
		continue;
	}
	if (q.size() > 0)
	{
		const auto token = q.back();
		q.pop();
		//��ȡ��ǰ����Ŀ¼
		BaseNode* cur_node = working_stack.back();
		DirNode* cur_dir = nullptr;
		if (cur_node->IsDirectory())
		{
			cur_dir = dynamic_cast<DirNode*>(cur_node);
		}
		else //if(cur_node->IsDirectoryLink())
		{
			cur_dir = FindSymlinkedDirectory(dynamic_cast<SymlinkNode*>(cur_node));
		}
		//���ص�ǰĿ¼�ڵ�
		if (IsCurrentDir(token))
		{
			return cur_dir;
		}
		//������һ��Ŀ¼�ڵ�
		if (IsParentDir(token))
		{
			working_stack.pop_back();
			if (working_stack.empty())
			{
				return nullptr;
			}
			auto parent_node = working_stack.back();
			if (parent_node->IsDirectory())
			{
				return parent_node;
			}
			else //if (parent_node->IsDirectoryLink())
			{
				return FindSymlinkedDirectory(dynamic_cast<SymlinkNode*>(parent_node));
			}
		}
		//������һ��Ŀ¼���ļ��ڵ�
		for (const auto& item : cur_dir->Children())
		{
			//ģ��ƥ��
			if (!StringTools::IsStringFuzzyEqualTo(item->GetName(), token))
			{
				continue;
			}
			if (item->IsDirectory() || item->IsFile())
			{
				return item;
			}
			if (item->IsDirectoryLink())
			{
				return FindSymlinkedDirectory(dynamic_cast<SymlinkNode*>(item));
			}
			if (item->IsFileLink())
			{
				return FindSymlinkedFile(dynamic_cast<SymlinkNode*>(item));
			}
		}
	}
	return nullptr;
}


//<update>
//����ɣ���·������ʧ�ܣ������ѳɹ������Ľڵ�
//���ж��ܷ���cur_dir�»�ȡ����tokenΪ���Ľڵ�
//���Ŀ��ڵ����ļ��ڵ㣬��mkdirʧ�ܣ�����ѭ��
//��cur_dir�´�����tokenΪ���Ľڵ�
//������ֱ���ڸ�Ŀ¼root�´����½ڵ㣨��Ϊ��Ŀ¼����������Ŀ¼������Ӧ����ֻ���ģ�
//��Ҫ��.��..token�����ж�
//�����ڵ�ʱ����Ҫ�ж�token�Ƿ�Ϸ�
//��Ҫ����·���ϵ����ӽڵ�
//��Ҫ�ж���Ŀ¼���ļ��Ƿ����
ReturnType NodeTreeManager::MkdirByTokens(const std::vector<string_local>& tokens)
{
	//��ʼ������ջ
	std::deque<BaseNode*> temp_working_stack = GetTemporaryWorkingStackByTokens(tokens);
	std::queue<string_local, std::deque<string_local>> q(std::deque<string_local>(tokens.begin(),tokens.end()));
	//�����ȫ��·������Ĭ�ϲ�ƥ���̷�
	if (StringTools::IsEqual(q.front(), m_cur_driven->GetName()))
	{
		q.pop();
	}
	bool is_new_dir_created = false;
	//����·��������
	while (!q.empty())
	{
		if (temp_working_stack.empty())
		{
			return ReturnType::MemoryPathIsNotFound; //error : �Ҳ���·��
		}
		string_local token = q.front();
		q.pop();
		//ƥ�䵱ǰĿ¼
		if (IsCurrentDir(token))
		{
			continue;
		}
		//ƥ���ϼ�Ŀ¼
		if (IsParentDir(token))
		{
			temp_working_stack.pop_back();
			continue;
		}
		//ƥ����һ��Ŀ¼
		DirNode* cur_dir = GetTrueDirNode(temp_working_stack.back());
		if (cur_dir == nullptr)
		{
			return ReturnType::MemoryPathIsNotFound; //error : �Ҳ���·��
		}
		BaseNode* next_node = cur_dir->FindChildByName(token);
		//������Ŀ¼
		if (nullptr == next_node)
		{
			if (cur_dir == m_tree->GetRoot())
			{
				return ReturnType::UnExpectedException; //error : �������ڸ�Ŀ¼�´����µĽڵ�
			}
			if (!PathTools::IsTokensFormatLegal({ token }))
			{
				return ReturnType::MemoryPathNameIsIllegal;
			}
			next_node = new DirNode(token);
			bool ok = m_tree->InsertNode(cur_dir, next_node);
			if (!ok)
			{
				return ReturnType::UnExpectedException;//error : ��Ŀ¼����ʧ��
			}
			is_new_dir_created = true;
		}
		//�����һ��Ŀ¼�Ƿ����
		else if(nullptr == GetTrueDirNode(next_node))
		{
			return ReturnType::MemoryPathIsNotFound; // error : �Ҳ���·��
		}
		//�����Ŀ¼
		temp_working_stack.emplace_back(next_node);
	}
	if (temp_working_stack.empty())
	{
		return ReturnType::UnExpectedException;
	}
	//û�д����µĽڵ�
	if (!is_new_dir_created)
	{
		return ReturnType::MemoryPathIsExist;//error : ·���Ѵ���
	}
	return ReturnType::Success;
}


//1��Ҫ���ǵ�����·���ϴ���Ŀ¼���ӵ����
//2����·���б�Ϊ�գ���ӡ��ǰ·��
//3����֧��ģ��ƥ��
ReturnType NodeTreeManager::ChangeDirByTokens(const std::vector<string_local>& tokens)
{
	//��ʾ����Ŀ¼
	if (tokens.empty())
	{
		Console::Write::PrintLine(GetCurrentPath());
		return ReturnType::Success;
	}
	//��ʼ����ʱ����ջ
	std::deque<BaseNode*> temp_working_stack = GetTemporaryWorkingStackByTokens(tokens);
	//��ȡtoken������·��ƥ��
	std::queue<string_local, std::deque<string_local>> q(std::deque<string_local>(tokens.begin(), tokens.end()));
	//�����ȫ��·������Ĭ�ϲ�ƥ���̷�
	if (StringTools::IsEqual(q.front(), m_cur_driven->GetName()))
	{
		q.pop();
	}
	while (!q.empty())
	{
		if (temp_working_stack.empty())
		{
			return ReturnType::MemoryPathIsNotFound;//error : ·��������
		}
		const string_local token = q.front();
		q.pop();
		//ƥ�䵱ǰĿ¼
		if (IsCurrentDir(token))
		{
			continue;
		}
		//ƥ����һ��Ŀ¼
		if (IsParentDir(token))
		{
			temp_working_stack.pop_back();
			continue;
		}
		//ƥ����һ��Ŀ¼
		DirNode* cur_dir = GetTrueDirNode(temp_working_stack.back());
		auto next_node = cur_dir->FindChildByName(token);
		if (!IsDirOrDirLink(next_node))
		{
			return ReturnType::MemoryPathIsNotFound; //error : ·��������
		}
		temp_working_stack.emplace_back(next_node);
	}
	if (temp_working_stack.empty())
	{
		return ReturnType::MemoryPathIsNotFound; // error : ·��������
	}
	//����ȫ�ֹ���ջ
	SetMyWorkingStack(temp_working_stack);
	return ReturnType::Success;
}



//1����Ҫ�ж�·���Ƿ����
//2�����������ʵ����·���ϵĽڵ㣨�������ڵ�������ڵ㣩������
//3����Ҫ�ж�dst_name�Ƿ�����
//4����֧��ͨ���
ReturnType NodeTreeManager::RenameNodeByTokens(const std::vector<string_local>& tokens, string_local dst_name)
{
	assert(PathTools::IsTokensFormatLegal({ dst_name }));//dst_name��֤�﷨��ȷ
	assert(!tokens.empty());
	//����Ŀ��ڵ�������ʵĿ¼
	auto working_stack = GetTemporaryWorkingStackByTokens(tokens);
	string_local target_node_token = tokens.back();//Ŀ��ڵ��token
	DirNode* cur_dir = FindWhichDirectoryIsNodeUnderByTokens(tokens,working_stack);
	//Ŀ¼������
	if (cur_dir == nullptr)
	{
		return ReturnType::MemoryPathIsNotFound;//error : �Ҳ���·��
	}
	//�������һ��token����Ŀ��ڵ�
	BaseNode* target_node = nullptr;
	//target_node_token��.��..
	if (IsCurrentDir(target_node_token))
	{
		target_node = cur_dir;
	}
	else if(IsParentDir(target_node_token))
	{
		working_stack.pop_back();
		if (working_stack.empty())
		{
			return ReturnType::MemoryPathIsNotFound;
		}
		target_node = working_stack.back();//ע�⣬��������ӽڵ㣬��ȡ�䱾��������Ҫ��ȡ�����ӵ���ʵ����
	}
	//ƥ���ӽڵ�
	else
	{
		auto target_node = cur_dir->FindChildByName(target_node_token);
	}
	//Ŀ��ڵ㲻����,��Ϊ��Ŀ¼
	if (target_node == nullptr || IsSameNode(target_node, m_tree->GetRoot()))
	{
		return ReturnType::MemoryPathIsNotFound;//error : �Ҳ���·��
	}
	//�ж�Ŀ��ڵ��Ƿ��ڹ���·����
	DirNode* working_dir = GetTrueDirNode(m_working_stack.back());
	if (IsTrueAncestor(target_node, working_dir))
	{
		return ReturnType::AccessDenied;//error : �Ƿ�����
	}
	//�ж��Ƿ��������ļ�
	auto dst_node = cur_dir->FindChildByName(dst_name);
	if (nullptr != dst_node && !IsSameNode(target_node, dst_node))//�������Ҳ���ԭ�ļ�
	{
		return ReturnType::MemoryFileIsExist;
	}	
	//����������command���Ѿ�����dst_name�Ƿ�Ϸ���
	target_node->SetName(dst_name);
	return ReturnType::Success;
}


//<udpate>
//���Ŀ��ڵ����ļ������ӡ�ļ���Ϣ
//���Ŀ��ڵ���
//ѡ��/ad��ӡ��Ŀ¼��Ϣ
//ѡ��/s�ݹ��ӡ��Ŀ¼���ļ���Ϣ
// /s�£���ͳ��Ŀ¼��Ϣ
// /ad�£���ͳ���ļ���Ϣ
ReturnType NodeTreeManager::DisplayDirNodeByTokensAndOptions(const std::vector<string_local>& tokens, const OptionSwitch& option_switch)	
{
	assert(!tokens.empty());
	auto working_stack = GetTemporaryWorkingStackByTokens(tokens);
	BaseNode* target_node = FindTrueNodeByTokensInternal(tokens, working_stack);//directory or file
	if (nullptr == target_node)
	{
		return ReturnType::MemoryPathIsNotFound;//�Ҳ���·��
	}
	const bool is_recursive = option_switch._s;
	//Ŀ��ڵ�Ϊ�ļ��ڵ㣬��ӡ�ļ�
	if (target_node->IsFile())
	{
		PrintFileNodeInfo(target_node);
		return ReturnType::Success;
	}
	//��ӡĿ��Ŀ¼�ڵ���Ŀ¼���ļ���Ϣ
	DirNode* cur_dir_node = dynamic_cast<DirNode*>(target_node);
	std::queue<DirNode*> q = {};
	q.push(cur_dir_node);
	StatisticInfo g_info;
	while (!q.empty())
	{
		DirNode* dir = q.front();
		q.pop();
		//��ӡ��ǰĿ¼�µ���Ŀ¼���ļ���Ϣ
		PrintDirectoryInfo(dir, g_info, option_switch);
		if (!is_recursive)
		{
			break;//�ǵݹ飬��ֹ����
		}
		//����Ŀ¼���ڴ���ӡĿ¼�ڵ������
		for (auto child : dir->Children())
		{
			if (child->IsDirectory())
			{
				DirNode* child_dir = dynamic_cast<DirNode*>(child);
				q.push(child_dir);
			}
		}
	}
	if (is_recursive)
	{
		Console::Write::PrintLine(TEXT("\n     �����ļ�����:"));
		g_info.tot_size = cur_dir_node->GetSize();
		OptionSwitch os = option_switch;
		os._s = false;
		PrintStatisticInfo(g_info, os);
	}
	return ReturnType::Success;
}


//����Ŀ¼�ϵĽڵ㲻�ܱ�ɾ��
//ѡ��/s:�ݹ��Ƴ�Ŀ¼�µ�������Ŀ¼���ļ�
//��pathָ���ļ����ļ����ӣ����޷�ɾ��
//ɾ��Ŀ¼����ʱ��ֻӦ��ɾ���䱾������ɾ�������ӵ�Ŀ¼
ReturnType NodeTreeManager::RemoveDirByTokensAndOptions(const std::vector<string_local>& tokens, const OptionSwitch& option_switch)
{
	assert(!tokens.empty());
	const string_local target_node_token = tokens.back();
	//�ҵ�Ŀ��ڵ�����Ŀ¼
	auto temp_working_stack = GetTemporaryWorkingStackByTokens(tokens);
	DirNode* parent_dir = FindWhichDirectoryIsNodeUnderByTokens(tokens, temp_working_stack);
	if (nullptr == parent_dir)
	{
		return ReturnType::MemoryPathIsNotFound; //error : Ŀ��·��������
	}

	BaseNode* target_node = nullptr;
	if (IsCurrentDir(target_node_token))
	{
		target_node = parent_dir;
	}
	else if (IsParentDir(target_node_token))
	{
		temp_working_stack.pop_back();
		if (temp_working_stack.empty())
		{
			return ReturnType::MemoryPathIsNotFound;
		}
		target_node = temp_working_stack.back();
	}
	else
	{
		target_node = parent_dir->FindChildByName(target_node_token);
	}
	//��Ŀ¼���Ŀ¼����ɾ��
	if (nullptr == target_node || IsSameNode(target_node, m_tree->GetRoot()))
	{
		return ReturnType::MemoryPathIsNotFound; //error : Ŀ��·��������
	}
	//��Ŀ¼(��Ŀ¼����)�ڵ��޷�ɾ��
	if (!IsDirOrDirLink(target_node))
	{
		return ReturnType::DirNameIsInvalid;
	}
	//��ǰ����Ŀ¼�ϵĽڵ��޷�ɾ��
	DirNode* cur_working_dir = GetTrueDirNode(GetMyWorkingStack().back());
	if (IsTrueAncestor(target_node, cur_working_dir))
	{
		return ReturnType::AccessDenied;
	}
	//�����Ŀ¼���ӣ���ֻɾ���䱾��
	if (target_node->IsDirectoryLink())
	{
		bool ok = m_tree->DeleteNode(target_node);
		return ok ? ReturnType::Success : ReturnType::UnExpectedException;
	}
	//�����Ŀ¼
	DirNode* cur_dir = dynamic_cast<DirNode*>(target_node);
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
//�����£���SymlinkNode�Ĵ����߼�
//1�����Ŀ��·����Ŀ¼�����Ƶ�Ŀ¼��
//2�����Ŀ��·�����ļ������Ŀ���ļ����и���д��
ReturnType NodeTreeManager::CopyFromDiskToMemory(const std::vector<string_local>& file_path_vec, const std::vector<string_local>& dst_path_tokens, const OptionSwitch& option_switch)
{
	//�ҵ���ʵ��Ŀ��ڵ㣨directory or file��
	auto temp_working_stack = GetTemporaryWorkingStackByTokens(dst_path_tokens);
	BaseNode* target_node = FindTrueNodeByTokensInternal(dst_path_tokens, temp_working_stack);
	if (target_node == nullptr)
	{
		return ReturnType::MemoryPathIsNotFound;
	}
	//Ŀ��·����Ŀ¼
	if (target_node->IsDirectory())
	{
		DirNode* target_dir = dynamic_cast<DirNode*>(target_node);
		CopyFromDiskToMemoryToDirectory(file_path_vec, target_dir, option_switch);
	}
	//Ŀ��·�����ļ�
	else if (target_node->IsFile())
	{
		FileNode* target_file = dynamic_cast<FileNode*>(target_node);
		CopyFromDiskToMemoryFile(file_path_vec, target_file, option_switch);
	}
	else 
	{
		return ReturnType::UnExpectedException;
	}
	return ReturnType::Success;
}


void NodeTreeManager::CopyFromDiskToMemoryToDirectory(const std::vector<string_local>& file_path_vec, DirNode* target_dir, const OptionSwitch& option_switch)
{
	bool is_silent_overwrite_all = option_switch._y; //�Ƿ�Ĭ�ϸ���ȫ�������ļ�
	int copy_count = 0;
	for (const auto& file_path : file_path_vec)
	{
		assert(PathTools::IsDiskPathExist(file_path));
		//��ȡ�����ļ�����
		size_t file_size = 0;
		char* buffer = ReadDiskFileData(StringTools::WStringToString(file_path).c_str(), file_size);//�����ڴ�
		if (buffer == nullptr)
		{
			Console::Write::PrintLine(TEXT("��ȡ�����ļ� ") + file_path + TEXT(" ʧ��"));
			continue;
		}
		string_local file_name = PathTools::GetFileName(file_path);
		//�����µ��ļ�
		if (!target_dir->ContainsChild(file_name))
		{
			m_tree->InsertNode(target_dir, new FileNode(file_name));
		}
		//��Ĭ���������ļ�
		else if (is_silent_overwrite_all)
		{
		}
		//��ʾ�û��Ƿ񸲸������ļ�
		else
		{
			string_local dir_path = GetTruePathOfNode(target_dir);
			SelectType select = Selector(TEXT("���� ") + dir_path + TEXT("/") + file_name + TEXT(" ��? (Yes/No/All):"));
			if (select == SelectType::all)
			{
				is_silent_overwrite_all = true;
			}
			//������ǰ�ļ�
			else if (select == SelectType::no)
			{
				continue;
			}
		}
		BaseNode* node = target_dir->FindChildByName(file_name);
		OverwriteFileNode(node, buffer,file_size);
		free(buffer); //�ͷ��ڴ�
		copy_count++;
		Console::Write::PrintLine(TEXT("�����ļ� ") + file_path + TEXT(" �ɹ�"));
	}
	Console::Write::PrintLine(TEXT("�Ѹ��� ") + StringTools::FormatFromNumber(copy_count) + TEXT(" ���ļ�"));
}


void NodeTreeManager::CopyFromDiskToMemoryFile(const std::vector<string_local>& file_path_vec, FileNode* dst_node, const OptionSwitch& option_switch)
{
	//update ...
}

//���ҽڵ�����Ŀ¼
DirNode* NodeTreeManager::FindWhichDirectoryIsNodeUnderByTokens(const std::vector<string_local>& tokens, std::deque<BaseNode*>& working_stack)
{
	DirNode* src_dir = nullptr;
	{
		auto src_dir_tokens = tokens;
		src_dir_tokens.pop_back();
		if (src_dir_tokens.empty())
		{
			src_dir_tokens.emplace_back(Constant::gs_cur_dir_token);
		}
		auto node = FindTrueNodeByTokensInternal(src_dir_tokens, working_stack);
		if (node != nullptr && node->IsDirectory())
		{
			src_dir = dynamic_cast<DirNode*>(node);
		}
	}
	return src_dir;
}


//<update> ...
//�����£���δ����׷��д��Ŀ���ļ�������߼�
ReturnType NodeTreeManager::CopyFromMemoryToMemory(const std::vector<string_local>& src_tokens, const std::vector<string_local>& dst_tokens, const OptionSwitch& option_switch)
{
	const auto& src_file_match_pattern = src_tokens.back();//��ƥ���ļ���
	//����Դ�ڵ�����Ŀ¼
	auto temp_working_stack = GetTemporaryWorkingStackByTokens(src_tokens);
	DirNode* src_dir = FindWhichDirectoryIsNodeUnderByTokens(src_tokens, temp_working_stack);
	if (src_dir == nullptr)
	{
		return ReturnType::MemoryPathIsNotFound;//error : ·��������
	}
	//��ȡԴ�ļ��б�
	std::vector<BaseNode*> src_nodes = {};
	auto src_children = src_dir->Children();
	for (const auto& child : src_children)
	{
		if (!child->IsFile() || !child->IsFileLink())
		{
			continue;
		}
		if (StringTools::IsStringFuzzyEqualTo(child->GetName(), src_file_match_pattern))
		{
			src_nodes.emplace_back(child);
		}
	}
	if (src_nodes.empty())
	{
		return ReturnType::MemoryPathIsNotFound;//error : Դ·��������
	}
	//��ȡĿ��ڵ�
	temp_working_stack = GetTemporaryWorkingStackByTokens(dst_tokens);
	BaseNode* dst_node = FindTrueNodeByTokensInternal(dst_tokens, temp_working_stack);
	if (nullptr == dst_node || dst_node->IsFileLink() || dst_node->IsDirectoryLink())
	{
		return ReturnType::MemoryPathIsNotFound;//error : Դ·��������
	}
	//������Ŀ¼
	if (dst_node->IsDirectory())
	{
		CopyFromMemoryToMemoryDirectory(src_nodes, dynamic_cast<DirNode*>(dst_node), option_switch);
	}
	//�������ļ�
	else if(dst_node->IsFile())
	{
		//update �����
		return ReturnType::UnExpectedException;
	}
	else
	{
		return ReturnType::UnExpectedException;
	}
	return ReturnType::Success;
}


void NodeTreeManager::CopyFromMemoryToMemoryDirectory(const std::vector<BaseNode*>& node_list, DirNode* target_dir, const OptionSwitch& option_switch)
{
	assert(nullptr != target_dir && !node_list.empty());
	bool is_silent_overwrite_all = option_switch._y; //�Ƿ�Ĭ�ϸ���ȫ�������ļ�
	int copy_count = 0;
	for (const auto& file_node : node_list)
	{
		auto file_name = file_node->GetName();
		//�����µ��ļ�
		if (!target_dir->ContainsChild(file_name))
		{
			m_tree->InsertNode(target_dir, new FileNode(file_name));
		}
		//��ʾ�û��Ƿ񸲸�Ŀ���ļ�
		else if (!is_silent_overwrite_all)
		{
			string_local dir_path = GetTruePathOfNode(target_dir);
			SelectType select = Selector(TEXT("���� ") + dir_path + TEXT("/") + file_name + TEXT(" ��? (Yes/No/All):"));
			if (select == SelectType::all)
			{
				is_silent_overwrite_all = true;
			}
			//������ǰ�ļ�
			else if (select == SelectType::no)
			{
				continue;
			}
		}
		BaseNode* node = target_dir->FindChildByName(file_name);
		char* data = nullptr;
		if (file_node->IsFile())
		{
			data = dynamic_cast<FileNode*>(node)->GetData();
		}
		else
		{
			auto link_node = GetTrueFileNode(node);
			if (nullptr == link_node)
			{
				Console::Write::PrintLine(TEXT("�ļ�����") + GetTruePathOfNode(file_node) + TEXT("��ʧЧ"));
				continue;
			}
			data = link_node->GetData();
		}
		assert(data != nullptr);
		OverwriteFileNode(node, data, file_node->GetSize());
		copy_count++;
		Console::Write::PrintLine(TEXT("�����ļ� ") + GetTruePathOfNode(file_node) + TEXT(" �ɹ�"));
	}
}



SelectType NodeTreeManager::Selector(const string_local& str)
{
	string_local input;
	while (true)
	{
		Console::Write::Print(str);
		Console::Read::ReadLine(input);
		if (StringTools::IsEqual(input, TEXT("yes")))
		{
			return SelectType::yes;
		}
		if (StringTools::IsEqual(input, TEXT("no")))
		{
			return SelectType::no;
		}
		if (StringTools::IsEqual(input, TEXT("all")))
		{
			return SelectType::all;
		}
	}
}


void NodeTreeManager::OverwriteFileNode(BaseNode* node, const char* content,const size_t& size)
{
	assert(node != nullptr);
	assert(node->IsFile());
	FileNode* file_node = dynamic_cast<FileNode*>(node);
	file_node->SetData(content, size);
}


//1���߼��򻯣�ֻ����ʹ�þ���·������������
//2��(�����)�߼��򻯣����Ϊ���Ӵ������ӣ������������ϲ�����·
//3���߼��򻯣������������������Ӷ���֮�����Ͳ�ƥ��
ReturnType NodeTreeManager::MklinkByPathAndOptions(const Path& link_path, const Path& src_path, const OptionSwitch& option_switch)
{
	bool is_directory_link = option_switch._d;
	//���Դ·����Ŀ��·���Ƿ�Ϸ�
	if (!src_path.IsValid() || !link_path.IsValid())
	{
		return ReturnType::MemoryPathNameIsIllegal;
	}
	const auto& src_tokens = src_path.Tokens();
	const auto& dst_tokens = link_path.Tokens();
	//���Դ·����Ŀ��·���Ƿ�Ϊ��
	if (src_tokens.empty() || dst_tokens.empty())
	{
		return ReturnType::MemoryPathIsNotFound;
	}
	//���Դ·���Ƿ�Ϊ����·��
	if (!IsAbsolutePath(src_tokens))
	{
		Console::Write::PrintLine(TEXT("Դ·�����Ǿ���·��"));
		return ReturnType::UnExpectedException;
	}
	//��ȡԴ·������Ŀ¼
	const auto& src_node_token = src_tokens.back();
	auto temp_working_stack = GetTemporaryWorkingStackByTokens(src_tokens);
	DirNode* src_parent_dir = FindWhichDirectoryIsNodeUnderByTokens(src_tokens,temp_working_stack);
	if (src_parent_dir == nullptr)
	{
		return ReturnType::MemoryPathIsNotFound;
	}
	BaseNode* src_node = nullptr;
	if (IsCurrentDir(src_node_token))
	{
		src_node = src_parent_dir;
	}
	else if (IsParentDir(src_node_token))
	{
		temp_working_stack.pop_back();
		if (temp_working_stack.empty())
		{
			return ReturnType::MemoryPathIsNotFound;
		}
		src_node = temp_working_stack.back();
	}
	else
	{
		src_node = src_parent_dir->FindChildByName(src_node_token);
	}
	if (nullptr == src_node || IsSameNode(src_node,m_tree->GetRoot()))
	{
		return ReturnType::MemoryPathIsNotFound;
	}
	//���������Ӷ������Ͳ�ƥ��
	if (is_directory_link && !(src_node->IsDirectory() || src_node->IsDirectoryLink()))
	{
		return ReturnType::TypeOfLinkAndSourceIsNotMatch;
	}
	if (!is_directory_link && !(src_node->IsFile() || src_node->IsFileLink()))
	{
		return ReturnType::TypeOfLinkAndSourceIsNotMatch;
	}
	//�����Ѵ��ڵĺϷ��ڵ㣬����������
	//�����������Ŀ¼
	auto dst_node_token = dst_tokens.back();
	auto dst_working_stack = GetTemporaryWorkingStackByTokens(dst_tokens);
	DirNode* dst_parent_dir = FindWhichDirectoryIsNodeUnderByTokens(dst_tokens, dst_working_stack);
	if (nullptr == dst_parent_dir)
	{
		return ReturnType::MemoryPathIsNotFound;
	}
	//�ж��Ƿ��������ļ�
	auto symlink_node = dst_parent_dir->FindChildByName(dst_node_token);
	if (nullptr != symlink_node)
	{
		return ReturnType::MemoryFileIsExist;//���������ļ�
	}
	//���������ӽڵ㣬����������
	symlink_node = new SymlinkNode(dst_node_token);
	NodeType link_type = (is_directory_link) ? NodeType::SymlinkD : NodeType::SymlinkF;
	dynamic_cast<SymlinkNode*>(symlink_node)->SetTarget(link_type, src_path.ToString());
	m_tree->InsertNode(dst_parent_dir, symlink_node);
	return ReturnType::Success;
}


//����ɣ�ʹ��ͨ���ƥ����Դ·��
//1����dstĿ¼���������ļ�/Ŀ¼�����Ա�����
//2����ʵ����·���ϵĽڵ㲻����move
//3��Դ·��������ڣ�Ŀ��·�����Բ����ڣ���������Ŀ¼�������,��ʱ��Ҫ��ԴĿ¼/�ļ�������
//4��src�����������ļ���dst��������ʵĿ¼
ReturnType NodeTreeManager::MoveByTokensAndOptions(const Path& src_path, const Path&  dst_path, const OptionSwitch& option_switch)
{
	bool is_silent_overwrite = option_switch._y; //�Ƿ�Ĭ����
	bool is_need_rename = false;//�Ƿ���Ҫ������
	auto src_tokens = src_path.Tokens();
	auto dst_tokens = dst_path.Tokens();
	auto src_working_stack = GetTemporaryWorkingStackByTokens(src_tokens);
	auto dst_working_stack = GetTemporaryWorkingStackByTokens(dst_tokens);
	DirNode* src_parent_dir = FindWhichDirectoryIsNodeUnderByTokens(src_tokens, src_working_stack);
	DirNode* dst_parent_dir = FindWhichDirectoryIsNodeUnderByTokens(dst_tokens, dst_working_stack);
	if (nullptr == src_parent_dir || nullptr == dst_parent_dir)
	{
		return ReturnType::MemoryPathIsNotFound;
	}
	//��ȡԴ�ڵ�
	const auto src_node_token = src_tokens.back();
	BaseNode* src_node = nullptr;
	if (IsCurrentDir(src_node_token))
	{
		src_node = src_parent_dir;
	}
	else if (IsParentDir(src_node_token))
	{
		src_working_stack.pop_back();
		if (src_working_stack.empty())
		{
			return ReturnType::MemoryPathIsNotFound;
		}
		src_node = src_working_stack.back();//ע�⣬���src_node��һ�����ӽڵ㣬��Ҫ��ȡ�䱾�������������Ӷ���
	}
	else
	{
		src_node = src_parent_dir->FindChildByName(src_node_token);
	}
	//�ж�Դ�ڵ��Ƿ����move
	//�����ƶ��սڵ���Ŀ¼
	if (nullptr == src_node || IsSameNode(src_node, m_tree->GetRoot()))
	{
		return ReturnType::MemoryPathIsNotFound;
	}
	//�����ƶ�����·���ϵ�Ŀ¼
	if (IsTrueAncestor(src_node, GetMyWorkingStack().back()))
	{
		return ReturnType::AccessDenied;//error : �ܾ�����
	}
	const auto& dst_node_token = dst_tokens.back();
	BaseNode* dst_node = nullptr;
	if (IsCurrentDir(dst_node_token))
	{
		dst_node = dst_parent_dir;
	}
	else if (IsParentDir(dst_node_token))
	{
		dst_working_stack.pop_back();
		if (dst_working_stack.empty())
		{
			return ReturnType::MemoryPathIsNotFound;
		}
		dst_node = dst_working_stack.back();
	}
	else
	{
		dst_node = dst_parent_dir->FindChildByName(dst_node_token);
	}
	//Ѱ��Ŀ��Ŀ¼
	DirNode* target_dir = nullptr;
	{
		//Ŀ��ڵ㲻���ڣ���Ҫ��Դ�ڵ�������
		if (dst_node == nullptr)
		{
			target_dir = dst_parent_dir;
			is_need_rename = true;
		}
		//Ŀ��ڵ����ļ��ڵ㣬��ʾ�û��Ƿ񸲸�
		else if(dst_node->IsFile() || dst_node->IsFileLink())
		{
			target_dir = dst_parent_dir;
		}
		//Ŀ��ڵ���Ŀ¼���ӣ���ȡ�����ӵ���ʵĿ¼��������
		else if (dst_node->IsDirectoryLink())
		{
			target_dir = GetTrueDirNode(dst_node);
		}
		//Ŀ��ڵ���Ŀ¼������
		else if (dst_node->IsDirectory())
		{
			target_dir = dynamic_cast<DirNode*>(dst_node);
		}
	}
	if (target_dir == nullptr)
	{
		return ReturnType::MemoryPathIsNotFound;
	}
	//�ж�Ŀ��Ŀ¼���Ƿ��������ڵ㣬����У�����ʾ�û��Ƿ񸲸�
	{
		auto node = target_dir->FindChildByName(dst_node_token);
		if (nullptr != node)
		{
			if (!is_silent_overwrite)
			{
				auto sel_ret = Selector(TEXT("����") + dst_path.ToString() + TEXT("�𣿣�Yes/No/All��"));
				if (sel_ret == SelectType::no)
				{
					return ReturnType::Success;
				}
			}
			//ɾ�������ڵ�
			m_tree->DeleteNode(dst_node);
		}
	}
	//�����ƶ�����
	m_tree->RemoveButNotDeleteNode(src_node);
	m_tree->InsertNode(target_dir, src_node);
	//������
	if (is_need_rename)
	{
		if (PathTools::IsTokensFormatLegal({ dst_node_token }))
		{
			src_node->SetName(dst_node_token);
		}
		else
		{
			return ReturnType::MemoryPathNameIsIllegal;
		}
	}
	//��ӡ��ʾ�İ�
	if (IsDirOrDirLink(src_node))
	{
		Console::Write::PrintLine(TEXT("�ƶ���1��Ŀ¼ ") + GetTruePathOfNode(src_node));
	}
	else
	{
		Console::Write::PrintLine(TEXT("�ƶ���1���ļ� ") + GetTruePathOfNode(src_node));
	}
	return ReturnType::Success;
}


//1������·����Ŀ¼����Ĭ��ɾ��Ŀ¼�������ļ�
//2�����ڲ�Ӱ��Ŀ¼�����Կ��Ի��ݵ�����·���Ͻ����ļ�ɾ������
ReturnType NodeTreeManager::DelByTokensAndOption(const Path& path, const OptionSwitch& option_switch)
{
	bool is_recursive = option_switch._s;
	auto tokens = path.Tokens();
	if (tokens.empty())
	{
		return ReturnType::MemoryPathIsNotFound;
	}
	const auto& file_name_pattern = tokens.back();
	auto temp_working_stack = GetTemporaryWorkingStackByTokens(tokens);
	DirNode* parent_dir = FindWhichDirectoryIsNodeUnderByTokens(tokens,temp_working_stack);
	if (nullptr == parent_dir)
	{
		return ReturnType::MemoryPathIsNotFound;
	}	
	//��ȡĿ��ڵ�
	BaseNode* target_node = nullptr;
	if (IsCurrentDir(file_name_pattern))
	{
		target_node = parent_dir;
	}
	else if (IsParentDir(file_name_pattern))
	{
		temp_working_stack.pop_back();
		if (m_working_stack.empty())
		{
			return ReturnType::MemoryPathIsNotFound;
		}
		target_node = temp_working_stack.back();
	}
	else
	{
		target_node = parent_dir->FindChildByName(file_name_pattern);
	}
	//Ŀ��ڵ㲻���Ǹ��ڵ�
	if (nullptr != target_node && IsSameNode(target_node, m_tree->GetRoot()))
	{
		return ReturnType::MemoryPathIsNotFound;
	}

	if (nullptr != target_node)
	{
		//Ŀ��ڵ���Ŀ¼��Ŀ¼����
		if (IsDirOrDirLink(target_node))
		{
			DirNode* dir = GetTrueDirNode(target_node);
			if (nullptr == dir)
			{
				return ReturnType::MemoryPathIsNotFound;
			}
			bool ret = DeleteNodeByWildcardFileName(dir, TEXT("*"), is_recursive);
			return ReturnType::Success;
		}
		//Ŀ��ڵ����ļ����ļ�����
		else
		{
			DeleteNodeByWildcardFileName(parent_dir, file_name_pattern, is_recursive);
			return ReturnType::Success;
		}
	}
	//ʹ��ͨ���ƥ��Ŀ¼�µ��ļ���ɾ��
	else if (path.IsWild())
	{
		//����ͨ���·��ƥ���ļ���ɾ��
		bool ret = DeleteNodeByWildcardFileName(parent_dir, file_name_pattern, is_recursive);
		return ReturnType::Success;
	}
	return ReturnType::MemoryPathIsNotFound;//error : �������·��������
}


bool NodeTreeManager::DeleteNodeByWildcardFileName(DirNode* cur_dir, const string_local& file_name, bool is_recursive)
{
	assert(nullptr != cur_dir);
	auto target_dir = dynamic_cast<DirNode*>(cur_dir);
	std::queue<DirNode*> q;
	q.push(target_dir);
	while (!q.empty())
	{
		DirNode* dir = q.front();
		auto children = dir->Children();
		q.pop();
		//���ɾ��ȫ���ļ�������ʾ�û�ȷ��
		if (StringTools::IsEqual(file_name, TEXT("*")))
		{
			SelectType ret = Selector(TEXT("ɾ�� ") + GetTruePathOfNode(dir) + TEXT("\\*,�Ƿ�ȷ��(Yes/No/All)��"));
			if (ret == SelectType::no)
			{
				continue;
			}
		}
		//ɾ����ǰĿ¼������ƥ��file_name���ļ�
		std::vector<BaseNode*> del_vec{};
		for (const auto& item : children)
		{
			if (!item->IsDirectory() && StringTools::IsFuzzyMatch(item->GetName().c_str(), file_name.c_str()))
			{
				del_vec.emplace_back(item);
			}
		}
		for (const auto& item : del_vec)
		{
			Console::Write::PrintLine(TEXT("ɾ���ļ� - ") + GetTruePathOfNode(item));
			m_tree->DeleteNode(item);
		}
		del_vec.clear();
		//���û�еݹ�ѡ�����ѭ��
		if (!is_recursive)
		{
			break;
		}
		//��������Ŀ¼�ڵ��������
		for (const auto& item : children)
		{
			if (item->IsDirectory())
			{
				q.push(dynamic_cast<DirNode*>(item));
			}
		}
	}
	while (!q.empty())
	{
		q.pop();
	}
	return true;
}


bool NodeTreeManager::IsSameNode(BaseNode* lhs, BaseNode* rhs)
{
	assert(nullptr != lhs && nullptr != rhs);
	bool ret = (lhs->GetId() == rhs->GetId()) ? true : false;
	return ret;
	/*auto lstr = GetPathByNode(lhs);
	auto rstr = GetPathByNode(rhs);
	return StringTools::IsEqual(lstr, rstr);*/
}


ReturnType NodeTreeManager::SaveToPath(const string_local& path_str)
{
	assert(!PathTools::IsDiskPathExist(path_str));
	tinyxml2::XMLDocument doc;	
	auto node_root = m_tree->GetRoot();
	auto xml_root = WriteDirToXml(dynamic_cast<DirNode*>(node_root), doc);
	xml_root->SetAttribute(XMLIdentifier::name, StringTools::UnicodeToUtf8(node_root->GetName().c_str()));
	xml_root->SetAttribute(XMLIdentifier::id, node_root->GetId());
	xml_root->SetAttribute(XMLIdentifier::type, static_cast<int>(node_root->GetType()));
	xml_root->SetAttribute(XMLIdentifier::timestamp, node_root->GetLatestModifiedTimeStamp());
	xml_root->SetAttribute(XMLIdentifier::parentId, 0);
	doc.LinkEndChild(xml_root);
	auto save_ret = doc.SaveFile(StringTools::UnicodeToUtf8(path_str.c_str()));
	if (save_ret != tinyxml2::XMLError::XML_SUCCESS)
	{
		Console::Write::PrintLine(TEXT("XML�ļ�����ʧ��"));
		return ReturnType::UnExpectedException;
	}
	return ReturnType::Success;
}

//<udpate>
//loadҪ���ݴ������xml���ݸ�ʽ�����⣬����Ҫ������Ӧ�Ĵ�����Ϣ
ReturnType NodeTreeManager::LoadFromPath(const string_local& path_str)
{
	assert(PathTools::IsDiskPathExist(path_str));
	tinyxml2::XMLDocument xml_doc;
	if (xml_doc.LoadFile(StringTools::UnicodeToUtf8(path_str.c_str())))
	{
		return ReturnType::LoadXmlFileFailed;
	}
	auto xml_root = xml_doc.RootElement();
	//��ȡXML�ļ�
	BaseNode* new_root = ReadXml(xml_root, nullptr);
	if (nullptr == new_root)
	{
		return ReturnType::LoadXmlFileFailed;
	}
	//�������������
	auto new_tree = new NodeTree();
	new_tree->Create(new_root);
	//���ظ��������
	bool ok = this->Load(new_tree);
	if (!ok)
	{
		return ReturnType::LoadXmlFileFailed;
	}
	return ReturnType::Success;
}


tinyxml2::XMLElement* NodeTreeManager::WriteDirToXml(DirNode* dir,tinyxml2::XMLDocument& doc)
{
	assert(dir);
	tinyxml2::XMLElement* xml_dir = doc.NewElement(XMLIdentifier::DirNode);
	//tinyxml2::XMLElement* ele_children = doc->NewElement(XMLIdentifier::children);
	std::vector<BaseNode*> children = dir->Children();
	//ele_children->SetAttribute(XMLIdentifier::count, children.size());
	for (const auto& node : children)
	{
		tinyxml2::XMLElement* ele_node = nullptr;
		//����Ŀ¼����
		if (node->IsDirectory())
		{
			auto dir_node = dynamic_cast<DirNode*>(node);
			ele_node = WriteDirToXml(dir_node, doc);
		}
		//�����ļ�����
		else if (node->IsFile())
		{
			auto file_node = dynamic_cast<FileNode*>(node);
			ele_node = doc.NewElement(XMLIdentifier::FileNode);
			ele_node->SetAttribute(XMLIdentifier::data, file_node->GetData());
			ele_node->SetAttribute(XMLIdentifier::dataSize, file_node->GetSize());
		}
		//�������Ӷ���
		else
		{
			auto link_node = dynamic_cast<SymlinkNode*>(node);
			ele_node = doc.NewElement(XMLIdentifier::SymlinkNode);
			ele_node->SetAttribute(XMLIdentifier::symlinkPath, StringTools::UnicodeToUtf8(link_node->GetTargetPath().c_str()));
		}
		//�����ӽڵ�Ļ�������
		ele_node->SetAttribute(XMLIdentifier::name, StringTools::UnicodeToUtf8(node->GetName().c_str()));
		ele_node->SetAttribute(XMLIdentifier::id, node->GetId());
		ele_node->SetAttribute(XMLIdentifier::type, static_cast<int>(node->GetType()));
		ele_node->SetAttribute(XMLIdentifier::timestamp, node->GetLatestModifiedTimeStamp());
		ele_node->SetAttribute(XMLIdentifier::parentId, dir->GetId());
		//׷���ӽڵ�
		//ele_children->LinkEndChild(ele_node);
		xml_dir->LinkEndChild(ele_node);
	}
	//xml_dir->LinkEndChild(ele_children);
	return xml_dir;
}


/// \brief ��XML�������ڵ���
BaseNode* NodeTreeManager::ReadXml(tinyxml2::XMLElement* xml_item, DirNode* parent)
{
	assert(xml_item);
	string_local node_name = StringTools::Utf8ToUnicode(xml_item->FindAttribute(XMLIdentifier::name)->Value());
	int64_t node_id = xml_item->FindAttribute(XMLIdentifier::id)->Int64Value();
	int32_t type_int = xml_item->FindAttribute(XMLIdentifier::type)->IntValue();
	int64_t node_timestamp = xml_item->FindAttribute(XMLIdentifier::timestamp)->Int64Value();
	if (!IsNodeAttributeIsValid(node_name, node_id, type_int, node_timestamp))
	{
		return nullptr;//error : xml�����쳣
	}
	NodeType node_type = static_cast<NodeType>(type_int);
	BaseNode* node = nullptr;
	switch (node_type)
	{
	case NodeType::Directory:
		node = new DirNode(node_name);
		break;
	case NodeType::File:
		node = new FileNode(node_name);
		dynamic_cast<FileNode*>(node)->SetData(
			xml_item->FindAttribute(XMLIdentifier::data)->Value(),
			xml_item->FindAttribute(XMLIdentifier::dataSize)->Int64Value());
		break;
	case NodeType::SymlinkD:
	case NodeType::SymlinkF:
		node = new SymlinkNode(node_name);
		dynamic_cast<SymlinkNode*>(node)->SetTarget(
			node_type, 
			StringTools::Utf8ToUnicode(xml_item->FindAttribute(XMLIdentifier::symlinkPath)->Value()));
		break;
	default:
		break;
	}
	node->SetId(node_id);
	node->SetLatestModifiedTimeStamp(node_timestamp);
	//���뵱ǰ�ڵ�
	if (parent && node && !parent->ContainsChild(node->GetName())) 
	{
		parent->AppendChild(node);
		node->SetParent(parent);
	}
	if (!node->IsDirectory())
	{
		return node;
	}
	//�������ӽڵ�
	for (auto iter = xml_item->FirstChild(); iter != 0; iter = iter->NextSibling())
	{
		auto ele = iter->ToElement();
		auto child = ReadXml(ele, dynamic_cast<DirNode*>(node));
	}
	return node;
}


bool NodeTreeManager::InsertSymlinkNodeByXml(std::vector<tinyxml2::XMLElement*>& link_elems)
{
	//�������ӽڵ�
	for (const auto& item : link_elems)
	{
		//��XML�����н����ڵ�����
		auto attr_name = item->FindAttribute(XMLIdentifier::name)->Value();
		auto attr_id = item->FindAttribute(XMLIdentifier::id)->Int64Value();
		auto attr_type = item->FindAttribute(XMLIdentifier::type)->IntValue();
		auto attr_timestamp = item->FindAttribute(XMLIdentifier::timestamp)->Int64Value();
		auto attr_parentId = item->FindAttribute(XMLIdentifier::parentId)->Int64Value();
		auto attr_symlinkPath = item->FindAttribute(XMLIdentifier::symlinkPath)->Value();
		auto parent_node = m_tree->FindNodeById(attr_parentId);
		assert(parent_node);
		//��ø��ڵ�
		auto parent_dir = dynamic_cast<DirNode*>(parent_node);
		//�������ӽڵ�
		SymlinkNode* node = new SymlinkNode(StringTools::Utf8ToUnicode(attr_name), parent_dir);
		//��������·��
		node->SetTarget(static_cast<NodeType>(attr_type), StringTools::Utf8ToUnicode(attr_symlinkPath));
		//����ID
		node->SetId(attr_id);
		//�����޸�ʱ��
		node->SetLatestModifiedTimeStamp(attr_timestamp);
		//�������ӽڵ�
		m_tree->InsertNode(parent_node, node);
	}
	return true;
}

//���ܴ��ڵ����⣺û��У��FileNode->data && SymlinkNode->path
bool NodeTreeManager::IsNodeAttributeIsValid(const string_local& name, const int64_t& id, const int32_t& type, const int64_t& timestamp)
{
	//token���ܰ����Ƿ��ַ�
	if (!PathTools::IsTokensFormatLegal({ name }))
	{
		return false;
	}
	//id��timestamp�����Ǹ���
	if (id < static_cast<int64_t>(0) || timestamp < static_cast<int64_t>(0))
	{
		return false;
	}
	//type��������nodetype��Χ��
	if (!static_cast<int32_t>(NodeType::Directory) == type || !static_cast<int32_t>(NodeType::File) == type || 
		!static_cast<int32_t>(NodeType::SymlinkF) == type || !static_cast<int32_t>(NodeType::SymlinkD) == type)
	{
		return false;
	}
	return true;
}

