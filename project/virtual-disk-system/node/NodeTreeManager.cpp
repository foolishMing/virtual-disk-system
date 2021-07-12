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
	m_tree->Create();
	InitDrivens();
	Log::LogInfo(TEXT("node tree manager is created."));
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
	Log::LogInfo(TEXT("node tree manager is destroyed."));
}




string_local NodeTreeManager::GetCurrentPath() const
{
	assert(nullptr != m_tree);
	BaseNode* node = m_working_dir;
	auto ret = GetPathByNode(node);
	return ret;
}

void NodeTreeManager::PrintFileNodeInfo(BaseNode* cur_node)
{
	assert(cur_node);
	//��ӡ���ļ��ڵ���Ϣ
	if (cur_node->IsFile())
	{
		auto file_node = static_cast<FileNode*>(cur_node);
		std::wcout << StringTools::TimeStampToDateTimeString(file_node->GetLatestModifiedTimeStamp())
			<< TEXT("    ")
			<< std::right << std::setw(14) << std::to_wstring(file_node->GetSize())
			<< TEXT(" ") << file_node->GetName() << std::endl;
	}
	//��ӡ���ӽڵ���Ϣ
	else
	{
		auto link_node = static_cast<SymlinkNode*>(cur_node);
		std::wcout << StringTools::TimeStampToDateTimeString(link_node->GetLatestModifiedTimeStamp())
			<< TEXT("    ")
			<< std::left << std::setw(15) << TEXT("<SYMLINK>")
			<< link_node->GetName() << std::endl;
	}
}

//<udpate ...>
//δ��ɣ�������ݸ�ʽ��
//δ��ɣ�������ֵ�����޸�ΪDirInfo
//is_ad == true,ֻ��ӡ��Ŀ¼�ڵ�
void NodeTreeManager::PrintDirectoryInfo(BaseNode* dir, StatisticInfo& g_info, bool is_ad)//��ӡĿ¼��Ϣ
{
	assert(dir && dir->IsDirectory());
	StatisticInfo info;
	DirNode* cur_dir = static_cast<DirNode*>(dir);
	Console::Write::PrintLine(TEXT(""));
	Console::Write::PrintLine(GetPathByNode(cur_dir) + TEXT(" ��Ŀ¼"));
	Console::Write::PrintLine(TEXT(""));
	
	auto children = cur_dir->Children();
	info.tot_cnt = children.size();
	//�����ӽڵ㣬ͳ�Ʋ���ӡ�ڵ���Ϣ{�޸�ʱ�䡢�ڵ����͡��ڵ��С���ڵ�����}
	for (BaseNode* cur_node : children)
	{
		info.tot_size += cur_node->GetSize();
		//��ӡĿ¼�ڵ���Ϣ
		if (cur_node->IsDirectory())
		{
			info.dir_cnt++;
			auto dir = static_cast<DirNode*>(cur_node);
			std::wcout << StringTools::TimeStampToDateTimeString(dir->GetLatestModifiedTimeStamp()) 
				<< TEXT("    ") 
				<< std::left << std::setw(15) << TEXT("<DIR>") 
				<< dir->GetName() << std::endl;
			continue;
		}
		if (is_ad) continue;
		//��ӡ�ļ��ڵ���Ϣ
		PrintFileNodeInfo(cur_node);
	}
	PrintStatisticInfo(info);
	g_info += info;
}

