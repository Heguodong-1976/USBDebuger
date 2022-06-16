#include <iostream>
#include "kyusbdebuger.h"
using namespace std;
void debugit(void ** devs,int index);
void print_arr(string pre,string post,int * arr,int len);
void view_deviceinfo(void ** devs,int index);
void debug_in(int type,void * handle,int endpoint);
void debug_out(int type,void * handle,int endpoint);
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
	system("cls");	
	char * buffer=NULL;
	devices_to_ascii(devs,&buffer);
	cout<<buffer<<endl;
	delete[] buffer;
	
	cout<<"Please input an opening device index:";
	int device_index=-1;
	cin>>device_index;
	
	cout<<"################Operations################"<<endl;
	cout<<"\t1: View device infomation"<<endl;
	cout<<"\t2: Start Debug"<<endl;
	cout<<"\t9: Go to parent menu"<<endl;
	cout<<"\t0: Exit"<<endl;
	while(true)
	{	
		cout<<"Please input an operation index:";
		int op=0;
		cin>>op;
		if(op==1){view_deviceinfo(devs,device_index);goto PRINT_DEVICES;}
		if(op==2){debugit(devs,device_index);goto PRINT_DEVICES;}
		if(op==9)goto PRINT_DEVICES;
		if(op==0)break;		
	}

	free_devices(devs);
	uninit(context);
	return 0;
}
void view_deviceinfo(void ** devs,int index)
{
	cout<<"..................................Device infomation................................"<<endl;
	char * buffer=NULL;
	device_to_ascii(devs,index,&buffer);
	cout<<buffer<<endl;
	delete[] buffer;
	cout<<"..................................................................................."<<endl;
	while(true)
	{
		cout<<"################Operations################"<<endl;
		cout<<"\t1: Start Debug"<<endl;
		cout<<"\t0: Exit"<<endl;
		cout<<"Please input an operation index:";
		int op=-1;
		cin>>op;
		if(op==1){debugit(devs,index);break;}
		if(op==0)break;
	}	
}
void print_arr(string pre,string post,int * arr,int len)
{
	cout<<pre;
	if(len<=0)return;	
	cout<<arr[0];
	for(int i=1;i<len;++i)
		cout<<","<<arr[i];
	cout<<post;
}
void debugit(void ** devs,int dev_index)
{
	void * dev_handle=nullptr;
	int *interfaces=nullptr;
	int count=-1;
	auto ret= get_interfaces(devs,dev_index,&interfaces,&count);
	
	int interface_number=0;
	print_arr("Please input an interface in ( "," ):",interfaces,count);
	delete interfaces;
	cin>>interface_number;
	
	ret = open_device(devs,dev_index,interface_number,&dev_handle);		
	if(ret < 0)
	{
		cout<<"Failed : open_device(...)=="<<ret<<endl;
		return;		
	}	
	ret= claim_interface(dev_handle,interface_number);	
	if(ret<0)
	{
		cout<<"Failed : claim_interface(...)=="<<ret<<endl;
		goto EXIT_debugit;
	}
	
	int *endpoints=nullptr;
	count=-1;
	ret= get_endpoints(devs,dev_index,interface_number,&endpoints,&count);
	int endpoint_number=0;
	print_arr("Please input an endpoint in (","):",endpoints,count);
	delete endpoints;
	cin>>endpoint_number;
	
	int direction=0B10000000&endpoint_number;
	int type=-1;
	get_endpoint_type(devs,dev_index,interface_number,endpoint_number,&type);
	cout<<"direction="<<direction<<endl;
	cout<<"type="<<type<<endl;
	if(direction==0X80)debug_in(type,dev_handle,endpoint_number);
	if(direction==0X00)debug_out(type,dev_handle,endpoint_number);
	
	ret= release_interface(dev_handle,interface_number);
	cout<<"release_interface=="<<ret<<endl;
	if(ret<0)goto EXIT_debugit;

EXIT_debugit:	
	close_device(dev_handle,interface_number);
	getchar();
}
void debug_Syn_interrupt_transfer(void * handle,int endpoint)
{
	while(true)
	{
		cout<<"################Operations################"<<endl;
		cout<<"\t1: run 10 times"<<endl;
		cout<<"\t0: Exit"<<endl;		
		int op_index=0;
		cout<<"Please input operation:";
		cin>>op_index;
		if(op_index==0)return;
		if(op_index==1)
		{	
			for(int i=0;i<10;++i)
			{
				cout<<"Time "<<i+1<<":";
				unsigned char data[1024]={0};
				int length=4;
				int transferred;
				unsigned int timeout=1000;
				auto ret=interrupt_transfer(handle, (unsigned char)endpoint, data, length, &transferred, timeout);				
				if(ret<0)cout<<"Failed:interrupt_transfer(......)"<<ret;
				else
				{
					cout<<"transferred="<<transferred<<":";
					for(int t=0;t<transferred;t++)
						cout<<" "<<hex<<(int)data[t];
				}
				cout<<endl;
			}
		}
	}
}
void debug_in(int type,void * handle,int endpoint)
{
	cout<<"################Operations################"<<endl;
	cout<<"\t1: Synchronous read"<<endl;
	cout<<"\t2: Asynchronous read"<<endl;
	cout<<"\t0: Exit"<<endl;
	
	int op_index=0;
	cout<<"Please input operation:";
	cin>>op_index;
	if(op_index==0)return;
	//if(type==0X00)return "CONTROL";
	//if(type==0X01)return "ISOCHRONOUS";
	//if(type==0X02)return "BULK";
	//if(type==0X03)return "INTERRUPT";
	if(op_index==1&&type==0X03)debug_Syn_interrupt_transfer(handle,endpoint);
	if(op_index==2)
	{}
}
void debug_out(int type,void * handle,int endpoint)
{}