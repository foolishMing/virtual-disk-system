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
	if (nullptr != m_tree) //删除文件树
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
	//设置新树
	m_tree = tree;
	//加载盘符
	bool init_success = InitDrivens();
	if (!init_success)
	{
		Console::Write::PrintLine(TEXT("虚拟磁盘加载失败,找不到C:盘符"));
		//恢复原树
		m_tree = old_tree;
		return false;
	}
	//删除原树
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
	//打印文件节点信息
	if (cur_node->IsFile())
	{
		auto file_node = dynamic_cast<FileNode*>(cur_node);
		std::wcout << StringTools::TimeStampToDateTimeString(file_node->GetLatestModifiedTimeStamp())
			<< TEXT("    ")
			<< std::right << std::setw(14) << std::to_wstring(file_node->GetSize())
			<< TEXT(" ") << file_node->GetName() << std::endl;
	}
	//打印文件链接信息
	else if(cur_node->IsFileLink())
	{
		auto link_node = dynamic_cast<SymlinkNode*>(cur_node);
		std::wcout << StringTools::TimeStampToDateTimeString(link_node->GetLatestModifiedTimeStamp())
			<< TEXT("    ")
			<< std::left << std::setw(15) << TEXT("<SYMLINK>")
			<< link_node->GetName() 
			<< TEXT(" [") << link_node->GetTargetPath() << TEXT("]") << std::endl;
	}
	//打印目录链接信息
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
//未完成：将返回值类型修改为DirInfo
//is_ad == true,只打印子目录节点
//对于目录，若非盘符，则需要打印.和..
void NodeTreeManager::PrintDirectoryInfo(BaseNode* dir, StatisticInfo& g_info, const OptionSwitch& option)//打印目录信息
{
	assert(dir && dir->IsDirectory());
	StatisticInfo info;
	DirNode* cur_dir = dynamic_cast<DirNode*>(dir);
	Console::Write::PrintLine(TEXT(""));
	Console::Write::PrintLine(GetTruePathOfNode(cur_dir) + TEXT(" 的目录"));
	Console::Write::PrintLine(TEXT(""));
	auto left_margin = 15;
	//对于非盘符节点，打印.和..目录
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
	//遍历子节点，统计并打印节点信息{修改时间、节点类型、节点大小、节点名称}
	for (BaseNode* cur_node : children)
	{
		//统计目录所占空间大小
		info.tot_size += cur_node->GetSize();
		//打印目录节点信息
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
		//打印文件节点信息
		PrintFileNodeInfo(cur_node);
	}
	g_info += info;
	//打印当前目录时，需要将.和..的目录信息统计在内
	if (!IsSameNode(cur_dir, m_cur_driven))
	{
		info.tot_cnt += 2;
		info.dir_cnt += 2;
	}
	PrintStatisticInfo(info, option);
}


// /s递归选项下，不打印目录的统计信息
void NodeTreeManager::PrintStatisticInfo(StatisticInfo& info, const OptionSwitch& option)
{
	const int left_margin = 15;
	//{文件数量、总字节}
	int64_t file_cnt = (option._ad) ? static_cast<int64_t>(0) : (info.tot_cnt - info.dir_cnt);
	std::wcout << std::right << std::setw(left_margin) << std::to_wstring(file_cnt) << TEXT(" 个文件")
		<< std::right << std::setw(left_margin) << std::to_wstring(info.tot_size) << TEXT("  字节") << std::endl;
	if (!option._s)
	{
		//{目录数量、可用字节}
		std::wcout << std::right << std::setw(left_margin) << std::to_wstring(info.dir_cnt) << TEXT(" 个目录")
			<< std::right << std::setw(left_margin) << std::to_wstring(getTotalSystemMemory()) << TEXT("  可用字节") << std::endl;
	}	
}


bool NodeTreeManager::InitDrivens()
{
	assert(m_tree);
	auto root_dir = dynamic_cast<DirNode*>(m_tree->GetRoot());
	//初始化盘符
	for (const auto& driven_name : m_driven_tokens)
	{
		if (root_dir->ContainsChild(driven_name))continue;
		m_tree->InsertNode(root_dir, new DirNode(driven_name));
	}
	m_cur_driven = dynamic_cast<DirNode*>(root_dir->FindChildByName(m_driven_tokens[0]));
	//初始化工作路径
	SetMyWorkingStack({ m_cur_driven });
	return (m_cur_driven) ? true : false;
}


//-constraints:
//1、绝对路径的第一个token一定是盘符（C:）
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

//查找链接文件
FileNode* NodeTreeManager::FindSymlinkedFile(SymlinkNode* node)
{
	return nullptr;
}

