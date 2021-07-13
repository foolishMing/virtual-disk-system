//<�ļ��ڵ�>
//-��Ա����: ����ʱ�䣨ʱ��������ļ����ݣ��ַ��������ļ���С��int��
#pragma once
#ifndef __FILENODE_H__
#define __FILENODE_H__

#include "BaseNode.h"

class FileNode : public BaseNode 
{
public:
	explicit FileNode(string_local name, BaseNode* parent = nullptr);
	virtual ~FileNode();

	//��ȡ�ļ���С
	virtual const size_t GetSize() const; 

	//��ȡ�޸�ʱ��
	virtual const time_t GetLatestModifiedTimeStamp(); 
	//�����޸�ʱ��
	virtual void SetLatestModifiedTimeStamp(time_t ts);

	//��ȡ�ļ�����
	char* GetData();
	//�����ǣ�д���ļ�����
	bool SetData(const char* data, const size_t size);
	
	//��׷�ӣ�д���ļ�����
	bool AppendData(const char* new_data, const size_t size);

private:
	//�ļ�����
	char* m_data = nullptr;
	
	//�ļ���С	
	size_t m_size = 0; 			
	
	//�����ļ���С
	void SetSize(const size_t size);
	

};
#endif // !__FILENODE_H__
