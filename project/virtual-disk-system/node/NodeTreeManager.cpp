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
	//删除文件树
	if (nullptr != m_tree) 
	{
		m_tree->Destroy();
		m_tree = nullptr;
	}
	//清空工作队列
	m_working_stack.clear();
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
	assert(m_tree && !m_working_stack.empty());
	string_local ret = {};
	for (const auto& item : m_working_stack)
	{
		ret += item->GetName();
		if (item != m_working_stack.back())
		{
			ret += TEXT("\\");
		}
	}
	//只有盘符
	if (m_working_stack.size() == 1)
	{
		ret += TEXT("\\");
	}
	return ret;
}

void NodeTreeManager::PrintFileNodeInfo(BaseNode* cur_node)
{
	assert(cur_node);
	//打印子文件节点信息
	if (cur_node->IsFile())
	{
		auto file_node = static_cast<FileNode*>(cur_node);
		std::wcout << StringTools::TimeStampToDateTimeString(file_node->GetLatestModifiedTimeStamp())
			<< TEXT("    ")
			<< std::right << std::setw(14) << std::to_wstring(file_node->GetSize())
			<< TEXT(" ") << file_node->GetName() << std::endl;
	}
	//打印文件链接信息
	else if(cur_node->IsFileLink())
	{
		auto link_node = static_cast<SymlinkNode*>(cur_node);
		std::wcout << StringTools::TimeStampToDateTimeString(link_node->GetLatestModifiedTimeStamp())
			<< TEXT("    ")
			<< std::left << std::setw(15) << TEXT("<SYMLINK>")
			<< link_node->GetName() << std::endl;
	}
	//打印目录链接信息
	else
	{
		auto link_node = static_cast<SymlinkNode*>(cur_node);
		std::wcout << StringTools::TimeStampToDateTimeString(link_node->GetLatestModifiedTimeStamp())
			<< TEXT("    ")
			<< std::left << std::setw(15) << TEXT("<SYMLINKD>")
			<< link_node->GetName() 
			<< TEXT(" [") << link_node->GetTargetPath() << TEXT("]") << std::endl;
	}
}

//<udpate ...>
//未完成：输出内容格式化
//未完成：将返回值类型修改为DirInfo
//is_ad == true,只打印子目录节点
void NodeTreeManager::PrintDirectoryInfo(BaseNode* dir, StatisticInfo& g_info, bool is_ad)//打印目录信息
{
	assert(dir && dir->IsDirectory());
	StatisticInfo info;
	DirNode* cur_dir = static_cast<DirNode*>(dir);
	Console::Write::PrintLine(TEXT(""));
	Console::Write::PrintLine(GetPathByNode(cur_dir) + TEXT(" 的目录"));
	Console::Write::PrintLine(TEXT(""));
	
	auto children = cur_dir->Children();
	info.tot_cnt = children.size();
	//遍历子节点，统计并打印节点信息{修改时间、节点类型、节点大小、节点名称}
	for (BaseNode* cur_node : children)
	{
		info.tot_size += cur_node->GetSize();
		//打印目录节点信息
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
		//打印文件节点信息
		PrintFileNodeInfo(cur_node);
	}
	PrintStatisticInfo(info);
	g_info += info;
}

void NodeTreeManager::PrintStatisticInfo(StatisticInfo& info)
{
	//{文件数量、总字节}
	std::wcout << std::right << std::setw(20) << std::to_wstring(info.tot_cnt - info.dir_cnt) << TEXT(" 个文件")
		<< std::right << std::setw(20) << std::to_wstring(info.tot_size) << TEXT("  字节") << std::endl;
	//{目录数量、可用字节}
	std::wcout << std::right << std::setw(20) << std::to_wstring(info.dir_cnt) << TEXT(" 个目录")
		<< std::right << std::setw(20) << std::to_wstring(getTotalSystemMemory()) << TEXT("  可用字节") << std::endl;
}


