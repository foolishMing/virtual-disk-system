#pragma once
#ifndef __FILENODE_H__
#define __FILENODE_H__

#include "BaseNode.h"

class FileNode : public BaseNode 
{
public:
	explicit FileNode(string_local& name, BaseNode* parent = nullptr);
	virtual ~FileNode();

	string_local GetDataByCopy();
	void SetDataByCopy(const string_local str);
	virtual uint64_t GetSize();

private:
	string_local m_file_data;
};

#endif // !__FILENODE_H__
