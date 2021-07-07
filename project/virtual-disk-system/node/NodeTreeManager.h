/*
* �ļ���������
*/
#pragma once
#ifndef __NODETREEMANAGER_H__
#define __NODETREEMANAGER_H__

#include "../util/Common.h"
#include "NodeTree.h"

/*
1����λ���tokens����·��
����Ǿ���·�����ͽ�������ʼ�ڵ�����Ϊ��Ŀ¼�ڵ�(ע����root����driven)
��������·�����ͽ�������ʼ�ڵ�����Ϊ����Ŀ¼�ڵ�
*/

class NodeTreeManager : Object {
public:
	explicit NodeTreeManager();
	~NodeTreeManager();
	virtual void Create();
	virtual void Destroy();
public:
	string_local GetCurrentPath() const; //��ȡ��ǰ(Ŀ¼)�ڵ��·��
	void PrintDirectoryInfo(BaseNode* node);	//��ӡ��ǰĿ¼����Ϣ

	 //��ѯ����·���Ƿ����
	bool IsPathExist(const std::vector<string_local>& tokens);

	/////dir [/s] [/ad] [path1] [path2] ...
	//void DisplayNodeByPathsWithArgs(std::vector<string_local>& paths, std::vector<string_local>& args);	//�г�Ŀ¼�е��ļ�����Ŀ¼�б�

	//����·��md path
	bool MkdirByPathByTokens(const std::vector<string_local>& tokens);

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
	std::vector<DirNode*> m_drivens;	//�����б�(��Ŀ¼)
	DirNode* m_cur_driven = nullptr;	//��ǰ����(��Ŀ¼)
	DirNode* m_working_dir = nullptr;		//����Ŀ¼

	std::vector<string_local> m_driver_tokens = { L"C:", L"D:", L"E:", L"F:", L"G" };
	void InitDrivers();

	//�Ƿ�Ϊ����·��
	bool IsAbsolutePath(const std::vector<string_local>& tokens);

};

#endif // !__NODETREEMANAGER_H__
