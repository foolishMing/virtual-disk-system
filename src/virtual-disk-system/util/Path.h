#pragma once
#include "Common.h"
#include "Name.h"
#include <cassert>
#include <queue>
namespace nsUtil {
	
	interface IPath {
		//virtual void toNext(Name next) = 0;		//׷�Ӻ�׺�ڵ�
		//virtual bool isExist() = 0;				//�ж�·���Ƿ����
		virtual std::wstring toString() = 0;		//·��ת�ַ���
		void split(const std::wstring& str);			//string -> vector
		void serialize(std::wstring& target);	//string <- vector
	};
	
	///����·����
	class BasePath : IPath{
	public:
		static enum PathType {
			virtualPath,
			diskPath
		};
	    static bool isDiskPath(const std::wstring& str)		//�ж��ַ����Ƿ��ʾһ������·��
		{
			if (str.length() > 0 && m_disk_identifier_const == str[0])
			{
				return true;
			}
			return false;
		}
		explicit BasePath() {}
		explicit BasePath(const std::wstring& str) {}
		virtual  ~BasePath() {}
		virtual std::wstring toString();
	protected:
		static const char m_disk_identifier_const = '@';
		std::wstring       m_path_str;
		std::vector<Name> m_path_name_vec;
		void split(const std::wstring& str);
		void serialize(std::wstring& target);
	};

	///����·��
	class DiskPath : public BasePath 
	{
	public:
		explicit DiskPath(const std::wstring& str);
		~DiskPath();
		virtual std::wstring toString() override;
	};

	///�ڴ�·��
	class MemoryPath : public BasePath 
	{
	public:
		explicit MemoryPath(const std::wstring& str);
		~MemoryPath();
		virtual std::wstring toString() override;
	};


	///������
};
