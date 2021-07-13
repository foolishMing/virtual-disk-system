//<路径类>
//用来管理用户输入命令中的路径参数
//一个完整的路径，可被视为由token组成的单向链表
//token标识路径上的节点，为不可再分割的语素单元
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

	//是否含有通配符
	const bool IsWild() const;


	const bool IsValid() const { return m_is_valid; };
	const string_local ToString() const { return m_path; }
	const std::vector<string_local> Tokens() const { return m_tokens; }

private:
	bool m_is_valid = false;
	string_local m_path = L""; 
	std::vector<string_local> m_tokens = {};

	//将路径拆分为tokens
	void Split();
	bool SplitToTokensInternal();
};

#endif // !__PATH_H__
