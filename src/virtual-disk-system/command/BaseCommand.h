#pragma once
#include "../util/common.h"

interface IBaseCommand
{

};

class BaseCommand : IBaseCommand {
public:
	static enum CommandType {
		undefine,	//δ��������
		exit,		//��������
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
	};
};


