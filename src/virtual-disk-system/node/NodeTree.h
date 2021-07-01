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
	BaseNode* m_nil = nullptr;			//�ڱ��ڵ�
	BaseNode* m_driver_dir = nullptr;	//����,��Ŀ¼
	BaseNode* m_cur_dir = nullptr;		//��ǰĿ¼
};
#endif // !__NODETREE_H__
