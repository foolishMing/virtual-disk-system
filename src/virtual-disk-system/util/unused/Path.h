#pragma once
#include "Common.h"
#include "Name.h"
#include <cassert>
#include <queue>
namespace nsUtil {
	
	interface IPath {
		//virtual void toNext(Name next) = 0;		//׷�Ӻ�׺�ڵ�
		//virtual bool isExist() = 0;				//�ж�·���Ƿ����
		virtual string_local toString() = 0;		//·��ת�ַ���
		void split(const string_local& str);			//string -> vector
		void serialize(string_local& target);	//string <- vector
	};
	
	///����·����
	class BasePath : IPath{
	public:
		static enum PathType {
			virtualPath,
			diskPath
		};
	    static bool isDiskPath(const string_local& str)		//�ж��ַ����Ƿ��ʾһ������·��
		{
			if (str.length() > 0 && m_disk_identifier_const == str[0])
			{
				return true;
			}
			return false;
		}
		explicit BasePath() {}
		explicit BasePath(const string_local& str) {}
		virtual  ~BasePath() {}
		virtual string_local toString();
	protected:
		static const char m_disk_identifier_const = '@';
		string_local       m_path_str;
		std::vector<Name> m_path_name_vec;
		void split(const string_local& str);
		void serialize(string_local& target);
	};

	///����·��
	class DiskPath : public BasePath 
	{
	public:
		explicit DiskPath(const string_local& str);
		~DiskPath();
		virtual string_local toString() override;
	};

	///�ڴ�·��
	class MemoryPath : public BasePath 
	{
	public:
		explicit MemoryPath(const string_local& str);
		~MemoryPath();
		virtual string_local toString() override;
	};


	///������
};
