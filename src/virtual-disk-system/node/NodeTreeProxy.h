/*
* ��������ɾ�Ĳ�������д���
*/
#pragma once
#include "../util/Common.h"
#include "NodeTree.h"

class NodeTreeProxy : Object{
public:
	explicit NodeTreeProxy();
	~NodeTreeProxy();
	virtual void create();
	virtual void destroy();
};
