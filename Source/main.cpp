#include <iostream>
#include "kyusbdebuger.h"
using namespace std;
void debugit(void ** devs,int index);
int main(void)
{	
	void *context=NULL;
	auto ret = init(&context);
	if (ret < 0)
		return ret;
	
	void **devs=NULL;
	auto cnt = get_devices(&devs);
	if (cnt < 0){
		uninit(context);
		return (int) cnt;
	}

PRINT_DEVICES:	
	char * buffer=NULL;
	devices_to_ascii(devs,&buffer);
	cout<<buffer<<endl;
	delete[] buffer;
	
	cout<<"Please input opening device index:";
	int index=-1;
	cin>>index;
	
	cout<<"################Selected Device################"<<endl;
	device_to_ascii(devs,index,&buffer);
	cout<<buffer<<endl;
	delete[] buffer;
	cout<<"###############################################"<<endl;
	
	cout<<"################Selected Operations################"<<endl;
	cout<<"1: Debug"<<endl;
	cout<<"9: Back"<<endl;
	cout<<"0: Exit"<<endl;
	while(true)
	{	
		int op=0;
		cin>>op;
		if(op==1){debugit(devs,index);goto PRINT_DEVICES;}
		if(op==9)goto PRINT_DEVICES;
		if(op==0)break;		
	}

	free_devices(devs);
	uninit(context);
	return 0;
}
void debugit(void ** devs,int index)
{
	void * dev_handle=NULL;
	auto ret = open_device(devs,index,0,&dev_handle);
	if(ret < 0)
	{
		cout<<"\tFailed libusb_open(...)=="<<ret<<endl;
		return;
	}
	cout<<"44444444444444444444444444"<<endl;
	
}