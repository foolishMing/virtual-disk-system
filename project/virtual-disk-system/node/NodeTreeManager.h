/*
* 文件树管理类
*/
#pragma once
#ifndef __NODETREEMANAGER_H__
#define __NODETREEMANAGER_H__

#include "../util/Common.h"
#include "NodeTree.h"

class NodeTreeManager : Object {
public:
	explicit NodeTreeManager();
	~NodeTreeManager();
	virtual void Create();
	virtual void Destroy();
public:
	string_local GetCurrentPath() const; //获取当前(目录)节点的路径
	void PrintDirectoryInfo(BaseNode* node);	//打印当前目录的信息

	/////dir [/s] [/ad] [path1] [path2] ...
	//void DisplayNodeByPathsWithArgs(std::vector<string_local>& paths, std::vector<string_local>& args);	//列出目录中的文件和子目录列表

	/////md path
	//void MkdirByPathWithArgs(string_local& memory_path, std::vector<string_local>& args);	//创建一个路径
	//
	/////cd [path]
	//void ChangeDirToPathWithArgs(string_local& memory_path, std::vector<string_local>& args);	//切换路径或显示当前路径

	/////copy path1 path2 [/y]
	//void CopyFileFromSrcToDstWithArgs(string_local& src_path, string_local& dst_path, std::vector<string_local>& args); //复制文件，需要支持磁盘路径与通配符
	//

	//void DeleteFileByPaths(std::vector<string_local>& paths); //删除路径集合所指向的每一个文件（/s递归删除目录及子目录下所有同名文件）

	/////rd [/s] path [path1] ...
	//void RemoveDirByPaths(std::vector<string_local>& paths, std::vector<string_local>& args);	//删除路径集合所指向的每一个空目录（/s递归删除非空目录）

	/////ren src dst
	//void RenameNodeByPathWithArgs(string_local& stc_path, string_local& dst_path, std::vector<string_local>& args);	//对节点进行改名

	/////mklink [/d] 
	//void MklinkFromSrcToSymbolWithArgs(string_local& src_path, string_local& symbol_path, std::vector<string_local>& args);//创建由src_path指向symbol_path的符号链接文件

	/////save
	//void SaveTreeToDiskPath(string_local& disk_path);	//序列化到磁盘
	//
	/////load
	//void LoadTreeFromDiskPath(string_local& disk_path);	//从磁盘反序列化

private:
	//以下方法放在外层的管理类上
	//void RenameNode(BaseNode* node, string_local& new_name); //修改节点名字
	//string_local GetAbsolutePathOfCurrentNode(); //获取当前节点的绝对路径
	//string_local GetAbsolutePathOfNode(BaseNode* node);	//获取目标节点的绝对路径
	//void CreateNodeByRelativePath(NodeType& type, string_local& relative_path);	//根据相对路径创建节点
	//BaseNode* FindNodeByRelativePath(string_local& relative_path, BaseNode* target_node = nullptr); //根据相对路径查找节点
	//BaseNode* FindNodeByAbsolutePath(string_local& absolute_path, BaseNode* target_node = nullptr); //根据绝对路径查找节点

private:
	NodeTree* m_tree = nullptr;
	std::vector<BaseNode*> m_drivens;	//驱动列表(根目录)
	BaseNode* m_cur_driven = nullptr;	//当前驱动(根目录)
	BaseNode* m_working_dir = nullptr;		//工作目录

	std::vector<string_local> m_driver_name_vec = { L"C:", L"D:", L"E:", L"F:", L"G" };
	void InitDrivers();
};

#endif // !__NODETREEMANAGER_H__
