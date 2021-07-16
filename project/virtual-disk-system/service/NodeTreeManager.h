/*
* �ļ���������
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

struct StatisticInfo
{
public:
	size_t tot_size = 0;//���ֽ���
	uint64_t tot_cnt = 0;//�ڵ�����
	uint64_t dir_cnt = 0;//Ŀ¼����
	//file_cnt = tot_cnt - dir_cnt //�ļ�����
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
	string_local GetCurrentPath() const; //��ȡ��ǰ(Ŀ¼)�ڵ��·��

	//�г�Ŀ¼�е��ļ�����Ŀ¼�б�
	//dir [/s] [/ad] [path1] [path2] ...
	ReturnType DisplayDirNodeByTokensAndOptions(const std::vector<string_local>& tokens, const OptionSwitch& option_switch);	

	//����·��
	//md path [path1] ...
	ReturnType MkdirByTokens(const std::vector<string_local>& tokens);

	//�л�·������ʾ��ǰ·��
	//cd [path]
	ReturnType ChangeDirByTokens(const std::vector<string_local>& tokens);	

	//������
	//ren src dst
	ReturnType RenameNodeByTokens(const std::vector<string_local>& tokens, string_local dst_name);

	//�Ƴ�Ŀ¼
	//rd [/s] path [path1] ...
	ReturnType RemoveDirByTokensAndOptions(const std::vector<string_local>& tokens, const OptionSwitch& option_switch);

	//����ʵ����·�������ļ����������
	//copy [/y] src_path dst_path
	ReturnType CopyFromDiskToMemory(const std::vector<string_local>& file_path_vec, const std::vector<string_local>& dst_path_tokens, const OptionSwitch& option_switch);


	//���������·�������ļ����������
	//copy [/y] src_path dst_path
	ReturnType CopyFromMemoryToMemory(const std::vector<string_local>& src_tokens, const std::vector<string_local>& dst_tokens, const OptionSwitch& option_switch);

	//Ϊsrc����symlink����
	//mklink [/d] sybolPath srcPath
	ReturnType MklinkByPathAndOptions(const Path& link_path, const Path& src_path, const OptionSwitch& option_switch);


	//�ƶ�Ŀ¼���ļ�
	//move [/y] srcPath dstPath
	ReturnType MoveByTokensAndOptions(const Path& src_path, const Path&  dst_path, const OptionSwitch& option_switch);

	//ɾ���ļ�
	//del [/s] path [path1] ...
	ReturnType DelByTokensAndOption(const Path& path, const OptionSwitch& option_switch);

	//���л�
	//save @path
	ReturnType SaveToPath(const string_local& path_str);

	//�����л�
	//load @path
	ReturnType LoadFromPath(const string_local& path_str);

	//��������Ŀ¼
	DirNode* FindSymlinkedDirectory(SymlinkNode* node);

	//���������ļ�
	FileNode* FindSymlinkedFile(SymlinkNode* node);

private:
	NodeTree* m_tree = nullptr;
	DirNode* m_cur_driven = nullptr; //��ǰ����
	//DirNode* m_working_dir = nullptr; //����Ŀ¼
	const std::vector<string_local> m_driven_tokens = { TEXT("C:"), TEXT("D:"), TEXT("E:"), TEXT("F:"), TEXT("G") };
	
	std::deque<BaseNode*> m_working_stack; //����ջ

private:
	//��ʼ���������̷���
	bool InitDrivens();

	//�Ƿ�Ϊ����·��
	bool IsAbsolutePath(const std::vector<string_local>& tokens);

	void SetMyWorkingStack(const std::deque<BaseNode*> working_stack);
	std::deque<BaseNode*> GetMyWorkingStack() const{ return m_working_stack; }

	std::deque<BaseNode*> GetTemporaryWorkingStackByTokens(const std::vector<string_local>& tokens);

	//���ҽڵ�����Ŀ¼
	DirNode* FindWhichDirectoryIsNodeUnderByTokens(const std::vector<string_local>& tokens, std::deque<BaseNode*>& working_stack);

	//��ýڵ�ָ���ʵ��Ŀ¼
	DirNode* GetTrueDirNode(BaseNode* node);

	//��ýڵ�ָ���ʵ���ļ�
	FileNode* GetTrueFileNode(BaseNode* node);

	//�жϽڵ��ǲ���Ŀ¼��Ŀ¼����
	bool IsDirOrDirLink(BaseNode* node);


	//��ӡ�ļ���Ϣ
	void PrintFileNodeInfo(BaseNode* cur_node);

	//��ӡĿ¼��Ϣ
	void PrintDirectoryInfo(BaseNode* node, StatisticInfo& g_info, const OptionSwitch& option);

	//��ӡͳ����Ϣ
	void PrintStatisticInfo(StatisticInfo& info, const OptionSwitch& option);

	
	//������ʵ��Ŀ��ڵ�
	BaseNode* FindTrueNodeByTokensInternal(const std::vector<string_local>& tokens, std::deque<BaseNode*>& working_stack);

	//��ýڵ�·��
	string_local GetTruePathOfNode(BaseNode* node) const;

	//�ж�pre_node�ǲ���next_node������
	bool IsTrueAncestor(BaseNode* pre_node, BaseNode* next_node);

	//�ж�token�ǲ���.
	bool IsCurrentDir(const string_local& token);

	//�ж�token�ǲ���..
	bool IsParentDir(const string_local& token);

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

	//���ļ����ڴ�Ŀ¼�������ڴ�Ŀ¼��
	void CopyFromMemoryToMemoryDirectory(const std::vector<BaseNode*>& node_list, DirNode* target_dir, const OptionSwitch& option_switch);

	//������Ϊ������ɾ���ļ��ڵ�
	bool DeleteNodeByWildcardFileName(DirNode* cur_dir, const string_local& file_name, bool is_recursive);

	//дxml
	tinyxml2::XMLElement* WriteDirToXml(DirNode* dir, tinyxml2::XMLDocument& doc);
	//��xml
	BaseNode* ReadXml(tinyxml2::XMLElement* xml_item, DirNode* parent);
	//��xml�����н����������ӽڵ�
	bool InsertSymlinkNodeByXml(std::vector<tinyxml2::XMLElement*>& link_elems);
};

#endif // !__NODETREEMANAGER_H__
