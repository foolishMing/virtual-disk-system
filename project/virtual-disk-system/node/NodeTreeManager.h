/*
* �ļ���������
*/
#pragma once
#ifndef __NODETREEMANAGER_H__
#define __NODETREEMANAGER_H__

#include "../util/Common.h"
#include "NodeTree.h"
#include "FileNode.h"
#include "SymlinkNode.h"

/*
1����λ���tokens����·��
����Ǿ���·�����ͽ�������ʼ�ڵ�����Ϊ��Ŀ¼�ڵ�(ע����root����driven)
��������·�����ͽ�������ʼ�ڵ�����Ϊ����Ŀ¼�ڵ�
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

	//����ʵ����·�������ļ����������
	//copy [/y] src_path dst_path
	ReturnType CopyFromDiskToMemory(const std::vector<string_local>& file_path_vec, const std::vector<string_local>& dst_path_tokens, const OptionSwitch& option_switch);


	//���������·�������ļ����������
	//copy [/y] src_path dst_path
	ReturnType CopyFromMemoryToMemory(const std::vector<string_local>& src_tokens, const std::vector<string_local>& dst_tokens, const OptionSwitch& option_switch);

	//Ϊsrc����symbol����
	//mklink [/d] sybolPath srcPath
	ReturnType MklinkByTokensAndOptions(const std::vector<string_local>& symbol_tokens, const std::vector<string_local>& src_tokens, const OptionSwitch& option_switch);


	//�ƶ�Ŀ¼���ļ�
	//move [/y] srcPath dstPath
	ReturnType MoveByTokensAndOptions(const std::vector<string_local>& src_tokens, const std::vector<string_local>& dst_tokens, const OptionSwitch& option_switch);

	//ɾ���ļ�
	//del [/s] path [path1] ...
	ReturnType DelByTokensAndOption(const Path& path, const OptionSwitch& option_switch);

	//���л�
	//save @path
	ReturnType SaveToPath(const Path& path);

	//�����л�
	//load @path
	ReturnType LoadFromPath(const Path& path);


private:
	NodeTree* m_tree = nullptr;
	std::vector<DirNode*> m_drivens;	//�����б�(��Ŀ¼)
	DirNode* m_cur_driven = nullptr;	//��ǰ����(��Ŀ¼)
	DirNode* m_working_dir = nullptr;		//����Ŀ¼

	const std::vector<string_local> m_driver_tokens = { TEXT("C:"), TEXT("D:"), TEXT("E:"), TEXT("F:"), TEXT("G") };
	
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

	//�ж������ڵ��Ƿ���ͬ
	bool IsSameNode(BaseNode* lhs, BaseNode* rhs);

	//�����û������ѡ����
	SelectType Selector(const string_local& str);

	//��д�ļ��ڵ������
	void OverwriteFileNode(BaseNode* file, const char* content, const size_t& size);

	//�Ӵ��̿�����Ŀ¼��
	void CopyFromDiskToMemoryToDirectory(const std::vector<string_local>& file_path_vec, DirNode* target_dir, const OptionSwitch& option_switch);
	//�Ӵ��̿������ļ���
	void CopyFromDiskToMemoryFile(const std::vector<string_local>& file_path_vec, FileNode* target_node, const OptionSwitch& option_switch);

	//���ڴ濽����Ŀ¼��
	void CopyFromMemoryToMemoryDirectory(const std::vector<FileNode*>& node_list, DirNode* target_dir, const OptionSwitch& option_switch);

	//������Ϊ������ɾ���ļ��ڵ�
	bool DeleteNodeByWildcardFileName(DirNode* cur_dir, const string_local& file_name, bool is_recursive);


};

#endif // !__NODETREEMANAGER_H__
