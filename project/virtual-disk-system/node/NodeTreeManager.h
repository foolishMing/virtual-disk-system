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

	void PrintDirectoryNodeInfo(BaseNode* node); //��ӡĿ¼�ڵ���Ϣ
	void PrintFileNodeInfo(BaseNode* node); //��ӡ�ļ��ڵ���Ϣ
	void PrintDirectoryInfo(BaseNode* node, bool is_ad = false);	//��ӡĿ¼��Ϣ

	 //��ѯ����·���Ƿ����
	bool IsPathExist(const std::vector<string_local>& tokens);

	//�г�Ŀ¼�е��ļ�����Ŀ¼�б�
	//dir [/s] [/ad] [path1] [path2] ...
	bool DisplayDirNodeByTokensAndOptions(const std::vector<string_local>& tokens, const OptionSwitch& option_switch);	

	//����·��
	//md path [path1] ...
	bool MkdirByTokens(const std::vector<string_local>& tokens);

	//�л�·������ʾ��ǰ·��
	//cd [path]
	bool ChangeDirByTokens(const std::vector<string_local>& tokens);	

	//������
	//ren src dst
	bool RenameNodeByTokens(const std::vector<string_local>& tokens, string_local dst_name);

	//�Ƴ�Ŀ¼
	//rd [/s] path [path1] ...
	ReturnType RemoveDirByTokensAndOptions(const std::vector<string_local>& tokens, const OptionSwitch& option_switch);

	/////copy path1 path2 [/y]
	//void CopyFileFromSrcToDstWithArgs(string_local& src_path, string_local& dst_path, std::vector<string_local>& args); //�����ļ�����Ҫ֧�ִ���·����ͨ���
	//

	//void DeleteFileByPaths(std::vector<string_local>& paths); //ɾ��·��������ָ���ÿһ���ļ���/s�ݹ�ɾ��Ŀ¼����Ŀ¼������ͬ���ļ���


	/////mklink [/d] 
	//void MklinkFromSrcToSymbolWithArgs(string_local& src_path, string_local& symbol_path, std::vector<string_local>& args);//������src_pathָ��symbol_path�ķ��������ļ�

	/////save
	//void SaveTreeToDiskPath(string_local& disk_path);	//���л�������
	//
	/////load
	//void LoadTreeFromDiskPath(string_local& disk_path);	//�Ӵ��̷����л�

private:
	NodeTree* m_tree = nullptr;
	std::vector<DirNode*> m_drivens;	//�����б�(��Ŀ¼)
	DirNode* m_cur_driven = nullptr;	//��ǰ����(��Ŀ¼)
	DirNode* m_working_dir = nullptr;		//����Ŀ¼

	const std::vector<string_local> m_driver_tokens = { L"C:", L"D:", L"E:", L"F:", L"G" };
	
	//��ʼ���������̷���
	void InitDrivens();

	//�Ƿ�Ϊ����·��
	bool IsAbsolutePath(const std::vector<string_local>& tokens);
	
	//����Ŀ��ڵ�
	BaseNode* FindNodeByTokensInternal(const std::vector<string_local>& tokens);

	//��ýڵ�·��
	string_local GetPathByNode(BaseNode* node) const;

	//�ж�pre_node�ǲ���next_node������
	bool IsAncestor(BaseNode* pre_node, BaseNode* next_node);

	//�ж�token�ǲ���.
	bool IsCurrentDirToken(string_local& token);

	//�ж�token�ǲ���..
	bool IsParentDirToken(string_local& token);
};

#endif // !__NODETREEMANAGER_H__
