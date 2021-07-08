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
	if (nullptr != m_tree) //删除文件树
	{
		m_tree->Destroy();
		m_tree = nullptr;
	}
	if (!m_drivens.empty()) //删除驱动列表里的指针
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
//未完成：输出内容格式化
//未完成：将返回值类型修改为DirInfo
//is_ad == true,只打印子目录节点
void NodeTreeManager::PrintDirectoryInfo(BaseNode* dir, bool is_ad)//打印目录信息
{
	assert(nullptr != dir);
	assert(NodeType::Directory == dir->GetType());
	DirNode* cur_dir = static_cast<DirNode*>(dir);
	Console::Write::PrintLine(L"");
	Console::Write::PrintLine(GetPathByNode(cur_dir) + L" 的目录");
	Console::Write::PrintLine(L"");
	int file_cnt = 0;//文件数量
	int dir_cnt = 0;//目录数量
	size_t tot_size = 0;//总大小
	//遍历子节点，统计并打印节点信息{修改时间、节点类型、节点大小、节点名称}
	for (BaseNode* cur_node : cur_dir->Children())
	{
		tot_size += cur_node->GetSize();
		//打印子目录节点
		if (NodeType::Directory == cur_node->GetType())
		{
			dir_cnt++;
			PrintDirectoryNodeInfo(cur_node);
		}
		//打印子文件节点
		else if(!is_ad)
		{
			file_cnt++;
			PrintFileNodeInfo(cur_node);
		}
	}
	//打印统计信息{文件数量、目录数量、总大小}
	Console::Write::Print(std::to_wstring(file_cnt) + L" 个文件 ");
	Console::Write::PrintLine(std::to_wstring(tot_size) + L" 字节");
	Console::Write::PrintLine(std::to_wstring(dir_cnt) + L" 个目录");
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
		string_local cur_name = tokens[index];
		DirNode* next_dir = nullptr;
		if (Constant::gs_cur_dir_token == cur_name) continue;//.
		if (Constant::gs_parent_dir_token == cur_name)//..
		{
			BaseNode* parent = cur_dir->GetParent();
			if (nullptr == parent || m_tree->GetRoot() == parent) return nullptr;//父节点不存在
			next_dir = static_cast<DirNode*>(parent);
		}
		//查找后继节点
		for (BaseNode* child : cur_dir->Children())
		{
			if (child->GetName() != cur_name) continue;
			if (NodeType::Directory != child->GetType()) return nullptr;//查找到了同名文件，无法继续完成搜索
			next_dir = static_cast<DirNode*>(child);
			break;
		}
		if (nullptr == next_dir) return nullptr; //目标节点不存在
		//更新当前所在目录
		cur_dir = next_dir;
	}
	auto cur_name = tokens.back();
	//目标节点为当前节点.，更新之
	if (Constant::gs_cur_dir_token == cur_name)
	{
		return cur_dir;
	}
	//目标节点为父节点..，更新之
	if (Constant::gs_parent_dir_token == cur_name)
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
		if (child->GetName() == cur_name)
		{
			return child;
		}
	}
	return nullptr;
}


//先判断能否在cur_dir下获取到以token为名的节点
//如果目标节点是文件节点，则mkdir失败，跳出循环
//在cur_dir下创建以token为名的节点
//不允许直接在根目录root下创建新节点（因为根目录下面是驱动目录，驱动应当是只读的）
//需要对.和..token进行特殊处理
bool NodeTreeManager::MkdirByTokens(const std::vector<string_local>& tokens)
{
	DirNode* cur_dir = static_cast<DirNode*>((IsAbsolutePath(tokens)) ? m_tree->GetRoot() : m_working_dir);
	std::queue<string_local, std::deque<string_local>> q(std::deque<string_local>(tokens.begin(),tokens.end()));
	while (!q.empty())
	{
		string_local token = q.front();
		q.pop();
		BaseNode* next_node = nullptr;
		//当前目录
		if (Constant::gs_cur_dir_token == token) continue;
		//上级目录
		if (Constant::gs_parent_dir_token == token)
		{
			next_node = cur_dir->GetParent();
			if (nullptr == next_node) return false;
		}
		//在cur_dir下查找目标节点
		for (auto child : cur_dir->Children())
		{
			if (child->GetName() == token)
			{
				next_node = child;
				break;
			}
		}
		//若目标节点已存在
		if (nullptr != next_node)
		{
			//如果目标节点是目录节点，则更新当前节点，并向后查询
			if (NodeType::Directory == next_node->GetType())
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
	if (NodeType::Directory != target_node->GetType())
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
	assert(PathTools::IsTokensFormatLegal({ dst_name }) == true);//dst_name保证语法正确
	//查找目标节点
	BaseNode* target_node = FindNodeByTokensInternal(tokens);
	assert(nullptr != target_node);//target_node保证不为空
	//判断节点是否在当前工作路径上
	if (isAncestor(target_node, m_working_dir))
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
		if (item->GetName() == dst_name)
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
bool NodeTreeManager::DisplayDirNodeByTokensAndOptions(const std::vector<string_local>& tokens, const OptionSwitch& option_switch)	//列出目录中的文件和子目录列表
{
	BaseNode* target_node = target_node = FindNodeByTokensInternal(tokens);
	//target_node保证不为空
	assert(nullptr != target_node);
	//打印目标文件节点信息
	if (NodeType::Directory != target_node->GetType())
	{
		PrintFileNodeInfo(target_node);
		return true;
	}
	//打印目标目录节点子目录及文件信息
	DirNode* cur_dir_node = static_cast<DirNode*>(target_node);
	std::queue<DirNode*> q = {};
	q.push(cur_dir_node);
	while (!q.empty())
	{
		DirNode* node = q.front();
		q.pop();
		//打印当前目录下的子目录及文件信息
		PrintDirectoryInfo(node, option_switch._ad);
		//将子目录放在待打印目录节点队列中
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
