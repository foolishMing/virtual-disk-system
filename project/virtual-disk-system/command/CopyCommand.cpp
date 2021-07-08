#include "CopyCommand.h"


CopyCommand::CopyCommand()
{

}

CopyCommand::~CopyCommand()
{

}


//-feature
//复制文件
//-format
//copy src dst
//-constraints
//1、仅支持/y参数，同名覆盖
//2、文件A -> 目录B ：合法，需要考虑是否重名，insert(new node)
//3、目录A -> 目录B ：合法，将目录A下的所有文件拷贝到目录B下，insert(new_node)
//4、磁盘文件A -> 目录B : 合法，同2
//5、磁盘目录A -> 目录B : 合法，同3
//6、src中可以使用通配符，寻找路径时需要调用FuzzyMatch来匹配
void CopyCommand::Handle(const CommandArg& arg, NodeTreeManager& proxy)
{

}