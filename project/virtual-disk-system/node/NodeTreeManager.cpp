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
	if (nullptr != m_tree) //删除文件树
	{
		m_tree->Destroy();
		m_tree = nullptr;
	}
	if (!m_drivens.empty()) //删除驱动列表里的指针
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
	//打印子文件节点信息
	if (cur_node->IsFile())
	{
		auto file_node = static_cast<FileNode*>(cur_node);
		std::wcout << StringTools::TimeStampToDateTimeString(file_node->GetLatestModifiedTimeStamp())
			<< TEXT("    ")
			<< std::right << std::setw(14) << std::to_wstring(file_node->GetSize())
			<< TEXT(" ") << file_node->GetName() << std::endl;
	}
	//打印链接节点信息
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
//1、可以是绝对路径或相对路径，需要进行判断
//2、token为.或..是合法的
//3、token为..时，需要判断空指针
//4、如果待查找的尾节点是.或..应当进行特殊处理
//5、若回溯，不允许从驱动节点回溯至根节点
BaseNode* NodeTreeManager::FindNodeByTokensInternal(const std::vector<string_local>& tokens)
{
	BaseNode* target_node = nullptr;
	auto cur_dir = static_cast<DirNode*>((IsAbsolutePath(tokens)) ? m_tree->GetRoot() : m_working_dir);
	const int tail = static_cast<int>(tokens.size());
	//搜索目录节点
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
			if (nullptr == parent || m_tree->GetRoot() == parent) return nullptr;//父节点不存在
			next_dir = static_cast<DirNode*>(parent);
		}
		//查找后继节点
		for (BaseNode* child : cur_dir->Children())
		{
			if (child->GetName() != token) continue;
			if (!child->IsDirectory()) // 目标节点不是目录，无法继续搜索
			{
				return nullptr;
			}
			next_dir = static_cast<DirNode*>(child);
			break;
		}
		if (nullptr == next_dir) return nullptr; //目标节点不存在
		//更新当前所在目录
		cur_dir = next_dir;
	}
	auto last_token = tokens.back();
	//目标节点为当前节点.，更新之
	if (IsCurrentDirToken(last_token))
	{
		return cur_dir;
	}
	//目标节点为父节点..，更新之
	if (IsParentDirToken(last_token))
	{
		auto parent = cur_dir->GetParent();
		if (nullptr == parent || m_tree->GetRoot() == parent)
		{
			return nullptr;
		}
		return parent;
	}
	//目标节点为子节点，更新之
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
//待完成：若路径创建失败，则需要删除本次操作新建的所有节点
//先判断能否在cur_dir下获取到以token为名的节点
//如果目标节点是文件节点，则mkdir失败，跳出循环
//在cur_dir下创建以token为名的节点
//不允许直接在根目录root下创建新节点（因为根目录下面是驱动目录，驱动应当是只读的）
//需要对.和..token进行特殊处理
//创建节点时，需要判断token是否合法
bool NodeTreeManager::MkdirByTokens(const std::vector<string_local>& tokens)
{
	DirNode* cur_dir = static_cast<DirNode*>((IsAbsolutePath(tokens)) ? m_tree->GetRoot() : m_working_dir);
	std::queue<string_local, std::deque<string_local>> q(std::deque<string_local>(tokens.begin(),tokens.end()));
	BaseNode* temp_node_root = nullptr;//本次操作创建的第一个(临时)节点
	while (!q.empty())
	{
		string_local token = q.front();
		q.pop();
		BaseNode* next_node = nullptr;
		//当前目录
		if (IsCurrentDirToken(token))
		{
			continue;
		}
		//上级目录
		if (IsParentDirToken(token))
		{
			next_node = cur_dir->GetParent();
			if (nullptr == next_node) return false;
		}
		//在cur_dir下查找目标节点
		for (auto child : cur_dir->Children())
		{
			if(child->IsNameEqualsTo(token))
			{
				next_node = child;
				break;
			}
		}
		//若目标节点已存在
		if (nullptr != next_node)
		{
			//如果目标节点是目录节点，则更新当前节点，并向后查询
			if (next_node->IsDirectory())
			{
				cur_dir = static_cast<DirNode*>(next_node);
				continue;
			}
			//如果目标节点是文件节点，则mkdir失败，跳出循环
			else
			{
				return false;
			}
		}
		//在cur_dir下插入目标节点，并更新cur_dir
		else
		{
			if (cur_dir == m_tree->GetRoot()) //不允许在根目录下创建新的节点
			{
				return false;
			}
			if (!PathTools::IsTokensFormatLegal({ token }))
			{
				Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//文件、目录或卷名称语法错误
				//创建失败，删除本次操作新建的所有节点
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
	//显示工作目录
	if (tokens.empty())
	{
		Console::Write::PrintLine(GetCurrentPath());
		return true;
	}
	//寻找目标节点
	BaseNode* target_node = FindNodeByTokensInternal(tokens);
	assert(nullptr != target_node);
	//目标节点非目录节点
	if (!target_node->IsDirectory())
	{
		return false;
	}
	//切换路径
	m_working_dir = static_cast<DirNode*>(target_node);
	return true;
}


//不允许对工作目录上的节点（包括根节点和驱动节点）重命名
//判断dst_name是否重名
bool NodeTreeManager::RenameNodeByTokens(const std::vector<string_local>& tokens, string_local dst_name)
{
	assert(PathTools::IsTokensFormatLegal({ dst_name }));//dst_name保证语法正确
	//查找目标节点
	BaseNode* target_node = FindNodeByTokensInternal(tokens);
	assert(nullptr != target_node);//target_node保证不为空
	//判断节点是否在当前工作路径上
	if (IsAncestor(target_node, m_working_dir))
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryPathAccessDenied);//error : 非法访问
		return false;
	}
	//查找目标节点所属目录下的重名节点
	DirNode* parent_dir = static_cast<DirNode*>(target_node->GetParent());
	for (const auto item : parent_dir->Children())
	{
		//跳过目标节点
		if (item == target_node) continue;
		//判断是否存在同名节点
		if(item->IsNameEqualsTo(dst_name))
		{
			Console::Write::PrintLine(ErrorTips::gsMemoryFileIsExist);//error : 存在重名文件
			return false;
		}
	}
	target_node->SetName(dst_name);
	return true;
}


//<udpate>
//未完成：统计递归打印的总目录数及总文件数
//如果tokens为空，则等价于dir .
//查找目标节点
//如果目标节点是文件，则打印文件信息
//选项/ad打印子目录信息
//选项/s递归打印子目录及文件信息
bool NodeTreeManager::DisplayDirNodeByTokensAndOptions(const std::vector<string_local>& tokens, const OptionSwitch& option_switch)	
{
	BaseNode* target_node = target_node = FindNodeByTokensInternal(tokens);
	bool is_recursive = option_switch._s;
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
		//将子目录放在待打印目录节点队列中
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
	if (IsAncestor(target_node, m_working_dir))
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
	//目标路径是链接
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
		char* buffer = ReadDiskFileData(StringTools::WStringToString(file_path).c_str(), file_size);
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
		copy_count++;
		Console::Write::PrintLine(TEXT("复制文件 ") + file_path + TEXT(" 成功"));
	}
	Console::Write::PrintLine(TEXT("已复制 ") + StringTools::FormatFromNumber(copy_count) + TEXT(" 个文件"));
}


