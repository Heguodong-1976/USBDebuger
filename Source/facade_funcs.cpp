#include <iostream>
#include "kyusbdebuger.h"
using namespace std;
extern "C" void view_devices(void)
{		
	void *context=NULL;
	auto ret = init(&context);
	if (ret < 0)
		return;
	
	void **devs=NULL;
	auto cnt = get_devices(context,&devs);
	if (cnt < 0){
		uninit(context);
		return;
	}

	char * buffer=NULL;
	devices_to_ascii(devs,&buffer);
	cout<<buffer;
	delete[] buffer;
	
	free_devices(devs);
	uninit(context);
	return ;
}

extern "C" void view_device(int venderid,int  productid)
{
	cout<<"view_device "<<venderid<<" "<<productid<<endl;	
}