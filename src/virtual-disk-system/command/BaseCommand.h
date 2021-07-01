#pragma once
#ifndef __BASECOMMAND_H__
#define __BASECOMMAND_H__

#include "../util/common.h"
#include "../node/NodeTreeProxy.h"

enum CommandType {
	undefine = 0,	//δ��������
	quit,		//��������
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
	tail,		//����������ʵ������
};

interface IBaseCommand
{
	virtual void handle() = 0;
	virtual void destroy() = 0;
};

class BaseCommand : IBaseCommand {
public:
	explicit BaseCommand();
	~BaseCommand();
	virtual void handle();
	virtual void handle(std::vector<string_local>& args, NodeTreeProxy& proxy);
	virtual void destroy();
private:
	CommandType	m_type;
	std::vector<string_local> m_args;
};

#endif // !__BASECOMMAND_H__

