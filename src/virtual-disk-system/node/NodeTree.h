#pragma once
#ifndef __NODETREE_H__
#define __NODETREE_H__

#include "BaseNode.h"
#include "../util/Common.h"

class NodeTree : Object {
public:
	explicit NodeTree();
	virtual	~NodeTree();
	virtual void create();
	virtual void destroy();
private:
	BaseNode* m_nil = nullptr;			//哨兵节点
	BaseNode* m_driver_dir = nullptr;	//驱动,根目录
	BaseNode* m_cur_dir = nullptr;		//当前目录
};
#endif // !__NODETREE_H__
