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
	void updata(std::string);  //更新数据
	Time getTime(time_t tm);
public:
	File(std::string pathname);
	~File();
	std::string getName();    //获取文件名
	std::string getParent();  //获取父路径
	File getParentFile();	  //获取父路径的一个File对象
	std::string getPath();    //获取路径
	bool isAbsolute();        //判断是否为绝对路径
	std::string getAbsolutePath();   //获取绝对路径
	File getAbsoluteFile();          //获取绝对路径的一个File对象
	Time getCreateTime();            //获取创建日期
	Time getAccessTime();            //获取最后访问日期
	Time getWriteTime();             //获取最后修改日期
	bool canRead();					 //判断是否可读
	bool canWrite();                 //判断是否可写
	bool canRun();                   //判断是否可执行
	bool exists();					 //判断是否存在
	bool isDirectory();              //判断是否为目录
	bool isFile();					 //判断是否为文档
	bool isHidden();                 //判断是否隐藏
	long long length();              //获取文件字节大小
	bool Delete();                   //删除文件
	std::vector<std::string> list(); //获取子文件列表
	std::vector<File> listFile();    //获取子文件的File对象列表
	bool mkdir();                    //创建新目录
	bool renameTo(std::string newName);  //重命名
};