//出于简化逻辑考虑，link_path必须是绝对路径
DirNode* NodeTreeManager::FindSymlinkedDirectory(SymlinkNode* node)
{
	if (nullptr == node || !node->IsDirectoryLink())
	{
		return nullptr;	//error : 找不到目录链接
	}
	Path path(node->GetTargetPath());
	auto tokens = path.Tokens();
	assert(IsAbsolutePath(tokens));
	//从根节点开始搜索
	DirNode* cur_dir = dynamic_cast<DirNode*>(m_tree->GetRoot());
	std::queue<string_local, std::deque<string_local>> q(std::deque<string_local>(tokens.begin(), tokens.end()));
	while (!q.empty())
	{
		if (nullptr == cur_dir)
		{
			return nullptr;//error : 找不到链接的目录
		}
		const string_local token = q.front();
		q.pop();
		//匹配当前目录
		if (IsCurrentDir(token))
		{
			continue;
		}
		//匹配(工作目录的)上一级目录
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
		//匹配下一级目录(可以是.或者..)
		BaseNode* next_node = cur_dir->FindChildByName(token);
		//空节点或者根目录是不合法的
		if (nullptr == next_node || IsSameNode(next_node, m_tree->GetRoot()))
		{
			return nullptr;//error : 找不到目录链接
		}
		//非目录（或目录链接）节点是不合法的
		if (!IsDirOrDirLink(next_node))
		{
			return nullptr; //error : 找不到目录链接
		}
		//进入到下一个目录节点
		if (next_node->IsDirectory())
		{
			cur_dir = dynamic_cast<DirNode*>(next_node);
			continue;
		}
		//获取目录链接对应的目录节点
		if (next_node->IsDirectoryLink())
		{
			cur_dir = FindSymlinkedDirectory(dynamic_cast<SymlinkNode*>(next_node));
		}
	}
	return cur_dir;
}


