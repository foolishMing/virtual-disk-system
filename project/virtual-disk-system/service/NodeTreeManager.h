/*
* 文件树管理类
*/
#pragma once
#ifndef __NODETREEMANAGER_H__
#define __NODETREEMANAGER_H__

#include "../util/Common.h"
#include "../datastruct/NodeTree.h"
#include "../datastruct/FileNode.h"
#include "../datastruct/FileNode.h"
#include "../util/tinyxml2.h"

/*
1、如何基于tokens搜索路径
如果是绝对路径，就将搜索起始节点设置为根目录节点(注意是root不是driven)
如果是相对路径，就将搜索起始节点设置为工作目录节点
*/

enum class SelectType
{
	yes,
	no,
	all
};

struct StatisticInfo
{
public:
	size_t tot_size = 0;//总字节数
	uint64_t tot_cnt = 0;//节点数量
	uint64_t dir_cnt = 0;//目录数量
	//file_cnt = tot_cnt - dir_cnt //文件数量
	bool operator += (const StatisticInfo& rhs)
	{
		tot_cnt += rhs.tot_cnt;
		tot_size += rhs.tot_size;
		dir_cnt += rhs.dir_cnt;
		return true;
	}
};

class NodeTreeManager {
public:
	explicit NodeTreeManager();
	~NodeTreeManager();
	virtual void Create();
	virtual void Destroy();
	bool Load(NodeTree* tree);
public:
	string_local GetCurrentPath() const; //获取当前(目录)节点的路径

	//列出目录中的文件和子目录列表
	//dir [/s] [/ad] [path1] [path2] ...
	ReturnType DisplayDirNodeByTokensAndOptions(const std::vector<string_local>& tokens, const OptionSwitch& option_switch);	

	//创建路径
	//md path [path1] ...
	ReturnType MkdirByTokens(const std::vector<string_local>& tokens);

	//切换路径或显示当前路径
	//cd [path]
	ReturnType ChangeDirByTokens(const std::vector<string_local>& tokens);	

	//重命名
	//ren src dst
	ReturnType RenameNodeByTokens(const std::vector<string_local>& tokens, string_local dst_name);

	//移除目录
	//rd [/s] path [path1] ...
	ReturnType RemoveDirByTokensAndOptions(const std::vector<string_local>& tokens, const OptionSwitch& option_switch);

	//从真实磁盘路径复制文件到虚拟磁盘
	//copy [/y] src_path dst_path
	ReturnType CopyFromDiskToMemory(const std::vector<string_local>& file_path_vec, const std::vector<string_local>& dst_path_tokens, const OptionSwitch& option_switch);


	//从虚拟磁盘路径复制文件到虚拟磁盘
	//copy [/y] src_path dst_path
	ReturnType CopyFromMemoryToMemory(const std::vector<string_local>& src_tokens, const std::vector<string_local>& dst_tokens, const OptionSwitch& option_switch);

	//为src创建symlink链接
	//mklink [/d] sybolPath srcPath
	ReturnType MklinkByPathAndOptions(const Path& link_path, const Path& src_path, const OptionSwitch& option_switch);


	//移动目录或文件
	//move [/y] srcPath dstPath
	ReturnType MoveByTokensAndOptions(const Path& src_path, const Path&  dst_path, const OptionSwitch& option_switch);

	//删除文件
	//del [/s] path [path1] ...
	ReturnType DelByTokensAndOption(const Path& path, const OptionSwitch& option_switch);

	//序列化
	//save @path
	ReturnType SaveToPath(const string_local& path_str);

	//反序列化
	//load @path
	ReturnType LoadFromPath(const string_local& path_str);

	//查找链接目录
	DirNode* FindSymlinkedDirectory(SymlinkNode* node);

	//查找链接文件
	FileNode* FindSymlinkedFile(SymlinkNode* node);

private:
	NodeTree* m_tree = nullptr;
	DirNode* m_cur_driven = nullptr; //当前驱动
	//DirNode* m_working_dir = nullptr; //工作目录
	const std::vector<string_local> m_driven_tokens = { TEXT("C:"), TEXT("D:"), TEXT("E:"), TEXT("F:"), TEXT("G") };
	
	std::deque<BaseNode*> m_working_stack; //工作栈

private:
	//初始化驱动（盘符）
	bool InitDrivens();

	//是否为绝对路径
	bool IsAbsolutePath(const std::vector<string_local>& tokens);

	void SetMyWorkingStack(const std::deque<BaseNode*> working_stack);
	std::deque<BaseNode*> GetMyWorkingStack() const{ return m_working_stack; }

	std::deque<BaseNode*> GetTemporaryWorkingStackByTokens(const std::vector<string_local>& tokens);

	//查找节点所在目录
	DirNode* FindWhichDirectoryIsNodeUnderByTokens(const std::vector<string_local>& tokens, std::deque<BaseNode*>& working_stack);

	//获得节点指向的实际目录
	DirNode* GetTrueDirNode(BaseNode* node);

	//获得节点指向的实际文件
	FileNode* GetTrueFileNode(BaseNode* node);

	//判断节点是不是目录或目录链接
	bool IsDirOrDirLink(BaseNode* node);


	//打印文件信息
	void PrintFileNodeInfo(BaseNode* cur_node);

	//打印目录信息
	void PrintDirectoryInfo(BaseNode* node, StatisticInfo& g_info, const OptionSwitch& option);

	//打印统计信息
	void PrintStatisticInfo(StatisticInfo& info, const OptionSwitch& option);

	
	//查找真实的目标节点
	BaseNode* FindTrueNodeByTokensInternal(const std::vector<string_local>& tokens, std::deque<BaseNode*>& working_stack);

	//获得节点路径
	string_local GetTruePathOfNode(BaseNode* node) const;

	//判断pre_node是不是next_node的祖先
	bool IsTrueAncestor(BaseNode* pre_node, BaseNode* next_node);

	//判断token是不是.
	bool IsCurrentDir(const string_local& token);

	//判断token是不是..
	bool IsParentDir(const string_local& token);

	//判断两个节点是否相同
	bool IsSameNode(BaseNode* lhs, BaseNode* rhs);

	//接收用户输入的选择器
	SelectType Selector(const string_local& str);

	//重写文件节点的数据
	void OverwriteFileNode(BaseNode* file, const char* content, const size_t& size);

	//从磁盘拷贝到目录下
	void CopyFromDiskToMemoryToDirectory(const std::vector<string_local>& file_path_vec, DirNode* target_dir, const OptionSwitch& option_switch);
	//从磁盘拷贝到文件下
	void CopyFromDiskToMemoryFile(const std::vector<string_local>& file_path_vec, FileNode* target_node, const OptionSwitch& option_switch);

	//把文件从内存目录拷贝到内存目录下
	void CopyFromMemoryToMemoryDirectory(const std::vector<BaseNode*>& node_list, DirNode* target_dir, const OptionSwitch& option_switch);

	//以名称为索引，删除文件节点
	bool DeleteNodeByWildcardFileName(DirNode* cur_dir, const string_local& file_name, bool is_recursive);

	//写xml
	tinyxml2::XMLElement* WriteDirToXml(DirNode* dir, tinyxml2::XMLDocument& doc);
	//读xml
	BaseNode* ReadXml(tinyxml2::XMLElement* xml_item, DirNode* parent);
	//从xml对象中解析符号链接节点
	bool InsertSymlinkNodeByXml(std::vector<tinyxml2::XMLElement*>& link_elems);
};

#endif // !__NODETREEMANAGER_H__
