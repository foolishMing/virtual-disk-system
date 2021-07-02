#pragma once
#ifndef __BASECOMMAND_H__
#define __BASECOMMAND_H__

#include "../util/common.h"
#include "../node/NodeTreeManager.h"

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
	cls,		//����
	tail,		//����������ʵ������
};

interface IBaseCommand
{
	virtual void Handle() = 0;
};

class BaseCommand : IBaseCommand, Object{
public:
	explicit BaseCommand();
	~BaseCommand();
	virtual void Handle();
	virtual void Handle(std::vector<string_local>& args, NodeTreeManager& proxy);
	virtual void Create();
	virtual void Destroy();
private:
	CommandType	m_type;
};

#endif // !__BASECOMMAND_H__