//<update ...>
//-constraints
//1、可以是绝对路径或相对路径，需要进行判断
//2、token为.或..是合法的
//3、token为..时，需要判断空指针
//4、如果待查找的尾节点是.或..应当进行特殊处理
//5、遇到链接节点，需要查找其所链接的真实节点
//6、如果是绝对路径，则临时工作栈初始化为盘符；否则初始化为当前工作栈
BaseNode* NodeTreeManager::FindTrueNodeByTokensInternal(const std::vector<string_local>& tokens, std::deque<BaseNode*>& working_stack)
{
	assert(!tokens.empty());
	std::queue<string_local, std::deque<string_local>> q(std::deque<string_local>(tokens.begin(), tokens.end()));
	//如果是全局路径，则默认不匹配盘符
	if (StringTools::IsEqual(q.front(), m_cur_driven->GetName()))
	{
		q.pop();
	}
	//返回工作路径
	if (q.empty())
	{
		return GetTrueDirNode(working_stack.back());
	}
	while (q.size() > 1)
	{
		if (working_stack.empty())
		{
			return nullptr; //error : 找不到路径
		}
		const string_local token = q.front();
		q.pop();
		//匹配当前目录
		if (IsCurrentDir(token))
		{
			continue;
		}
		//匹配上一级目录
		if (IsParentDir(token))
		{
			working_stack.pop_back();
			if (working_stack.empty())
			{
				return nullptr; //error : 找不到路径
			}
			continue;
		}
		//匹配下一级目录
		DirNode* working_dir = GetTrueDirNode(working_stack.back());
		if (nullptr == working_dir)
		{
			return nullptr; //error : 找不到路径
		}
		auto next_dir = working_dir->FindChildByName(token);
		if (!IsDirOrDirLink(next_dir))
		{
			return nullptr; // error : 找不到路径
		}
		working_stack.emplace_back(next_dir);
		continue;
	}
	if (q.size() > 0)
	{
		const auto token = q.back();
		q.pop();
		//获取当前工作目录
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
		//返回当前目录节点
		if (IsCurrentDir(token))
		{
			return cur_dir;
		}
		//返回上一级目录节点
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
		//返回下一级目录或文件节点
		for (const auto& item : cur_dir->Children())
		{
			//模糊匹配
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
//待完成：若路径创建失败，保留已成功创建的节点
//先判断能否在cur_dir下获取到以token为名的节点
//如果目标节点是文件节点，则mkdir失败，跳出循环
//在cur_dir下创建以token为名的节点
//不允许直接在根目录root下创建新节点（因为根目录下面是驱动目录，驱动应当是只读的）
//需要对.和..token进行判断
//创建节点时，需要判断token是否合法
//需要处理路径上的链接节点
//需要判断子目录或文件是否存在
ReturnType NodeTreeManager::MkdirByTokens(const std::vector<string_local>& tokens)
{
	//初始化工作栈
	std::deque<BaseNode*> temp_working_stack = GetTemporaryWorkingStackByTokens(tokens);
	std::queue<string_local, std::deque<string_local>> q(std::deque<string_local>(tokens.begin(),tokens.end()));
	//如果是全局路径，则默认不匹配盘符
	if (StringTools::IsEqual(q.front(), m_cur_driven->GetName()))
	{
		q.pop();
	}
	bool is_new_dir_created = false;
	//查找路径并创建
	while (!q.empty())
	{
		if (temp_working_stack.empty())
		{
			return ReturnType::MemoryPathIsNotFound; //error : 找不到路径
		}
		string_local token = q.front();
		q.pop();
		//匹配当前目录
		if (IsCurrentDir(token))
		{
			continue;
		}
		//匹配上级目录
		if (IsParentDir(token))
		{
			temp_working_stack.pop_back();
			continue;
		}
		//匹配下一级目录
		DirNode* cur_dir = GetTrueDirNode(temp_working_stack.back());
		if (cur_dir == nullptr)
		{
			return ReturnType::MemoryPathIsNotFound; //error : 找不到路径
		}
		BaseNode* next_node = cur_dir->FindChildByName(token);
		//创建新目录
		if (nullptr == next_node)
		{
			if (cur_dir == m_tree->GetRoot())
			{
				return ReturnType::UnExpectedException; //error : 不允许在根目录下创建新的节点
			}
			if (!PathTools::IsTokensFormatLegal({ token }))
			{
				return ReturnType::MemoryPathNameIsIllegal;
			}
			next_node = new DirNode(token);
			bool ok = m_tree->InsertNode(cur_dir, next_node);
			if (!ok)
			{
				return ReturnType::UnExpectedException;//error : 新目录插入失败
			}
			is_new_dir_created = true;
		}
		//检查下一级目录是否存在
		else if(nullptr == GetTrueDirNode(next_node))
		{
			return ReturnType::MemoryPathIsNotFound; // error : 找不到路径
		}
		//进入该目录
		temp_working_stack.emplace_back(next_node);
	}
	if (temp_working_stack.empty())
	{
		return ReturnType::UnExpectedException;
	}
	//没有创建新的节点
	if (!is_new_dir_created)
	{
		return ReturnType::MemoryPathIsExist;//error : 路径已存在
	}
	return ReturnType::Success;
}


//1、要考虑到工作路径上存在目录链接的情况
//2、若路径列表为空，打印当前路径
//3、不支持模糊匹配
ReturnType NodeTreeManager::ChangeDirByTokens(const std::vector<string_local>& tokens)
{
	//显示工作目录
	if (tokens.empty())
	{
		Console::Write::PrintLine(GetCurrentPath());
		return ReturnType::Success;
	}
	//初始化临时工作栈
	std::deque<BaseNode*> temp_working_stack = GetTemporaryWorkingStackByTokens(tokens);
	//读取token，进行路径匹配
	std::queue<string_local, std::deque<string_local>> q(std::deque<string_local>(tokens.begin(), tokens.end()));
	//如果是全局路径，则默认不匹配盘符
	if (StringTools::IsEqual(q.front(), m_cur_driven->GetName()))
	{
		q.pop();
	}
	while (!q.empty())
	{
		if (temp_working_stack.empty())
		{
			return ReturnType::MemoryPathIsNotFound;//error : 路径不存在
		}
		const string_local token = q.front();
		q.pop();
		//匹配当前目录
		if (IsCurrentDir(token))
		{
			continue;
		}
		//匹配上一级目录
		if (IsParentDir(token))
		{
			temp_working_stack.pop_back();
			continue;
		}
		//匹配下一级目录
		DirNode* cur_dir = GetTrueDirNode(temp_working_stack.back());
		auto next_node = cur_dir->FindChildByName(token);
		if (!IsDirOrDirLink(next_node))
		{
			return ReturnType::MemoryPathIsNotFound; //error : 路径不存在
		}
		temp_working_stack.emplace_back(next_node);
	}
	if (temp_working_stack.empty())
	{
		return ReturnType::MemoryPathIsNotFound; // error : 路径不存在
	}
	//更新全局工作栈
	SetMyWorkingStack(temp_working_stack);
	return ReturnType::Success;
}



//1、需要判断路径是否存在
//2、不允许对真实工作路径上的节点（包括根节点和驱动节点）重命名
//3、需要判断dst_name是否重名
//4、不支持通配符
ReturnType NodeTreeManager::RenameNodeByTokens(const std::vector<string_local>& tokens, string_local dst_name)
{
	assert(PathTools::IsTokensFormatLegal({ dst_name }));//dst_name保证语法正确
	assert(!tokens.empty());
	//查找目标节点所在真实目录
	auto working_stack = GetTemporaryWorkingStackByTokens(tokens);
	string_local target_node_token = tokens.back();//目标节点的token
	DirNode* cur_dir = FindWhichDirectoryIsNodeUnderByTokens(tokens,working_stack);
	//目录不存在
	if (cur_dir == nullptr)
	{
		return ReturnType::MemoryPathIsNotFound;//error : 找不到路径
	}
	//基于最后一个token查找目标节点
	BaseNode* target_node = nullptr;
	//target_node_token是.或..
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
		target_node = working_stack.back();//注意，如果是链接节点，获取其本身，而不需要获取其链接的真实对象
	}
	//匹配子节点
	else
	{
		auto target_node = cur_dir->FindChildByName(target_node_token);
	}
	//目标节点不存在,或为根目录
	if (target_node == nullptr || IsSameNode(target_node, m_tree->GetRoot()))
	{
		return ReturnType::MemoryPathIsNotFound;//error : 找不到路径
	}
	//判断目标节点是否在工作路径上
	DirNode* working_dir = GetTrueDirNode(m_working_stack.back());
	if (IsTrueAncestor(target_node, working_dir))
	{
		return ReturnType::AccessDenied;//error : 非法访问
	}
	//判断是否有重名文件
	auto dst_node = cur_dir->FindChildByName(dst_name);
	if (nullptr != dst_node && !IsSameNode(target_node, dst_node))//重名，且不是原文件
	{
		return ReturnType::MemoryFileIsExist;
	}	
	//重命名（在command中已经检查过dst_name是否合法）
	target_node->SetName(dst_name);
	return ReturnType::Success;
}


//<udpate>
//如果目标节点是文件，则打印文件信息
//如果目标节点是
//选项/ad打印子目录信息
//选项/s递归打印子目录及文件信息
// /s下，不统计目录信息
// /ad下，不统计文件信息
ReturnType NodeTreeManager::DisplayDirNodeByTokensAndOptions(const std::vector<string_local>& tokens, const OptionSwitch& option_switch)	
{
	assert(!tokens.empty());
	auto working_stack = GetTemporaryWorkingStackByTokens(tokens);
	BaseNode* target_node = FindTrueNodeByTokensInternal(tokens, working_stack);//directory or file
	if (nullptr == target_node)
	{
		return ReturnType::MemoryPathIsNotFound;//找不到路径
	}
	const bool is_recursive = option_switch._s;
	//目标节点为文件节点，打印文件
	if (target_node->IsFile())
	{
		PrintFileNodeInfo(target_node);
		return ReturnType::Success;
	}
	//打印目标目录节点子目录及文件信息
	DirNode* cur_dir_node = dynamic_cast<DirNode*>(target_node);
	std::queue<DirNode*> q = {};
	q.push(cur_dir_node);
	StatisticInfo g_info;
	while (!q.empty())
	{
		DirNode* dir = q.front();
		q.pop();
		//打印当前目录下的子目录及文件信息
		PrintDirectoryInfo(dir, g_info, option_switch);
		if (!is_recursive)
		{
			break;//非递归，终止遍历
		}
		//将子目录放在待打印目录节点队列中
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
		Console::Write::PrintLine(TEXT("\n     所列文件总数:"));
		g_info.tot_size = cur_dir_node->GetSize();
		OptionSwitch os = option_switch;
		os._s = false;
		PrintStatisticInfo(g_info, os);
	}
	return ReturnType::Success;
}


//工作目录上的节点不能被删除
//选项/s:递归移除目录下的所有子目录与文件
//若path指向文件或文件链接，则无法删除
//删除目录链接时，只应该删除其本身，而不删除其链接的目录
ReturnType NodeTreeManager::RemoveDirByTokensAndOptions(const std::vector<string_local>& tokens, const OptionSwitch& option_switch)
{
	assert(!tokens.empty());
	const string_local target_node_token = tokens.back();
	//找到目标节点所在目录
	auto temp_working_stack = GetTemporaryWorkingStackByTokens(tokens);
	DirNode* parent_dir = FindWhichDirectoryIsNodeUnderByTokens(tokens, temp_working_stack);
	if (nullptr == parent_dir)
	{
		return ReturnType::MemoryPathIsNotFound; //error : 目标路径不存在
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
	//空目录或根目录不能删除
	if (nullptr == target_node || IsSameNode(target_node, m_tree->GetRoot()))
	{
		return ReturnType::MemoryPathIsNotFound; //error : 目标路径不存在
	}
	//非目录(或目录链接)节点无法删除
	if (!IsDirOrDirLink(target_node))
	{
		return ReturnType::DirNameIsInvalid;
	}
	//当前工作目录上的节点无法删除
	DirNode* cur_working_dir = GetTrueDirNode(GetMyWorkingStack().back());
	if (IsTrueAncestor(target_node, cur_working_dir))
	{
		return ReturnType::AccessDenied;
	}
	//如果是目录链接，则只删除其本身
	if (target_node->IsDirectoryLink())
	{
		bool ok = m_tree->DeleteNode(target_node);
		return ok ? ReturnType::Success : ReturnType::UnExpectedException;
	}
	//如果是目录
	DirNode* cur_dir = dynamic_cast<DirNode*>(target_node);
	//空目录，直接删除
	if (cur_dir->Children().empty())
	{
		bool ok = m_tree->DeleteNode(cur_dir);
		return ok ? ReturnType::Success : ReturnType::UnExpectedException;
	}
	//非空目录，判断是否有/s参数
	if (true != option_switch._s)
	{
		return ReturnType::MemoryDirIsNotEmpty;
	}
	//递归删除
	else
	{
		bool ok = m_tree->DeleteNode(cur_dir);
		return ok ? ReturnType::Success : ReturnType::UnExpectedException;
	}
}

//<update>
//待更新：对SymlinkNode的处理逻辑
//1、如果目标路径是目录，则复制到目录下
//2、如果目标路径是文件，则对目标文件进行覆盖写入
ReturnType NodeTreeManager::CopyFromDiskToMemory(const std::vector<string_local>& file_path_vec, const std::vector<string_local>& dst_path_tokens, const OptionSwitch& option_switch)
{
	//找到真实的目标节点（directory or file）
	auto temp_working_stack = GetTemporaryWorkingStackByTokens(dst_path_tokens);
	BaseNode* target_node = FindTrueNodeByTokensInternal(dst_path_tokens, temp_working_stack);
	if (target_node == nullptr)
	{
		return ReturnType::MemoryPathIsNotFound;
	}
	//目标路径是目录
	if (target_node->IsDirectory())
	{
		DirNode* target_dir = dynamic_cast<DirNode*>(target_node);
		CopyFromDiskToMemoryToDirectory(file_path_vec, target_dir, option_switch);
	}
	//目标路径是文件
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
	bool is_silent_overwrite_all = option_switch._y; //是否默认覆盖全部重名文件
	int copy_count = 0;
	for (const auto& file_path : file_path_vec)
	{
		assert(PathTools::IsDiskPathExist(file_path));
		//读取磁盘文件内容
		size_t file_size = 0;
		char* buffer = ReadDiskFileData(StringTools::WStringToString(file_path).c_str(), file_size);//申请内存
		if (buffer == nullptr)
		{
			Console::Write::PrintLine(TEXT("读取磁盘文件 ") + file_path + TEXT(" 失败"));
			continue;
		}
		string_local file_name = PathTools::GetFileName(file_path);
		//创建新的文件
		if (!target_dir->ContainsChild(file_name))
		{
			m_tree->InsertNode(target_dir, new FileNode(file_name));
		}
		//静默覆盖重名文件
		else if (is_silent_overwrite_all)
		{
		}
		//提示用户是否覆盖重名文件
		else
		{
			string_local dir_path = GetTruePathOfNode(target_dir);
			SelectType select = Selector(TEXT("覆盖 ") + dir_path + TEXT("/") + file_name + TEXT(" 吗? (Yes/No/All):"));
			if (select == SelectType::all)
			{
				is_silent_overwrite_all = true;
			}
			//跳过当前文件
			else if (select == SelectType::no)
			{
				continue;
			}
		}
		BaseNode* node = target_dir->FindChildByName(file_name);
		OverwriteFileNode(node, buffer,file_size);
		free(buffer); //释放内存
		copy_count++;
		Console::Write::PrintLine(TEXT("复制文件 ") + file_path + TEXT(" 成功"));
	}
	Console::Write::PrintLine(TEXT("已复制 ") + StringTools::FormatFromNumber(copy_count) + TEXT(" 个文件"));
}


void NodeTreeManager::CopyFromDiskToMemoryFile(const std::vector<string_local>& file_path_vec, FileNode* dst_node, const OptionSwitch& option_switch)
{
	//update ...
}

//查找节点所在目录
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
//待更新：暂未处理追加写入目标文件的相关逻辑
ReturnType NodeTreeManager::CopyFromMemoryToMemory(const std::vector<string_local>& src_tokens, const std::vector<string_local>& dst_tokens, const OptionSwitch& option_switch)
{
	const auto& src_file_match_pattern = src_tokens.back();//待匹配文件名
	//查找源节点所在目录
	auto temp_working_stack = GetTemporaryWorkingStackByTokens(src_tokens);
	DirNode* src_dir = FindWhichDirectoryIsNodeUnderByTokens(src_tokens, temp_working_stack);
	if (src_dir == nullptr)
	{
		return ReturnType::MemoryPathIsNotFound;//error : 路径不存在
	}
	//获取源文件列表
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
		return ReturnType::MemoryPathIsNotFound;//error : 源路径不存在
	}
	//获取目标节点
	temp_working_stack = GetTemporaryWorkingStackByTokens(dst_tokens);
	BaseNode* dst_node = FindTrueNodeByTokensInternal(dst_tokens, temp_working_stack);
	if (nullptr == dst_node || dst_node->IsFileLink() || dst_node->IsDirectoryLink())
	{
		return ReturnType::MemoryPathIsNotFound;//error : 源路径不存在
	}
	//拷贝到目录
	if (dst_node->IsDirectory())
	{
		CopyFromMemoryToMemoryDirectory(src_nodes, dynamic_cast<DirNode*>(dst_node), option_switch);
	}
	//拷贝到文件
	else if(dst_node->IsFile())
	{
		//update 待完成
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
	bool is_silent_overwrite_all = option_switch._y; //是否默认覆盖全部重名文件
	int copy_count = 0;
	for (const auto& file_node : node_list)
	{
		auto file_name = file_node->GetName();
		//创建新的文件
		if (!target_dir->ContainsChild(file_name))
		{
			m_tree->InsertNode(target_dir, new FileNode(file_name));
		}
		//提示用户是否覆盖目标文件
		else if (!is_silent_overwrite_all)
		{
			string_local dir_path = GetTruePathOfNode(target_dir);
			SelectType select = Selector(TEXT("覆盖 ") + dir_path + TEXT("/") + file_name + TEXT(" 吗? (Yes/No/All):"));
			if (select == SelectType::all)
			{
				is_silent_overwrite_all = true;
			}
			//跳过当前文件
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
				Console::Write::PrintLine(TEXT("文件链接") + GetTruePathOfNode(file_node) + TEXT("已失效"));
				continue;
			}
			data = link_node->GetData();
		}
		assert(data != nullptr);
		OverwriteFileNode(node, data, file_node->GetSize());
		copy_count++;
		Console::Write::PrintLine(TEXT("复制文件 ") + GetTruePathOfNode(file_node) + TEXT(" 成功"));
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


//1、逻辑简化：只允许使用绝对路径创建软链接
//2、(待完成)逻辑简化：如果为链接创建链接，不允许在树上产生环路
//3、逻辑简化：不允许软链接与链接对象之间类型不匹配
ReturnType NodeTreeManager::MklinkByPathAndOptions(const Path& link_path, const Path& src_path, const OptionSwitch& option_switch)
{
	bool is_directory_link = option_switch._d;
	//检查源路径与目标路径是否合法
	if (!src_path.IsValid() || !link_path.IsValid())
	{
		return ReturnType::MemoryPathNameIsIllegal;
	}
	const auto& src_tokens = src_path.Tokens();
	const auto& dst_tokens = link_path.Tokens();
	//检查源路径与目标路径是否为空
	if (src_tokens.empty() || dst_tokens.empty())
	{
		return ReturnType::MemoryPathIsNotFound;
	}
	//检查源路径是否为绝对路径
	if (!IsAbsolutePath(src_tokens))
	{
		Console::Write::PrintLine(TEXT("源路径不是绝对路径"));
		return ReturnType::UnExpectedException;
	}
	//获取源路径所在目录
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
	//链接与链接对象类型不匹配
	if (is_directory_link && !(src_node->IsDirectory() || src_node->IsDirectoryLink()))
	{
		return ReturnType::TypeOfLinkAndSourceIsNotMatch;
	}
	if (!is_directory_link && !(src_node->IsFile() || src_node->IsFileLink()))
	{
		return ReturnType::TypeOfLinkAndSourceIsNotMatch;
	}
	//对于已存在的合法节点，允许创建链接
	//获得链接所在目录
	auto dst_node_token = dst_tokens.back();
	auto dst_working_stack = GetTemporaryWorkingStackByTokens(dst_tokens);
	DirNode* dst_parent_dir = FindWhichDirectoryIsNodeUnderByTokens(dst_tokens, dst_working_stack);
	if (nullptr == dst_parent_dir)
	{
		return ReturnType::MemoryPathIsNotFound;
	}
	//判断是否有重名文件
	auto symlink_node = dst_parent_dir->FindChildByName(dst_node_token);
	if (nullptr != symlink_node)
	{
		return ReturnType::MemoryFileIsExist;//存在重名文件
	}
	//创建软链接节点，并插入树中
	symlink_node = new SymlinkNode(dst_node_token);
	NodeType link_type = (is_directory_link) ? NodeType::SymlinkD : NodeType::SymlinkF;
	dynamic_cast<SymlinkNode*>(symlink_node)->SetTarget(link_type, src_path.ToString());
	m_tree->InsertNode(dst_parent_dir, symlink_node);
	return ReturnType::Success;
}


//待完成：使用通配符匹配多个源路径
//1、若dst目录下有重名文件/目录，可以被覆盖
//2、真实工作路径上的节点不允许move
//3、源路径必须存在；目标路径可以不存在，但其所在目录必须存在,此时需要将源目录/文件重命名
//4、src可以是链接文件，dst必须是真实目录
ReturnType NodeTreeManager::MoveByTokensAndOptions(const Path& src_path, const Path&  dst_path, const OptionSwitch& option_switch)
{
	bool is_silent_overwrite = option_switch._y; //是否静默覆盖
	bool is_need_rename = false;//是否需要重命名
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
	//获取源节点
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
		src_node = src_working_stack.back();//注意，如果src_node是一个链接节点，需要获取其本身，而不是其链接对象
	}
	else
	{
		src_node = src_parent_dir->FindChildByName(src_node_token);
	}
	//判断源节点是否可以move
	//不能移动空节点或根目录
	if (nullptr == src_node || IsSameNode(src_node, m_tree->GetRoot()))
	{
		return ReturnType::MemoryPathIsNotFound;
	}
	//不能移动工作路径上的目录
	if (IsTrueAncestor(src_node, GetMyWorkingStack().back()))
	{
		return ReturnType::AccessDenied;//error : 拒绝访问
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
	//寻找目标目录
	DirNode* target_dir = nullptr;
	{
		//目标节点不存在，需要对源节点重命名
		if (dst_node == nullptr)
		{
			target_dir = dst_parent_dir;
			is_need_rename = true;
		}
		//目标节点是文件节点，提示用户是否覆盖
		else if(dst_node->IsFile() || dst_node->IsFileLink())
		{
			target_dir = dst_parent_dir;
		}
		//目标节点是目录链接，获取其链接的真实目录，并进入
		else if (dst_node->IsDirectoryLink())
		{
			target_dir = GetTrueDirNode(dst_node);
		}
		//目标节点是目录，进入
		else if (dst_node->IsDirectory())
		{
			target_dir = dynamic_cast<DirNode*>(dst_node);
		}
	}
	if (target_dir == nullptr)
	{
		return ReturnType::MemoryPathIsNotFound;
	}
	//判断目标目录下是否有重名节点，如果有，则提示用户是否覆盖
	{
		auto node = target_dir->FindChildByName(dst_node_token);
		if (nullptr != node)
		{
			if (!is_silent_overwrite)
			{
				auto sel_ret = Selector(TEXT("覆盖") + dst_path.ToString() + TEXT("吗？（Yes/No/All）"));
				if (sel_ret == SelectType::no)
				{
					return ReturnType::Success;
				}
			}
			//删除重名节点
			m_tree->DeleteNode(dst_node);
		}
	}
	//进行移动操作
	m_tree->RemoveButNotDeleteNode(src_node);
	m_tree->InsertNode(target_dir, src_node);
	//重命名
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
	//打印提示文案
	if (IsDirOrDirLink(src_node))
	{
		Console::Write::PrintLine(TEXT("移动了1个目录 ") + GetTruePathOfNode(src_node));
	}
	else
	{
		Console::Write::PrintLine(TEXT("移动了1个文件 ") + GetTruePathOfNode(src_node));
	}
	return ReturnType::Success;
}


//1、输入路径是目录，则默认删除目录下所有文件
//2、由于不影响目录，所以可以回溯到工作路径上进行文件删除工作
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
	//获取目标节点
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
	//目标节点不能是根节点
	if (nullptr != target_node && IsSameNode(target_node, m_tree->GetRoot()))
	{
		return ReturnType::MemoryPathIsNotFound;
	}

	if (nullptr != target_node)
	{
		//目标节点是目录或目录链接
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
		//目标节点是文件或文件链接
		else
		{
			DeleteNodeByWildcardFileName(parent_dir, file_name_pattern, is_recursive);
			return ReturnType::Success;
		}
	}
	//使用通配符匹配目录下的文件并删除
	else if (path.IsWild())
	{
		//基于通配符路径匹配文件并删除
		bool ret = DeleteNodeByWildcardFileName(parent_dir, file_name_pattern, is_recursive);
		return ReturnType::Success;
	}
	return ReturnType::MemoryPathIsNotFound;//error : 虚拟磁盘路径不存在
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
		//如果删除全部文件，则提示用户确认
		if (StringTools::IsEqual(file_name, TEXT("*")))
		{
			SelectType ret = Selector(TEXT("删除 ") + GetTruePathOfNode(dir) + TEXT("\\*,是否确认(Yes/No/All)？"));
			if (ret == SelectType::no)
			{
				continue;
			}
		}
		//删除当前目录下所有匹配file_name的文件
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
			Console::Write::PrintLine(TEXT("删除文件 - ") + GetTruePathOfNode(item));
			m_tree->DeleteNode(item);
		}
		del_vec.clear();
		//如果没有递归选项，跳出循环
		if (!is_recursive)
		{
			break;
		}
		//将所有子目录节点推入队列
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
		Console::Write::PrintLine(TEXT("XML文件保存失败"));
		return ReturnType::UnExpectedException;
	}
	return ReturnType::Success;
}

//<udpate>
//load要做容错处理，如果xml数据格式有问题，则需要返回相应的错误信息
ReturnType NodeTreeManager::LoadFromPath(const string_local& path_str)
{
	assert(PathTools::IsDiskPathExist(path_str));
	tinyxml2::XMLDocument xml_doc;
	if (xml_doc.LoadFile(StringTools::UnicodeToUtf8(path_str.c_str())))
	{
		return ReturnType::LoadXmlFileFailed;
	}
	auto xml_root = xml_doc.RootElement();
	//读取XML文件
	BaseNode* new_root = ReadXml(xml_root, nullptr);
	if (nullptr == new_root)
	{
		return ReturnType::LoadXmlFileFailed;
	}
	//创建新虚拟磁盘
	auto new_tree = new NodeTree();
	new_tree->Create(new_root);
	//加载该虚拟磁盘
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
		//创建目录对象
		if (node->IsDirectory())
		{
			auto dir_node = dynamic_cast<DirNode*>(node);
			ele_node = WriteDirToXml(dir_node, doc);
		}
		//创建文件对象
		else if (node->IsFile())
		{
			auto file_node = dynamic_cast<FileNode*>(node);
			ele_node = doc.NewElement(XMLIdentifier::FileNode);
			ele_node->SetAttribute(XMLIdentifier::data, file_node->GetData());
			ele_node->SetAttribute(XMLIdentifier::dataSize, file_node->GetSize());
		}
		//创建链接对象
		else
		{
			auto link_node = dynamic_cast<SymlinkNode*>(node);
			ele_node = doc.NewElement(XMLIdentifier::SymlinkNode);
			ele_node->SetAttribute(XMLIdentifier::symlinkPath, StringTools::UnicodeToUtf8(link_node->GetTargetPath().c_str()));
		}
		//设置子节点的基本属性
		ele_node->SetAttribute(XMLIdentifier::name, StringTools::UnicodeToUtf8(node->GetName().c_str()));
		ele_node->SetAttribute(XMLIdentifier::id, node->GetId());
		ele_node->SetAttribute(XMLIdentifier::type, static_cast<int>(node->GetType()));
		ele_node->SetAttribute(XMLIdentifier::timestamp, node->GetLatestModifiedTimeStamp());
		ele_node->SetAttribute(XMLIdentifier::parentId, dir->GetId());
		//追加子节点
		//ele_children->LinkEndChild(ele_node);
		xml_dir->LinkEndChild(ele_node);
	}
	//xml_dir->LinkEndChild(ele_children);
	return xml_dir;
}


