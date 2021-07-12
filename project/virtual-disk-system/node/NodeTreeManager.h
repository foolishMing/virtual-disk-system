/*
* 文件树管理类
*/
#pragma once
#ifndef __NODETREEMANAGER_H__
#define __NODETREEMANAGER_H__

#include "../util/Common.h"
#include "NodeTree.h"
#include "FileNode.h"
#include "SymlinkNode.h"

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

class NodeTreeManager : Object {
public:
	explicit NodeTreeManager();
	~NodeTreeManager();
	virtual void Create();
	virtual void Destroy();
public:
	string_local GetCurrentPath() const; //获取当前(目录)节点的路径

	void PrintDirectoryNodeInfo(BaseNode* node); //打印目录节点信息
	void PrintFileNodeInfo(BaseNode* node); //打印文件节点信息
	void PrintDirectoryInfo(BaseNode* node, bool is_ad = false);	//打印目录信息

	 //查询输入路径是否存在
	bool IsPathExist(const std::vector<string_local>& tokens);

	//列出目录中的文件和子目录列表
	//dir [/s] [/ad] [path1] [path2] ...
	bool DisplayDirNodeByTokensAndOptions(const std::vector<string_local>& tokens, const OptionSwitch& option_switch);	

	//创建路径
	//md path [path1] ...
	bool MkdirByTokens(const std::vector<string_local>& tokens);

	//切换路径或显示当前路径
	//cd [path]
	bool ChangeDirByTokens(const std::vector<string_local>& tokens);	

	//重命名
	//ren src dst
	bool RenameNodeByTokens(const std::vector<string_local>& tokens, string_local dst_name);

	//移除目录
	//rd [/s] path [path1] ...
	ReturnType RemoveDirByTokensAndOptions(const std::vector<string_local>& tokens, const OptionSwitch& option_switch);

	//从真实磁盘路径复制文件到虚拟磁盘
	//copy [/y] src_path dst_path
	ReturnType CopyFromDiskToMemory(const std::vector<string_local>& file_path_vec, const std::vector<string_local>& dst_path_tokens, const OptionSwitch& option_switch);


	//从虚拟磁盘路径复制文件到虚拟磁盘
	//copy [/y] src_path dst_path
	ReturnType CopyFromMemoryToMemory(const std::vector<string_local>& src_tokens, const std::vector<string_local>& dst_tokens, const OptionSwitch& option_switch);

	//为src创建symbol链接
	//mklink [/d] sybolPath srcPath
	ReturnType MklinkByTokensAndOptions(const std::vector<string_local>& symbol_tokens, const std::vector<string_local>& src_tokens, const OptionSwitch& option_switch);


	//移动目录或文件
	//move [/y] srcPath dstPath
	ReturnType MoveByTokensAndOptions(const std::vector<string_local>& src_tokens, const std::vector<string_local>& dst_tokens, const OptionSwitch& option_switch);

	//删除文件
	//del [/s] path [path1] ...
	ReturnType DelByTokensAndOption(const Path& path, const OptionSwitch& option_switch);

	//序列化
	//save @path
	ReturnType SaveToPath(const Path& path);

	//反序列化
	//load @path
	ReturnType LoadFromPath(const Path& path);


private:
	NodeTree* m_tree = nullptr;
	std::vector<DirNode*> m_drivens;	//驱动列表(根目录)
	DirNode* m_cur_driven = nullptr;	//当前驱动(根目录)
	DirNode* m_working_dir = nullptr;		//工作目录

	const std::vector<string_local> m_driver_tokens = { TEXT("C:"), TEXT("D:"), TEXT("E:"), TEXT("F:"), TEXT("G") };
	
	//初始化驱动（盘符）
	void InitDrivens();

	//是否为绝对路径
	bool IsAbsolutePath(const std::vector<string_local>& tokens);
	
	//查找目标节点
	BaseNode* FindNodeByTokensInternal(const std::vector<string_local>& tokens);

	//获得节点路径
	string_local GetPathByNode(BaseNode* node) const;

	//判断pre_node是不是next_node的祖先
	bool IsAncestor(BaseNode* pre_node, BaseNode* next_node);

	//判断token是不是.
	bool IsCurrentDirToken(string_local& token);

	//判断token是不是..
	bool IsParentDirToken(string_local& token);

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

	//从内存拷贝到目录下
	void CopyFromMemoryToMemoryDirectory(const std::vector<FileNode*>& node_list, DirNode* target_dir, const OptionSwitch& option_switch);

	//以名称为索引，删除文件节点
	bool DeleteNodeByWildcardFileName(DirNode* cur_dir, const string_local& file_name, bool is_recursive);


};

#endif // !__NODETREEMANAGER_H__
