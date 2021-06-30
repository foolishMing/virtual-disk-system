#pragma once

#include <vector>
#include <string>
#include <memory>
#include <map>

enum NodeType
{
	File = 0,	//叶子节点
	Directory,	//非叶子节点
};

struct IBaseNode {
	virtual std::string getPath() = 0;	//获取节点绝对路径
	virtual void setName(const char* name) = 0;
	virtual std::string getName() = 0;
};

class Node {
public:
	Node(NodeType type, Node* parent = nullptr) {
		m_type = type;
		m_parent = parent;
	}
	virtual ~Node() {
		
	}

	bool isFile() { return m_type == NodeType::File; }

	static void destroy(std::vector<Node*>children) {
		for (int i = 0; i < children.size(); i++) {
			if (children[i]->isFile()) {
				delete children[i];
				continue;
			}
			destroy(children[i]->children);
			delete children[i];
		}
		
	}

private:
	NodeType m_type = NodeType::File;
	Node* m_parent = nullptr;
	std::vector<Node*> children;
	
};




