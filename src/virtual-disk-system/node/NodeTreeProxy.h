/*
* 对树的增删改查操作进行代理
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
