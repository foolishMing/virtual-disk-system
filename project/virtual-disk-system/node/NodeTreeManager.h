/*
* �ļ���������
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
	string_local GetCurrentPath() const; //��ȡ��ǰ(Ŀ¼)�ڵ��·��
	void PrintDirectoryInfo(BaseNode* node);	//��ӡ��ǰĿ¼����Ϣ

	/////dir [/s] [/ad] [path1] [path2] ...
	//void DisplayNodeByPathsWithArgs(std::vector<string_local>& paths, std::vector<string_local>& args);	//�г�Ŀ¼�е��ļ�����Ŀ¼�б�

	/////md path
	//void MkdirByPathWithArgs(string_local& memory_path, std::vector<string_local>& args);	//����һ��·��
	//
	/////cd [path]
	//void ChangeDirToPathWithArgs(string_local& memory_path, std::vector<string_local>& args);	//�л�·������ʾ��ǰ·��

	/////copy path1 path2 [/y]
	//void CopyFileFromSrcToDstWithArgs(string_local& src_path, string_local& dst_path, std::vector<string_local>& args); //�����ļ�����Ҫ֧�ִ���·����ͨ���
	//

	//void DeleteFileByPaths(std::vector<string_local>& paths); //ɾ��·��������ָ���ÿһ���ļ���/s�ݹ�ɾ��Ŀ¼����Ŀ¼������ͬ���ļ���

	/////rd [/s] path [path1] ...
	//void RemoveDirByPaths(std::vector<string_local>& paths, std::vector<string_local>& args);	//ɾ��·��������ָ���ÿһ����Ŀ¼��/s�ݹ�ɾ���ǿ�Ŀ¼��

	/////ren src dst
	//void RenameNodeByPathWithArgs(string_local& stc_path, string_local& dst_path, std::vector<string_local>& args);	//�Խڵ���и���

	/////mklink [/d] 
	//void MklinkFromSrcToSymbolWithArgs(string_local& src_path, string_local& symbol_path, std::vector<string_local>& args);//������src_pathָ��symbol_path�ķ��������ļ�

	/////save
	//void SaveTreeToDiskPath(string_local& disk_path);	//���л�������
	//
	/////load
	//void LoadTreeFromDiskPath(string_local& disk_path);	//�Ӵ��̷����л�

private:
	//���·����������Ĺ�������
	//void RenameNode(BaseNode* node, string_local& new_name); //�޸Ľڵ�����
	//string_local GetAbsolutePathOfCurrentNode(); //��ȡ��ǰ�ڵ�ľ���·��
	//string_local GetAbsolutePathOfNode(BaseNode* node);	//��ȡĿ��ڵ�ľ���·��
	//void CreateNodeByRelativePath(NodeType& type, string_local& relative_path);	//�������·�������ڵ�
	//BaseNode* FindNodeByRelativePath(string_local& relative_path, BaseNode* target_node = nullptr); //�������·�����ҽڵ�
	//BaseNode* FindNodeByAbsolutePath(string_local& absolute_path, BaseNode* target_node = nullptr); //���ݾ���·�����ҽڵ�

private:
	NodeTree* m_tree = nullptr;
	std::vector<BaseNode*> m_drivens;	//�����б�(��Ŀ¼)
	BaseNode* m_cur_driven = nullptr;	//��ǰ����(��Ŀ¼)
	BaseNode* m_working_dir = nullptr;		//����Ŀ¼

	std::vector<string_local> m_driver_name_vec = { L"C:", L"D:", L"E:", L"F:", L"G" };
	void InitDrivers();
};

#endif // !__NODETREEMANAGER_H__