bool NodeTreeManager::InitDrivens()
{
	assert(m_tree);
	auto root_dir = static_cast<DirNode*>(m_tree->GetRoot());
	//初始化盘符
	for (const auto& driven_name : m_driven_tokens)
	{
		if (root_dir->ContainsChild(driven_name))continue;
		m_tree->InsertNode(root_dir, new DirNode(driven_name));
	}
	m_cur_driven = static_cast<DirNode*>(root_dir->FindChildByName(m_driven_tokens[0]));
	//初始化工作路径
	m_working_stack.push_back(m_cur_driven);
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
	return StringTools::IsEqual(tokens[0], m_cur_driven->GetName());
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
	BaseNode* target_node = FindNodeByTokensInternal(tokens, m_working_stack);
	return (target_node != nullptr && target_node != m_tree->GetRoot()) ? true : false;
}


//注意：此方法不需要考虑祖先节点中存在链接节点的情况，
//因为软链接节点只持有路径信息，它不是真正的节点！
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
//1、可以是绝对路径或相对路径，需要进行判断
//2、token为.或..是合法的
//3、token为..时，需要判断空指针
//6、如果是链接节点，则通过其持有的路径，获取其指向的目录节点
BaseNode* NodeTreeManager::FindNodeByTokensInternal(std::vector<string_local> tokens, std::deque<BaseNode*> que)
{
	if (que.empty() || tokens.empty())
	{
		return nullptr;
	}
	//创建工作栈
	std::deque<BaseNode*> working_stack;
	if (IsAbsolutePath(tokens))//从盘符开始搜索
	{
		working_stack = std::deque<BaseNode*>{ m_cur_driven };
		tokens.erase(tokens.begin());
	}
	else//从当前工作目录开始搜索
	{
		working_stack = que;
	}
	std::queue<string_local, std::deque<string_local>> token_que(std::deque<string_local>(tokens.begin(), tokens.end()));
	while (token_que.size() > 1)
	{
		if (working_stack.empty())
		{
			return false;
		}
		//获得当前所在节点
		auto cur_node = working_stack.back();
		//获得待匹配的token
		auto token = token_que.front();
		token_que.pop();
		//停留在当前节点
		if (IsCurrentDirToken(token))
		{
			continue;
		}
		//返回上一级节点
		if (IsParentDirToken(token))
		{
			working_stack.pop_back();
			continue;
		}
		//若当前节点是目录节点，搜索下一级节点
		if (cur_node->IsDirectory())
		{
			//查找后继节点
			auto next_node = static_cast<DirNode*>(cur_node)->FindChildByName(token);//注意：在前继节点中，token不支持通配符
			if (nullptr == next_node)
			{
				return nullptr; //目标节点不存在
			}
			//更新当前所在目录
			working_stack.push_back(next_node);
			continue;
		}
		//若当前节点是目录链接，则通过路径获取其索引的节点
		if (cur_node->IsDirectoryLink())
		{
			Path path(static_cast<SymlinkNode*>(cur_node)->GetTargetPath());
			auto next_node = FindNodeByTokensInternal(path.Tokens(), working_stack);
			if (nullptr == next_node)
			{
				return nullptr;
			}
			working_stack.push_back(next_node);
			continue;
		}
		return nullptr;
	}
	//工作栈为空
	if (working_stack.empty())
	{
		return false;
	}
	auto& token = tokens.front();
	//目标节点为当前节点.，更新之
	if (IsCurrentDirToken(token))
	{
		return working_stack.back();
	}
	//目标节点为父节点..，更新之
	if (IsParentDirToken(token))
	{
		working_stack.pop_back();
		if (working_stack.empty())
		{
			return false;
		}
		return working_stack.back();
	}
	//如果当前节点是目录节点,则查找下一级节点
	auto& cur_node = working_stack.back();
	if (cur_node->IsDirectory())
	{
		auto children = static_cast<DirNode*>(cur_node)->Children();
		for (auto child : children)
		{
			if (StringTools::IsEqual(child->GetName(), token))
			{
				return child;
			}
		}
		return nullptr;
	}
	//如果当前节点是目录链接，则通过路径获取其索引的节点
	if (cur_node->IsDirectoryLink())
	{
		Path path(static_cast<SymlinkNode*>(cur_node)->GetTargetPath());
		auto link_node = FindNodeByTokensInternal(path.Tokens(), working_stack);
		return link_node;
	}
	return nullptr;
}


