#ifndef __STRUCT_H__
#define __STRUCT_H__
#include <string>
using namespace std;
namespace Kiyun
{
	struct Error
	{
		string Message;
		int ErrorCode;
		string File;
		int Line;
	};
}
#endif