/// \brief 读XML，创建节点树
BaseNode* NodeTreeManager::ReadXml(tinyxml2::XMLElement* xml_item, DirNode* parent)
{
	assert(xml_item);
	string_local node_name = StringTools::Utf8ToUnicode(xml_item->FindAttribute(XMLIdentifier::name)->Value());
	int64_t node_id = xml_item->FindAttribute(XMLIdentifier::id)->Int64Value();
	int32_t type_int = xml_item->FindAttribute(XMLIdentifier::type)->IntValue();
	int64_t node_timestamp = xml_item->FindAttribute(XMLIdentifier::timestamp)->Int64Value();
	if (!IsNodeAttributeIsValid(node_name, node_id, type_int, node_timestamp))
	{
		return nullptr;//error : xml数据异常
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
	//插入当前节点
	if (parent && node && !parent->ContainsChild(node->GetName())) 
	{
		parent->AppendChild(node);
		node->SetParent(parent);
	}
	if (!node->IsDirectory())
	{
		return node;
	}
	//遍历儿子节点
	for (auto iter = xml_item->FirstChild(); iter != 0; iter = iter->NextSibling())
	{
		auto ele = iter->ToElement();
		auto child = ReadXml(ele, dynamic_cast<DirNode*>(node));
	}
	return node;
}


bool NodeTreeManager::InsertSymlinkNodeByXml(std::vector<tinyxml2::XMLElement*>& link_elems)
{
	//插入链接节点
	for (const auto& item : link_elems)
	{
		//从XML对象中解析节点属性
		auto attr_name = item->FindAttribute(XMLIdentifier::name)->Value();
		auto attr_id = item->FindAttribute(XMLIdentifier::id)->Int64Value();
		auto attr_type = item->FindAttribute(XMLIdentifier::type)->IntValue();
		auto attr_timestamp = item->FindAttribute(XMLIdentifier::timestamp)->Int64Value();
		auto attr_parentId = item->FindAttribute(XMLIdentifier::parentId)->Int64Value();
		auto attr_symlinkPath = item->FindAttribute(XMLIdentifier::symlinkPath)->Value();
		auto parent_node = m_tree->FindNodeById(attr_parentId);
		assert(parent_node);
		//获得父节点
		auto parent_dir = dynamic_cast<DirNode*>(parent_node);
		//创建链接节点
		SymlinkNode* node = new SymlinkNode(StringTools::Utf8ToUnicode(attr_name), parent_dir);
		//设置链接路径
		node->SetTarget(static_cast<NodeType>(attr_type), StringTools::Utf8ToUnicode(attr_symlinkPath));
		//设置ID
		node->SetId(attr_id);
		//设置修改时间
		node->SetLatestModifiedTimeStamp(attr_timestamp);
		//插入链接节点
		m_tree->InsertNode(parent_node, node);
	}
	return true;
}

//可能存在的问题：没有校验FileNode->data && SymlinkNode->path
bool NodeTreeManager::IsNodeAttributeIsValid(const string_local& name, const int64_t& id, const int32_t& type, const int64_t& timestamp)
{
	//token不能包含非法字符
	if (!PathTools::IsTokensFormatLegal({ name }))
	{
		return false;
	}
	//id、timestamp不能是负数
	if (id < static_cast<int64_t>(0) || timestamp < static_cast<int64_t>(0))
	{
		return false;
	}
	//type必须是在nodetype范围内
	if (!static_cast<int32_t>(NodeType::Directory) == type || !static_cast<int32_t>(NodeType::File) == type || 
		!static_cast<int32_t>(NodeType::SymlinkF) == type || !static_cast<int32_t>(NodeType::SymlinkD) == type)
	{
		return false;
	}
	return true;
}