//<update>
//待完成：若路径创建失败，则需要删除本次操作新建的所有节点
//先判断能否在cur_dir下获取到以token为名的节点
//如果目标节点是文件节点，则mkdir失败，跳出循环
//在cur_dir下创建以token为名的节点
//不允许直接在根目录root下创建新节点（因为根目录下面是驱动目录，驱动应当是只读的）
//需要对.和..token进行特殊处理
//创建节点时，需要判断token是否合法
//注意：若某一子目录创建失败，需要保留其成功创建的父目录
bool NodeTreeManager::MkdirByTokens(const std::vector<string_local>& tokens)
{
	//保存一份工作栈的拷贝
	auto working_stack = m_working_stack;
	std::queue<string_local, std::deque<string_local>> token_que(std::deque<string_local>(tokens.begin(),tokens.end()));
	//搜索已创建的节点(目录或目录索引)
	while (!token_que.empty())
	{
		if (working_stack.empty())
		{
			Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound);
			return false;
		}
		string_local token = token_que.front();
		token_que.pop();
		//检查token语法
		if (!PathTools::IsTokensFormatLegal({ token }))
		{
			Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//文件、目录或卷名称语法错误
			return false;
		}
		//当前目录.
		if (IsCurrentDirToken(token))
		{
			continue;
		}
		//回到上级目录..
		if (IsParentDirToken(token))
		{
			working_stack.pop_back();
			if (working_stack.empty())
			{
				Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound);
				return false;
			}
			continue;
		}
		auto cur_node = working_stack.back();
		//非目录（链接）节点，md失败
		if (cur_node->GetType() == NodeType::File || cur_node->GetType() == NodeType::SymlinkF)
		{
			return false;
		}
		//目录节点，查找下一级节点
		if (cur_node->IsDirectory())
		{
			auto dir = static_cast<DirNode*>(cur_node);
			auto next_node = dir->FindChildByName(token);
			if (nullptr != next_node)
			{
				working_stack.push_back(next_node);
				continue;
			}
			//目标节点不存在，在cur_dir下创建并插入，随之更新工作栈
			next_node = new DirNode(token);
			bool ok = m_tree->InsertNode(dir, next_node);
			if (!ok)
			{
				return false;
			}
			working_stack.push_back(next_node);
			continue;
		}	
		//目录链接节点，获取其链接的节点
		if (cur_node->IsDirectoryLink())
		{
			Path path(static_cast<SymlinkNode*>(cur_node)->GetTargetPath());
			auto next_node = FindNodeByTokensInternal(path.Tokens(), working_stack);
			if (nullptr == next_node)
			{
				Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound);
				return false;
			}
			working_stack.push_back(next_node);
			continue;
		}
	}
	return true;
}


bool NodeTreeManager::ChangeDirByTokens(const std::vector<string_local>& tokens)
{
	//显示工作目录
	if (tokens.empty())
	{
		Console::Write::PrintLine(GetCurrentPath());
		return true;
	}
	//寻找目标节点
	BaseNode* target_node = FindNodeByTokensInternal(tokens,m_working_stack);
	assert(nullptr != target_node);
	//目标节点为目录节点或目录链接节点,切换路径
	if (target_node->IsDirectory() || target_node->IsDirectoryLink())
	{
		m_working_node = target_node;
		return true;
	}
	return false;
}


//不允许对工作目录上的节点（包括根节点和驱动节点）重命名
//判断dst_name是否重名
bool NodeTreeManager::RenameNodeByTokens(const std::vector<string_local>& tokens, string_local dst_name)
{
	assert(PathTools::IsTokensFormatLegal({ dst_name }));//dst_name保证语法正确
	//查找目标节点
	BaseNode* target_node = FindNodeByTokensInternal(tokens,m_working_node);
	assert(nullptr != target_node);//target_node保证不为空
	//判断节点是否在当前工作路径上
	if (IsAncestor(target_node, m_working_node))
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryPathAccessDenied);//error : 非法访问
		return false;
	}
	//查找目标节点所属目录下的重名节点
	DirNode* parent_dir = static_cast<DirNode*>(target_node->GetParent());
	auto child = parent_dir->FindChildByName(dst_name);
	//查找到重名节点
	if (child && child->GetId() != target_node->GetId())
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryFileIsExist);//error : 存在重名文件
		return false;
	}
	//对目标节点重命名
	target_node->SetName(dst_name);
	return true;
}


