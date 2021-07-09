/*
* 文件树管理类
*/
#pragma once
#ifndef __NODETREEMANAGER_H__
#define __NODETREEMANAGER_H__

#include "../util/Common.h"
#include "NodeTree.h"

/*
1、如何基于tokens搜索路径
如果是绝对路径，就将搜索起始节点设置为根目录节点(注意是root不是driven)
如果是相对路径，就将搜索起始节点设置为工作目录节点
*/

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

	/////copy path1 path2 [/y]
	//void CopyFileFromSrcToDstWithArgs(string_local& src_path, string_local& dst_path, std::vector<string_local>& args); //复制文件，需要支持磁盘路径与通配符
	//

	//void DeleteFileByPaths(std::vector<string_local>& paths); //删除路径集合所指向的每一个文件（/s递归删除目录及子目录下所有同名文件）


	/////mklink [/d] 
	//void MklinkFromSrcToSymbolWithArgs(string_local& src_path, string_local& symbol_path, std::vector<string_local>& args);//创建由src_path指向symbol_path的符号链接文件

	/////save
	//void SaveTreeToDiskPath(string_local& disk_path);	//序列化到磁盘
	//
	/////load
	//void LoadTreeFromDiskPath(string_local& disk_path);	//从磁盘反序列化

private:
	NodeTree* m_tree = nullptr;
	std::vector<DirNode*> m_drivens;	//驱动列表(根目录)
	DirNode* m_cur_driven = nullptr;	//当前驱动(根目录)
	DirNode* m_working_dir = nullptr;		//工作目录

	const std::vector<string_local> m_driver_tokens = { L"C:", L"D:", L"E:", L"F:", L"G" };
	
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
};

#endif // !__NODETREEMANAGER_H__