void NodeTreeManager::PrintStatisticInfo(StatisticInfo& info)
{
	//{�ļ����������ֽ�}
	std::wcout << std::right << std::setw(20) << std::to_wstring(info.tot_cnt - info.dir_cnt) << TEXT(" ���ļ�")
		<< std::right << std::setw(20) << std::to_wstring(info.tot_size) << TEXT("  �ֽ�") << std::endl;
	//{Ŀ¼�����������ֽ�}
	std::wcout << std::right << std::setw(20) << std::to_wstring(info.dir_cnt) << TEXT(" ��Ŀ¼")
		<< std::right << std::setw(20) << std::to_wstring(getTotalSystemMemory()) << TEXT("  �����ֽ�") << std::endl;
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
	return (target_node != nullptr && target_node != m_tree->GetRoot()) ? true : false;
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
	bool is_recursive = option_switch._s;
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
	StatisticInfo g_info;
	while (!q.empty())
	{
		DirNode* node = q.front();
		q.pop();
		//��ӡ��ǰĿ¼�µ���Ŀ¼���ļ���Ϣ
		PrintDirectoryInfo(node, g_info, option_switch._ad);
		//����Ŀ¼���ڴ���ӡĿ¼�ڵ������
		if (is_recursive)
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
	if (is_recursive)
	{
		Console::Write::PrintLine(TEXT("\n     �����ļ�����:"));
		g_info.tot_size = cur_dir_node->GetSize();
		PrintStatisticInfo(g_info);
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


//1�����Ŀ��·����Ŀ¼�����Ƶ�Ŀ¼��
//2�����Ŀ��·�����ļ������Ŀ���ļ����и���д��
ReturnType NodeTreeManager::CopyFromDiskToMemory(const std::vector<string_local>& file_path_vec, const std::vector<string_local>& dst_path_tokens, const OptionSwitch& option_switch)
{
	BaseNode* target_node = FindNodeByTokensInternal(dst_path_tokens);
	assert(target_node != nullptr);
	//Ŀ��·����Ŀ¼
	if (target_node->IsDirectory())
	{
		DirNode* target_dir = static_cast<DirNode*>(target_node);
		CopyFromDiskToMemoryToDirectory(file_path_vec, target_dir, option_switch);
	}
	//Ŀ��·�����ļ�
	else if (target_node->IsFile())
	{
		FileNode* target_file = static_cast<FileNode*>(target_node);
		CopyFromDiskToMemoryFile(file_path_vec, target_file, option_switch);
	}
	//Ŀ��·��������
	else
	{
		//update...
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
		char* buffer = ReadDiskFileData(StringTools::WStringToString(file_path).c_str(), file_size);
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
			string_local dir_path = GetPathByNode(target_dir);
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
		copy_count++;
		Console::Write::PrintLine(TEXT("�����ļ� ") + file_path + TEXT(" �ɹ�"));
	}
	Console::Write::PrintLine(TEXT("�Ѹ��� ") + StringTools::FormatFromNumber(copy_count) + TEXT(" ���ļ�"));
}


void NodeTreeManager::CopyFromDiskToMemoryFile(const std::vector<string_local>& file_path_vec, FileNode* dst_node, const OptionSwitch& option_switch)
{
	//update ...
}


ReturnType NodeTreeManager::CopyFromMemoryToMemory(const std::vector<string_local>& src_tokens, const std::vector<string_local>& dst_tokens, const OptionSwitch& option_switch)
{
	//����Դ�ڵ�����Ŀ¼
	BaseNode* cur_node = nullptr;
	{
		auto src_dir_tokens = src_tokens;
		src_dir_tokens.pop_back();
		if (src_dir_tokens.empty())
		{
			cur_node = m_working_dir;
		}
		else
		{
			cur_node = FindNodeByTokensInternal(src_dir_tokens);
		}
		if (cur_node == nullptr || !cur_node->IsDirectory())
		{
			return ReturnType::MemoryPathIsNotFound; //error : Դ·��������
		}
	}
	//��ȡԴ�ڵ�
	std::vector<FileNode*> src_nodes = {};
	const auto& src_file_match_pattern = src_tokens.back();//��ƥ���ļ���
	DirNode* cur_dir = static_cast<DirNode*>(cur_node);
	for (const auto& child : cur_dir->Children())
	{
		if (!child->IsFile())
		{
			continue;
		}
		if (StringTools::IsStringFuzzyEqualTo(child->GetName(), src_file_match_pattern))
		{
			auto item = static_cast<FileNode*>(child);
			src_nodes.emplace_back(item);			
		}
	}
	if (src_nodes.empty())
	{
		return ReturnType::MemoryPathIsNotFound;//error : Դ·��������
	}
	//��ȡĿ��ڵ�
	BaseNode* dst_node = FindNodeByTokensInternal(dst_tokens);
	assert(nullptr != dst_node);
	//���Ŀ��ڵ���Ŀ¼
	if (dst_node->IsDirectory())
	{
		CopyFromMemoryToMemoryDirectory(src_nodes, static_cast<DirNode*>(dst_node), option_switch);
	}
	else
	{
		//update 
	}
	return ReturnType::Success;
}

void NodeTreeManager::CopyFromMemoryToMemoryDirectory(const std::vector<FileNode*>& node_list, DirNode* target_dir, const OptionSwitch& option_switch)
{
	assert(nullptr != target_dir);
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
		//��Ĭ����
		else if (is_silent_overwrite_all)
		{

		}
		//��ʾ�û��Ƿ񸲸�Ŀ���ļ�
		else
		{
			string_local dir_path = GetPathByNode(target_dir);
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
		OverwriteFileNode(node, file_node->GetData(), file_node->GetSize());
		copy_count++;
		Console::Write::PrintLine(TEXT("�����ļ� ") + GetPathByNode(file_node) + TEXT(" �ɹ�"));
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
	FileNode* file_node = static_cast<FileNode*>(node);
	file_node->SetData(content, size);
}


ReturnType NodeTreeManager::MklinkByTokensAndOptions(const std::vector<string_local>& symbol_tokens, const std::vector<string_local>& src_tokens, const OptionSwitch& option_switch)
{
	const auto& symbol_name = symbol_tokens.back();
	if (!PathTools::IsTokensFormatLegal({ symbol_name }))
	{
		return ReturnType::MemoryPathNameIsIllegal; //���Ϸ���·������
	}
	//��ȡ��ݷ�ʽ������Ŀ¼
	BaseNode* target_node = nullptr;
	if (symbol_tokens.size() == 1)
	{
		target_node = m_working_dir;
	}
	else
	{
		std::vector<string_local> tar_dir_tokens = symbol_tokens;
		tar_dir_tokens.pop_back();
		target_node = FindNodeByTokensInternal(tar_dir_tokens);
	}
	if (!target_node || !target_node->IsDirectory())
	{
		return ReturnType::MemoryPathIsNotFound; //�Ҳ�����ݷ�ʽ����·��
	}
	DirNode* target_dir = static_cast<DirNode*>(target_node);
	//�ҵ�Դ�ڵ�
	auto src_node = FindNodeByTokensInternal(src_tokens);
	assert(nullptr != src_node);
	//������ݷ�ʽ
	auto symbo_node = new SymlinkNode(symbol_name);
	NodeType link_type = (option_switch._d) ? NodeType::SymlinkD : NodeType::SymlinkF;
	symbo_node->SetSymlinkNode(link_type, src_node);
	m_tree->InsertNode(target_dir, symbo_node);
	return ReturnType::Success;
}


//1����dstĿ¼���������ļ�/Ŀ¼���ļ����Ա����ǣ�Ŀ¼�����ԡ���error �� �ܾ�����
//2������·���ϵĽڵ㲻����move
//3��Դ·��������ڣ�Ŀ��·�����Բ����ڣ���������Դ·����ͬһĿ¼�£���ʱ�൱��������
ReturnType NodeTreeManager::MoveByTokensAndOptions(const std::vector<string_local>& src_tokens, const std::vector<string_local>& dst_tokens, const OptionSwitch& option_switch)
{
	bool is_silent_overwrite = option_switch._y;
	BaseNode* src_node = FindNodeByTokensInternal(src_tokens);
	assert(src_node != nullptr);
	//�����ƶ�����·���ϵ�Ŀ¼
	if (IsAncestor(src_node, m_working_dir))
	{
		return ReturnType::AccessDenied;//error : �ܾ�����
	}
	//��λԴĿ¼/�ļ�����Ŀ¼
	BaseNode* src_parent_node = src_node->GetParent();
	DirNode* src_parent_dir = static_cast<DirNode*>(src_parent_node);
	//Ŀ��·��������
	auto dst_node = FindNodeByTokensInternal(dst_tokens);
	if (dst_node == nullptr)
	{
		auto dst_parent_tokens = dst_tokens;
		dst_parent_tokens.pop_back();
		auto dst_parent_node = FindNodeByTokensInternal(dst_parent_tokens);
		if (dst_parent_node == nullptr)
		{
			return ReturnType::MemoryPathIsNotFound;//error : �Ҳ����������·��
		}
		//��Ŀ��·����Դ·����ͬһĿ¼��
		if (dst_parent_node->IsDirectory() && IsSameNode(src_parent_node, dst_parent_node))
		{
			//��Դ·��������ΪĿ��·��
			assert(!dst_tokens.empty());
			src_node->SetName(dst_tokens.back());
			return ReturnType::Success;
		}
		else
		{
			return ReturnType::MemoryPathIsNotFound;//error : �Ҳ����������·��
		}
	}
	//Ŀ��·����Ŀ¼
	else if (dst_node->IsDirectory())
	{
		DirNode* dst_dir = static_cast<DirNode*>(dst_node);
		auto child = dst_dir->FindChildByName(src_node->GetName());
		if (child)
		{
			//����Ŀ¼
			if (child->IsDirectory())
			{
				return ReturnType::AccessDenied;//error : �ܾ�����
			}
			//�����ļ�
			else if (!is_silent_overwrite)
			{
				auto ret = Selector(TEXT("���� ") + GetPathByNode(child) + TEXT(" ��? (Yes/No/All):"));
				if (ret == SelectType::no)
				{
					return ReturnType::Success;
				}
			}
			m_tree->DeleteNode(child);
		}
		//move
		m_tree->RemoveButNotDeleteNode(src_node);
		m_tree->InsertNode(dst_dir, src_node);
	}
	//Ŀ��·�����ļ�
	else
	{
		auto new_name = dst_node->GetName();
		auto new_dir = dst_node->GetParent();
		if (new_dir == m_tree->GetRoot())
		{
			return ReturnType::MemoryPathIsNotFound;//�Ҳ����������·��
		}
		if (!is_silent_overwrite)
		{
			auto ret = Selector(TEXT("���� ") + GetPathByNode(dst_node) + TEXT(" ��? (Yes/No/All):"));
			if (ret == SelectType::no)
			{
				return ReturnType::Success;
			}
		}
		//ɾ��Ŀ���ļ�
		if (dst_node)
		{
			m_tree->DeleteNode(dst_node);
		}
		m_tree->RemoveButNotDeleteNode(src_node);
		m_tree->InsertNode(new_dir, src_node);
		//��Դ�ļ�������
		src_node->SetName(new_name);
	}
	//��ӡ��ʾ�İ�
	if (src_node->IsDirectory())
	{
		Console::Write::PrintLine(TEXT("�ƶ���1��Ŀ¼ ") + GetPathByNode(src_node));
	}
	else
	{
		Console::Write::PrintLine(TEXT("�ƶ���1���ļ� ") + GetPathByNode(src_node));
	}
	return ReturnType::Success;
}


ReturnType NodeTreeManager::DelByTokensAndOption(const Path& path, const OptionSwitch& option_switch)
{
	bool is_recursive = option_switch._s;
	BaseNode* target_node = FindNodeByTokensInternal(path.Tokens());
	if (nullptr != target_node)
	{
		//ɾ��Ŀ¼�µ������ļ�
		if (target_node->IsDirectory())
		{
			bool ret = DeleteNodeByWildcardFileName(static_cast<DirNode*>(target_node), TEXT("*"), is_recursive);
			return ReturnType::Success;
		}
		//ɾ�����ļ���ƥ����ļ�
		auto file_name = path.Tokens().back();
		auto cur_dir = static_cast<DirNode*>(target_node->GetParent());
		bool ret = DeleteNodeByWildcardFileName(cur_dir, file_name, is_recursive);
		return ReturnType::Success;
	}
	//���·�����Ƿ���ͨ���
	if (path.IsWild())
	{
		auto dir_tokens = path.Tokens();
		auto file_name = dir_tokens.back();
		dir_tokens.pop_back();
		if (dir_tokens.empty()) 
		{
			dir_tokens.emplace_back(Constant::gs_cur_dir_token);
		}
		auto dir_node = FindNodeByTokensInternal(dir_tokens);
		if (dir_node == nullptr)
		{
			return ReturnType::MemoryPathIsNotFound;
		}
		//����ͨ���·��ƥ���ļ���ɾ��
		bool ret = DeleteNodeByWildcardFileName(static_cast<DirNode*>(dir_node), file_name, is_recursive);
		return ReturnType::Success;
	}
	return ReturnType::MemoryPathIsNotFound;//error : �������·��������
}


bool NodeTreeManager::DeleteNodeByWildcardFileName(DirNode* cur_dir, const string_local& file_name, bool is_recursive)
{
	assert(nullptr != cur_dir);
	auto target_dir = static_cast<DirNode*>(cur_dir);
	std::queue<DirNode*> q;
	q.push(target_dir);
	while (!q.empty())
	{
		DirNode* dir = q.front();
		auto children = dir->Children();
		q.pop();
		//��ʾ�û�ѡ���Ƿ�ɾ��
		SelectType ret = Selector(TEXT("ɾ�� ") + GetPathByNode(dir) + TEXT("\\*,�Ƿ�ȷ��(Yes/No/All)��"));
		if (ret == SelectType::no)
		{
			continue;
		}
		//ɾ����ǰĿ¼�µ�����ƥ����ļ�
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
			Console::Write::PrintLine(TEXT("ɾ���ļ� - ") + GetPathByNode(item));
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
				q.push(static_cast<DirNode*>(item));
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
	auto lstr = GetPathByNode(lhs);
	auto rstr = GetPathByNode(rhs);
	if (StringTools::IsEqual(lstr, rstr))
	{
		return true;
	}
	return false;
}


ReturnType NodeTreeManager::SaveToPath(const Path& path)
{

	return ReturnType::Success;
}


ReturnType NodeTreeManager::LoadFromPath(const Path& path)
{
	return ReturnType::Success;
}