//<udpate>
//待完成：链接节点相关逻辑
//如果tokens为空，则等价于dir .
//查找目标节点
//如果目标节点是文件，则打印文件信息
//选项/ad打印子目录信息
//选项/s递归打印子目录及文件信息
//若递归，则需要统计路径下所有目录与文件
bool NodeTreeManager::DisplayDirNodeByTokensAndOptions(const std::vector<string_local>& tokens, const OptionSwitch& option_switch)	
{
	BaseNode* target_node = FindNodeByTokensInternal(tokens);
	const bool is_recursive = option_switch._s;
	//target_node保证不为空
	assert(nullptr != target_node);
	//目标节点为文件节点，打印信息
	if (!target_node->IsDirectory())
	{
		PrintFileNodeInfo(target_node);
		return true;
	}
	//打印目标目录节点子目录及文件信息
	DirNode* cur_dir_node = static_cast<DirNode*>(target_node);
	std::queue<DirNode*> q = {};
	q.push(cur_dir_node);
	StatisticInfo g_info;
	while (!q.empty())
	{
		DirNode* node = q.front();
		q.pop();
		//打印当前目录下的子目录及文件信息
		PrintDirectoryInfo(node, g_info, option_switch._ad);
		if (!is_recursive)
		{
			break;
		}
		//将子目录放在待打印目录节点队列中
		for (auto child : node->Children())
		{
			if (child->IsDirectory())
			{
				DirNode* child_dir = static_cast<DirNode*>(child);
				q.push(child_dir);
			}
		}
	}
	if (is_recursive)
	{
		Console::Write::PrintLine(TEXT("\n     所列文件总数:"));
		g_info.tot_size = cur_dir_node->GetSize();
		PrintStatisticInfo(g_info);
	}
	return true;
}