void NodeTreeManager::CopyFromDiskToMemoryFile(const std::vector<string_local>& file_path_vec, FileNode* dst_node, const OptionSwitch& option_switch)
{
	//update ...
}


ReturnType NodeTreeManager::CopyFromMemoryToMemory(const std::vector<string_local>& src_tokens, const std::vector<string_local>& dst_tokens, const OptionSwitch& option_switch)
{
	//查找源节点所在目录
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
		//静默覆盖
		else if (is_silent_overwrite_all)
		{

		}
		//提示用户是否覆盖目标文件
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


ReturnType NodeTreeManager::MklinkByTokensAndOptions(const std::vector<string_local>& symbol_tokens, const std::vector<string_local>& src_tokens, const OptionSwitch& option_switch)
{
	const auto& symbol_name = symbol_tokens.back();
	if (!PathTools::IsTokensFormatLegal({ symbol_name }))
	{
		return ReturnType::MemoryPathNameIsIllegal; //不合法的路径名称
	}
	//获取快捷方式的所在目录
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
		return ReturnType::MemoryPathIsNotFound; //找不到快捷方式所在路径
	}
	DirNode* target_dir = static_cast<DirNode*>(target_node);
	//找到源节点
	auto src_node = FindNodeByTokensInternal(src_tokens);
	assert(nullptr != src_node);
	//创建快捷方式
	auto symbo_node = new SymlinkNode(symbol_name);
	NodeType link_type = (option_switch._d) ? NodeType::SymlinkD : NodeType::SymlinkF;
	symbo_node->SetSymlinkNode(link_type, src_node);
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
	if (IsAncestor(src_node, m_working_dir))
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
		//提示用户选择是否删除
		SelectType ret = Selector(TEXT("删除 ") + GetPathByNode(dir) + TEXT("\\*,是否确认(Yes/No/All)？"));
		if (ret == SelectType::no)
		{
			continue;
		}
		//删除当前目录下的所有匹配的文件
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


