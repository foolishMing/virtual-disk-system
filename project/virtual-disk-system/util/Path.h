//<·����>
//���������û����������е�·������
//һ��������·�����ɱ���Ϊ��token��ɵĵ�������
//token��ʶ·���ϵĽڵ㣬Ϊ�����ٷָ�����ص�Ԫ
#pragma once
#ifndef __PATH_H__
#define __PATH_H__
#include "define.h"

struct Path
{
	friend class CommandArg;
public:
	explicit Path(const string_local& path);
	~Path();

	//�Ƿ���ͨ���
	const bool IsWild() const;


	const bool IsValid() const { return m_is_valid; };
	const string_local ToString() const { return m_path; }
	const std::vector<string_local> Tokens() const { return m_tokens; }

private:
	bool m_is_valid = false;
	string_local m_path = L""; 
	std::vector<string_local> m_tokens = {};

	//��·�����Ϊtokens
	void Split();
	bool SplitToTokensInternal();
};

#endif // !__PATH_H__
