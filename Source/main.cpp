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
	auto cnt = get_devices(context,&devs);
	if (cnt < 0){
		uninit(context);
		return (int) cnt;
	}

PRINT_DEVICES:	
	//system("cls");	
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
	cout<<"1: Start Debug"<<endl;
	cout<<"9: Go to parent menu"<<endl;
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
void debug_in(int type,void * handle,int endpoint);
void debug_out(int type,void * handle,int endpoint);
void debugit(void ** devs,int dev_index)
{
	//system("cls");
	void * dev_handle=nullptr;
	int *interfaces=nullptr;
	int count=-1;
	auto ret= get_interfaces(devs,dev_index,&interfaces,&count);
	
	int interface_number=0;
	cout<<"Please select an interface in (";
	for(int i=0;i<count;++i)cout<<interfaces[i]<<",";
	cout<<"):"<<endl;
	delete interfaces;
	cin>>interface_number;
	
	ret = open_device(devs,dev_index,interface_number,&dev_handle);	
	cout<<"open_device(...)=="<<ret<<endl;
	if(ret < 0)return;
	
	ret= claim_interface(dev_handle,interface_number);
	cout<<"claim_interface=="<<ret<<endl;
	if(ret<0)goto EXIT_debugit;
	
	int *endpoints=nullptr;
	count=-1;
	ret= get_endpoints(devs,dev_index,interface_number,&endpoints,&count);
	int endpoint_number=0;
	cout<<"Please select an endpoint in (";
	for(int i=0;i<count;++i)cout<<endpoints[i]<<",";
	cout<<"):"<<endl;
	delete endpoints;
	cin>>endpoint_number;
	
	int direction=0B10000000&endpoint_number;
	int type=-1;
	get_endpoint_type(devs,dev_index,interface_number,endpoint_number,&type);
	cout<<"direction="<<direction<<endl;
	cout<<"type="<<type<<endl;
	if(direction==0X80)debug_in(type,dev_handle,endpoint_number);
	if(direction==0X00)debug_out(type,dev_handle,endpoint_number);
//	system("pause");
	
	int operation_index=-1;
	cout<<"Please select "<<endl;	
	
	ret= release_interface(dev_handle,interface_number);
	cout<<"release_interface=="<<ret<<endl;
	if(ret<0)goto EXIT_debugit;

EXIT_debugit:	
	close_device(dev_handle,interface_number);
	getchar();
}
void debug_in(int type,void * handle,int endpoint)
{
	//system("cls");
	cout<<"################Selected Operations################"<<endl;
	cout<<"1: Synchronous read"<<endl;
	cout<<"2: Asynchronous read"<<endl;
	cout<<"0: Exit"<<endl;
	
	int op_index=0;
	IN:cout<<"Please input operation:"<<endl;
	cin>>op_index;
	if(op_index==0)return;
	//if(type==0X00)return "CONTROL";
	//if(type==0X01)return "ISOCHRONOUS";
	//if(type==0X02)return "BULK";
	//if(type==0X03)return "INTERRUPT";
	if(op_index==1)
	{
		if(type==0X03)
		{
			unsigned char data[1024]={0};
			int length=4;
			int transferred;
			unsigned int timeout=1000;
			auto ret=interrupt_transfer (handle, (unsigned char)endpoint, data, length, &transferred, timeout);
			cout<<"interrupt_transfer(......)"<<ret<<endl;
		}
	}
	else if(op_index==2)
	{
		
	}
	else goto IN;
}
void debug_out(int type,void * handle,int endpoint)
{}