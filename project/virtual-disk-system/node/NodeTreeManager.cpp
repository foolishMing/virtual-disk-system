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
	if (m_drivens.size() != 0) //删除驱动列表里的指针
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
void NodeTreeManager::PrintDirectoryInfo(BaseNode* node)//打印目录信息
{
	assert(nullptr != node);
	assert(NodeType::Directory == node->GetType());
	if (NodeType::Directory != node->GetType())
	{
		Log::LogWarn(L"非法操作：尝试打印非目录节点的目录信息");
		return;
	}
	auto dir = static_cast<DirNode*>(node);
	int file_cnt = 0;//文件数量
	int dir_cnt = 0;//目录数量
	size_t tot_size;//总大小
	//遍历子节点，统计并打印节点信息{修改时间、节点类型、节点大小、节点名称}
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
	//打印统计信息{文件数量、目录数量、总大小}
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
//1、绝对路径的第一个token一定是盘符
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
//1、可以是绝对路径或相对路径，需要进行判断
//2、token为.或..是合法的
//3、token为..时，需要判断空指针
//4、如果待查找的尾节点是.或..应当进行特殊处理
bool NodeTreeManager::IsPathExist(const std::vector<string_local>& tokens)
{
	bool ret = FindNodeByTokens(tokens);
	return ret;
}


bool NodeTreeManager::FindNodeByTokens(const std::vector<string_local>& tokens, BaseNode* target_node)
{
	auto cur_dir = static_cast<DirNode*>((IsAbsolutePath(tokens)) ? m_tree->GetRoot() : m_working_dir);
	int tail = tokens.size();
	//搜索目录节点
	for (int index = 0; index < tail - 1; index++)
	{
		string_local cur_name = tokens[index];
		DirNode* next_dir = nullptr;
		if (Constant::gs_cur_dir_token == cur_name) continue;//.
		if (Constant::gs_parent_dir_token == cur_name)//..
		{
			BaseNode* parent = cur_dir->GetParent();
			if (nullptr == parent) return false;//父节点不存在
			next_dir = static_cast<DirNode*>(parent);
		}
		//查找后继节点
		for (BaseNode* child : cur_dir->Children())
		{
			if (child->GetName() != cur_name) continue;
			if (NodeType::Directory != child->GetType()) return false;//查找到了同名文件，无法继续完成搜索
			next_dir = static_cast<DirNode*>(child);
			break;
		}
		if (nullptr == next_dir) return false; //目标节点不存在
		//更新当前所在目录
		cur_dir = next_dir;
	}
	//处理尾节点
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
			target_node = child;//找到了目标节点，更新之
			is_find = true;
			break;
		}
	}
	return is_find;
}


//先判断能否在cur_dir下获取到以token为名的节点
//如果目标节点是文件节点，则mkdir失败，跳出循环
//在cur_dir下创建以token为名的节点
//不允许直接在根目录root下创建新节点（因为根目录下面是驱动目录，驱动应当是只读的）
bool NodeTreeManager::MkdirByTokens(const std::vector<string_local>& tokens)
{
	DirNode* cur_dir = static_cast<DirNode*>((IsAbsolutePath(tokens)) ? m_tree->GetRoot() : m_working_dir);
	std::queue<string_local, std::deque<string_local>> q(std::deque<string_local>(tokens.begin(),tokens.end()));
	while (!q.empty())
	{
		string_local token = q.front();
		q.pop();
		BaseNode* next_node = nullptr;
		//特判.
		if (Constant::gs_cur_dir_token == token) continue;
		//特判..
		if (Constant::gs_parent_dir_token == token)
		{
			next_node = cur_dir->GetParent();
			if (nullptr == next_node) return false;
		}
		//先判断待插入节点是否在cur_dir下
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
		//在cur_dir下创建以token为名的节点，并更新当前节点，继续向后创建
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


//如果tokens为空，则等价于dir .
//查询tokens所对应目录节点是否存在
//
bool NodeTreeManager::DisplayDirNodeByTokensAndOptions(const std::vector<string_local>& tokens, const OptionSwitch& option_switch)	//列出目录中的文件和子目录列表
{
	return false;
}
