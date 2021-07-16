//<文件节点>
//-成员属性: 创建时间（时间戳）、文件数据（字符流）、文件大小（int）
#pragma once
#ifndef __FILENODE_H__
#define __FILENODE_H__

#include "BaseNode.h"

class FileNode : public BaseNode 
{
public:
	explicit FileNode(string_local name, BaseNode* parent = nullptr);
	virtual ~FileNode();

	//获取文件大小
	virtual const size_t GetSize() const; 

	//获取修改时间
	virtual const time_t GetLatestModifiedTimeStamp(); 
	//设置修改时间
	virtual void SetLatestModifiedTimeStamp(time_t ts);

	//读取文件数据
	char* GetData();
	//（覆盖）写入文件数据
	bool SetData(const char* data, const size_t size);
	
	//（追加）写入文件数据
	bool AppendData(const char* new_data, const size_t size);

private:
	//文件数据
	char* m_data = nullptr;
	
	//文件大小	
	size_t m_size = 0; 			
	
	//设置文件大小
	void SetSize(const size_t size);
	

};
#endif // !__FILENODE_H__
