#pragma once
#include "BaseCommand.h"
#include "CdCommand.h"
#include "CopyCommand.h"
#include "DelCommand.h"
#include "DirCommand.h"
#include "LoadCommand.h"
#include "MdCommand.h"
#include "MklinkCommand.h"
#include "MoveCommand.h"
#include "RdCommand.h"
#include "RenCommand.h"
#include "SaveCommand.h"
#include "ClsCommand.h"
#include "UndefinedCommand.h"

enum CommandType {
	undefine = 0,	//δ��������
	quit,		//�˳�����
	dir,		//�г���ǰĿ¼�µ�Ŀ¼���ļ�
	md,			//����Ŀ¼
	rd,			//ɾ��Ŀ¼
	cd,			//�л���ǰĿ¼
	del,		//ɾ���ļ�
	copy,		//�����ļ�
	ren,		//������
	move,		//�ƶ�
	mklink,		//������������
	save,		//���л�
	load,		//�����л�
	cls,		//����
	tail,		//����������ʵ������
};

class CommandFactory : Object{
public:
	explicit CommandFactory();
	~CommandFactory();
	void Create();
	void Destroy();
	BaseCommand* GetCommandInstance(CommandType type);	//��ȡ����ʵ��
	CommandType GetCommandTypeByToken(string_local& token);	//��ȡ��������


private:
	std::map<CommandType, BaseCommand*> m_cmd_instance_map;
	std::map<string_local, CommandType> m_cmd_token_map;
	
	BaseCommand* CreateCommandInstance(CommandType type);
};
