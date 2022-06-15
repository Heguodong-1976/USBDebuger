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
		cout<<"Please select operation index:";
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
void debugit(void ** devs,int dev_index)
{
	system("cls");
	void * dev_handle=nullptr;
	int *interfaces=nullptr;
	int count=-1;
	auto ret= get_interfaces(devs,dev_index,&interfaces,&count);
	if(ret < 0)
	{
		cout<<"get_interfaces(...)=="<<ret<<endl;
		return;
	}
	
	int interface_number=0;
	cout<<"Please select an interface in (";
	for(int i=0;i<count;++i)cout<<interfaces[i]<<" ";
	cout<<"):"<<endl;	
	cin>>interface_number;
	
	
	ret = open_device(devs,dev_index,interface_number,&dev_handle);	
	if(ret < 0)
	{
		cout<<"Failed libusb_open(...)=="<<ret<<endl;
		return;
	}
	
	ret= claim_interface(dev_handle,interface_number);
	cout<<"claim_interface=="<<ret<<endl;
	if(ret<0)goto EXIT_debugit;
	
	
	ret= release_interface(dev_handle,interface_number);
	cout<<"release_interface=="<<ret<<endl;
	if(ret<0)goto EXIT_debugit;

EXIT_debugit:	
	close_device(dev_handle,interface_number);
	getchar();
}