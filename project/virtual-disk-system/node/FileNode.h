#pragma once
#ifndef __FILENODE_H__
#define __FILENODE_H__

#include "BaseNode.h"

class FileNode : public BaseNode 
{
public:
	explicit FileNode(string_local name, BaseNode* parent = nullptr);
	virtual ~FileNode();

	virtual size_t GetSize(); //获取文件大小
	virtual const time_t GetLatestModifiedTimeStamp(); //获取修改时间

	char_local* GetData();
	bool SetData(const char_local* data, const size_t size);

private:
	char_local* m_data = nullptr;
	size_t m_size = 0; //文件大小						
	void SetSize(const size_t size);
	virtual void SetLatestModifiedTimeStamp(time_t ts); //设置修改时间
};

#endif // !__FILENODE_H__