//选项/s:递归移除目录下的所有子目录与文件
//工作目录上的节点不能被删除
//若path指向非目录节点，则无法删除
ReturnType NodeTreeManager::RemoveDirByTokensAndOptions(const std::vector<string_local>& tokens, const OptionSwitch& option_switch)
{
	BaseNode* target_node = FindNodeByTokensInternal(tokens);
	assert(target_node != nullptr);
	//非目录节点无法删除
	if (!target_node->IsDirectory())
	{
		return ReturnType::DirNameIsInvalid;
	}
	//工作目录上的节点无法删除
	if (IsAncestor(target_node, m_working_node))
	{
		return ReturnType::AccessDenied;
	}
	DirNode* cur_dir = static_cast<DirNode*>(target_node);
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
	BaseNode* target_node = FindNodeByTokensInternal(dst_path_tokens);
	assert(target_node != nullptr);
	//目标路径是目录
	if (target_node->IsDirectory())
	{
		DirNode* target_dir = static_cast<DirNode*>(target_node);
		CopyFromDiskToMemoryToDirectory(file_path_vec, target_dir, option_switch);
	}
	//目标路径是文件
	else if (target_node->IsFile())
	{
		FileNode* target_file = static_cast<FileNode*>(target_node);
		CopyFromDiskToMemoryFile(file_path_vec, target_file, option_switch);
	}
	//目标路径是目录链接
	else
	{
		//update...
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
			string_local dir_path = GetPathByNode(target_dir);
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


//<update> ...
//待更新：暂未处理目标节点是文件，对文件进行追加写入的相关逻辑
ReturnType NodeTreeManager::CopyFromMemoryToMemory(const std::vector<string_local>& src_tokens, const std::vector<string_local>& dst_tokens, const OptionSwitch& option_switch)
{
	//查找源节点所在目录
	BaseNode* cur_node = nullptr;
	{
		auto src_dir_tokens = src_tokens;
		src_dir_tokens.pop_back();
		if (src_dir_tokens.empty())
		{
			cur_node = m_working_node;
		}
		else
		{
			cur_node = FindNodeByTokensInternal(src_dir_tokens);
		}
		if (cur_node == nullptr || !cur_node->IsDirectory())
		{
			return ReturnType::MemoryPathIsNotFound; //error : 源路径不存在
		}
	}
	//获取源节点
	std::vector<FileNode*> src_nodes = {};
	const auto& src_file_match_pattern = src_tokens.back();//待匹配文件名
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
		return ReturnType::MemoryPathIsNotFound;//error : 源路径不存在
	}
	//获取目标节点
	BaseNode* dst_node = FindNodeByTokensInternal(dst_tokens);
	assert(nullptr != dst_node);
	//如果目标节点是目录
	if (dst_node->IsDirectory())
	{
		CopyFromMemoryToMemoryDirectory(src_nodes, static_cast<DirNode*>(dst_node), option_switch);
	}
	//如果目标节点是文件
	else
	{
		//update 
	}
	return ReturnType::Success;
}

void NodeTreeManager::CopyFromMemoryToMemoryDirectory(const std::vector<FileNode*>& node_list, DirNode* target_dir, const OptionSwitch& option_switch)
{
	assert(nullptr != target_dir);
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
			string_local dir_path = GetPathByNode(target_dir);
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
		OverwriteFileNode(node, file_node->GetData(), file_node->GetSize());
		copy_count++;
		Console::Write::PrintLine(TEXT("复制文件 ") + GetPathByNode(file_node) + TEXT(" 成功"));
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


ReturnType NodeTreeManager::MklinkByTokensAndOptions(const std::vector<string_local>& symbol_tokens, const Path& src_path, const OptionSwitch& option_switch)
{
	assert(src_path.IsValid() && !src_path.Tokens().empty());
	const auto& symbol_name = symbol_tokens.back();
	if (!PathTools::IsTokensFormatLegal({ symbol_name }))
	{
		return ReturnType::MemoryPathNameIsIllegal; //不合法的路径名称
	}
	//获取快捷方式的所在目录
	BaseNode* target_node = nullptr;
	if (symbol_tokens.size() == 1)
	{
		target_node = m_working_node;
	}
	else
	{
		std::vector<string_local> tar_dir_tokens = symbol_tokens;
		tar_dir_tokens.pop_back();
		target_node = FindNodeByTokensInternal(tar_dir_tokens);
	}
	if (!target_node || !target_node->IsDirectory())
	{
		return ReturnType::MemoryPathIsNotFound; //找不到快捷方式所在路径
	}
	DirNode* target_dir = static_cast<DirNode*>(target_node);
	//找到源节点
	auto src_tokens = src_path.Tokens();
	auto src_node = FindNodeByTokensInternal(src_tokens);
	assert(nullptr != src_node);
	//创建快捷方式
	auto symbo_node = new SymlinkNode(symbol_name);
	NodeType link_type = (option_switch._d) ? NodeType::SymlinkD : NodeType::SymlinkF;
	symbo_node->SetTarget(link_type, src_path.ToString());
	m_tree->InsertNode(target_dir, symbo_node);
	return ReturnType::Success;
}


//1、若dst目录下有重名文件/目录，文件可以被覆盖，目录不可以——error ： 拒绝访问
//2、工作路径上的节点不允许move
//3、源路径必须存在；目标路径可以不存在，但必须与源路径在同一目录下，此时相当于重命名
ReturnType NodeTreeManager::MoveByTokensAndOptions(const std::vector<string_local>& src_tokens, const std::vector<string_local>& dst_tokens, const OptionSwitch& option_switch)
{
	bool is_silent_overwrite = option_switch._y;
	BaseNode* src_node = FindNodeByTokensInternal(src_tokens);
	assert(src_node != nullptr);
	//不能移动工作路径上的目录
	if (IsAncestor(src_node, m_working_node))
	{
		return ReturnType::AccessDenied;//error : 拒绝访问
	}
	//定位源目录/文件所在目录
	BaseNode* src_parent_node = src_node->GetParent();
	DirNode* src_parent_dir = static_cast<DirNode*>(src_parent_node);
	//目标路径不存在
	auto dst_node = FindNodeByTokensInternal(dst_tokens);
	if (dst_node == nullptr)
	{
		auto dst_parent_tokens = dst_tokens;
		dst_parent_tokens.pop_back();
		auto dst_parent_node = FindNodeByTokensInternal(dst_parent_tokens);
		if (dst_parent_node == nullptr)
		{
			return ReturnType::MemoryPathIsNotFound;//error : 找不到虚拟磁盘路径
		}
		//若目标路径与源路径在同一目录下
		if (dst_parent_node->IsDirectory() && IsSameNode(src_parent_node, dst_parent_node))
		{
			//将源路径重命名为目标路径
			assert(!dst_tokens.empty());
			src_node->SetName(dst_tokens.back());
			return ReturnType::Success;
		}
		else
		{
			return ReturnType::MemoryPathIsNotFound;//error : 找不到虚拟磁盘路径
		}
	}
	//目标路径是目录
	else if (dst_node->IsDirectory())
	{
		DirNode* dst_dir = static_cast<DirNode*>(dst_node);
		auto child = dst_dir->FindChildByName(src_node->GetName());
		if (child)
		{
			//重名目录
			if (child->IsDirectory())
			{
				return ReturnType::AccessDenied;//error : 拒绝访问
			}
			//重名文件
			else if (!is_silent_overwrite)
			{
				auto ret = Selector(TEXT("覆盖 ") + GetPathByNode(child) + TEXT(" 吗? (Yes/No/All):"));
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
	//目标路径是文件
	else
	{
		auto new_name = dst_node->GetName();
		auto new_dir = dst_node->GetParent();
		if (new_dir == m_tree->GetRoot())
		{
			return ReturnType::MemoryPathIsNotFound;//找不到虚拟磁盘路径
		}
		if (!is_silent_overwrite)
		{
			auto ret = Selector(TEXT("覆盖 ") + GetPathByNode(dst_node) + TEXT(" 吗? (Yes/No/All):"));
			if (ret == SelectType::no)
			{
				return ReturnType::Success;
			}
		}
		//删除目标文件
		if (dst_node)
		{
			m_tree->DeleteNode(dst_node);
		}
		m_tree->RemoveButNotDeleteNode(src_node);
		m_tree->InsertNode(new_dir, src_node);
		//将源文件重命名
		src_node->SetName(new_name);
	}
	//打印提示文案
	if (src_node->IsDirectory())
	{
		Console::Write::PrintLine(TEXT("移动了1个目录 ") + GetPathByNode(src_node));
	}
	else
	{
		Console::Write::PrintLine(TEXT("移动了1个文件 ") + GetPathByNode(src_node));
	}
	return ReturnType::Success;
}


ReturnType NodeTreeManager::DelByTokensAndOption(const Path& path, const OptionSwitch& option_switch)
{
	bool is_recursive = option_switch._s;
	BaseNode* target_node = FindNodeByTokensInternal(path.Tokens());
	if (nullptr != target_node)
	{
		//删除目录下的所有文件
		if (target_node->IsDirectory())
		{
			bool ret = DeleteNodeByWildcardFileName(static_cast<DirNode*>(target_node), TEXT("*"), is_recursive);
			return ReturnType::Success;
		}
		//删除与文件名匹配的文件
		auto file_name = path.Tokens().back();
		auto cur_dir = static_cast<DirNode*>(target_node->GetParent());
		bool ret = DeleteNodeByWildcardFileName(cur_dir, file_name, is_recursive);
		return ReturnType::Success;
	}
	//检查路径中是否含有通配符
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
		//基于通配符路径匹配文件并删除
		bool ret = DeleteNodeByWildcardFileName(static_cast<DirNode*>(dir_node), file_name, is_recursive);
		return ReturnType::Success;
	}
	return ReturnType::MemoryPathIsNotFound;//error : 虚拟磁盘路径不存在
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
		//如果删除全部文件，则提示用户确认
		if (StringTools::IsEqual(file_name, TEXT("*")))
		{
			SelectType ret = Selector(TEXT("删除 ") + GetPathByNode(dir) + TEXT("\\*,是否确认(Yes/No/All)？"));
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
			Console::Write::PrintLine(TEXT("删除文件 - ") + GetPathByNode(item));
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
	return StringTools::IsEqual(lstr, rstr);
}


ReturnType NodeTreeManager::SaveToPath(const string_local& path_str)
{
	assert(!PathTools::IsDiskPathExist(path_str));
	tinyxml2::XMLDocument doc;	
	auto node_root = m_tree->GetRoot();
	auto xml_root = WriteDirToXml(static_cast<DirNode*>(node_root), doc);
	xml_root->SetAttribute(XMLIdentifier::name, StringTools::UnicodeToUtf8(node_root->GetName().c_str()));
	xml_root->SetAttribute(XMLIdentifier::id, node_root->GetId());
	xml_root->SetAttribute(XMLIdentifier::type, static_cast<int>(node_root->GetType()));
	xml_root->SetAttribute(XMLIdentifier::timestamp, node_root->GetLatestModifiedTimeStamp());
	xml_root->SetAttribute(XMLIdentifier::parentId, 0);
	doc.LinkEndChild(xml_root);
	doc.SaveFile(StringTools::UnicodeToUtf8(path_str.c_str()));
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
			auto dir_node = static_cast<DirNode*>(node);
			ele_node = WriteDirToXml(dir_node, doc);
		}
		//创建文件对象
		else if (node->IsFile())
		{
			auto file_node = static_cast<FileNode*>(node);
			ele_node = doc.NewElement(XMLIdentifier::FileNode);
			ele_node->SetAttribute(XMLIdentifier::data, file_node->GetData());
			ele_node->SetAttribute(XMLIdentifier::dataSize, file_node->GetSize());
		}
		//创建链接对象
		else
		{
			auto link_node = static_cast<SymlinkNode*>(node);
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
/// \param xml_dir 待解析的xml目录节点
/// \param parent 待插入的父节点
/// \param doc 待解析的xml文档
/// \param link_elems 该列表维护节点类型为SymlinkNode的xml对象
BaseNode* NodeTreeManager::ReadXml(tinyxml2::XMLElement* xml_item, DirNode* parent)
{
	assert(xml_item);
	auto node_name = StringTools::Utf8ToUnicode(xml_item->FindAttribute(XMLIdentifier::name)->Value());
	auto node_id = xml_item->FindAttribute(XMLIdentifier::id)->Int64Value();
	auto node_type = static_cast<NodeType>(xml_item->FindAttribute(XMLIdentifier::type)->IntValue());
	auto node_timestamp = xml_item->FindAttribute(XMLIdentifier::timestamp)->Int64Value();
	//auto node_parentId = xml_item->FindAttribute(XMLIdentifier::parentId)->Int64Value();
	BaseNode* node = nullptr;
	switch (node_type)
	{
	case NodeType::Directory:
		node = new DirNode(node_name);
		break;
	case NodeType::File:
		node = new FileNode(node_name);
		static_cast<FileNode*>(node)->SetData(
			xml_item->FindAttribute(XMLIdentifier::data)->Value(),
			xml_item->FindAttribute(XMLIdentifier::dataSize)->Int64Value());
		break;
	case NodeType::SymlinkD:
	case NodeType::SymlinkF:
		node = new SymlinkNode(node_name);
		static_cast<SymlinkNode*>(node)->SetTarget(
			node_type, 
			StringTools::Utf8ToUnicode(xml_item->FindAttribute(XMLIdentifier::symlinkPath)->Value()));
		break;
	default:
		break;
	}
	assert(node);
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
		auto child = ReadXml(ele, static_cast<DirNode*>(node));
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
		auto parent_dir = static_cast<DirNode*>(parent_node);
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



