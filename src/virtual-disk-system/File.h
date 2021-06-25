#include<string>
#include<Windows.h>
#include<vector>
#include<direct.h>
#include<io.h>
#include<time.h>
const int BUFSIZE = 4096, EXIST = 0,READABLE = 2,WRITEABLE=4,EXECUTABLE=6;
struct Time {
	int year, month, day, hour, minute, second;
};
 
class File {
private:
	_finddata_t fileInfo;
	std::string absolute_path,path,name;
	void updata(std::string);  //��������
	Time getTime(time_t tm);
public:
	File(std::string pathname);
	~File();
	std::string getName();    //��ȡ�ļ���
	std::string getParent();  //��ȡ��·��
	File getParentFile();	  //��ȡ��·����һ��File����
	std::string getPath();    //��ȡ·��
	bool isAbsolute();        //�ж��Ƿ�Ϊ����·��
	std::string getAbsolutePath();   //��ȡ����·��
	File getAbsoluteFile();          //��ȡ����·����һ��File����
	Time getCreateTime();            //��ȡ��������
	Time getAccessTime();            //��ȡ����������
	Time getWriteTime();             //��ȡ����޸�����
	bool canRead();					 //�ж��Ƿ�ɶ�
	bool canWrite();                 //�ж��Ƿ��д
	bool canRun();                   //�ж��Ƿ��ִ��
	bool exists();					 //�ж��Ƿ����
	bool isDirectory();              //�ж��Ƿ�ΪĿ¼
	bool isFile();					 //�ж��Ƿ�Ϊ�ĵ�
	bool isHidden();                 //�ж��Ƿ�����
	long long length();              //��ȡ�ļ��ֽڴ�С
	bool Delete();                   //ɾ���ļ�
	std::vector<std::string> list(); //��ȡ���ļ��б�
	std::vector<File> listFile();    //��ȡ���ļ���File�����б�
	bool mkdir();                    //������Ŀ¼
	bool renameTo(std::string newName);  //������
};