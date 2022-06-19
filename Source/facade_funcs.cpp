#include <iostream>
#include "Include/libusb.h"
#include "kyusbdebuger.h"
using namespace std;
extern "C" void view_devices(void)
{		
	void *context=NULL;
	auto ret = init(&context);
	if (ret < 0)
		return;
	
	char * buffer=NULL;
	devices_to_ascii(context,&buffer);
	cout<<buffer;
	delete[] buffer;

	uninit(context);
	return ;
}

extern "C" void view_device(int venderid,int  productid)
{
	void *context=NULL;
	auto ret = init(&context);
	if (ret < 0)
		return;

	auto device=find_device(context,venderid,productid);	
	if(device!=nullptr)
	{
		char * buffer=NULL;
		device_to_ascii((libusb_device *)device,&buffer);
		cout<<buffer<<endl;
		delete[] buffer;
	}
	
	uninit(context);
	return ;
}
void debug_in(int type,void * handle,int endpoint);
void debug_out(int type,void * handle,int endpoint);
extern "C" void debug_device(int venderid, int productid,int interface_num,int endpoint_num)
{
	void *context=NULL;
	auto ret = init(&context);
	if (ret < 0)return;

	auto dev_handle=open_device(context,venderid,productid);
	if(dev_handle==nullptr)
	{
		uninit(context);
		return ;
	}
	int direction=0B10000000&endpoint_num;
	int type=-1;
	ret=get_endpoint_type(context,venderid,productid,interface_num,endpoint_num,&type);
	
	if(ret<0)
	{
		cout<<"Failed get_endpoint_type(......)=="<<ret<<endl;
		goto EXIT_debug_device;	
	}

	if(direction==0X80)debug_in(type,dev_handle,endpoint_num);
	if(direction==0X00)debug_out(type,dev_handle,endpoint_num);
	
EXIT_debug_device:
	ret= release_interface(dev_handle,interface_num);
	close_device(dev_handle);
	uninit(context);
}
void interrupt_transfer(void * handle,int endpoint)
{
	//while(true)
	//{
	//	cout<<"################Operations################"<<endl;
	//	cout<<"\t1: transfer 100 times"<<endl;
	//	cout<<"\t0: Exit"<<endl;		
	//	int op_index=0;
	//	cout<<"Please input operation:";
	//	cin>>op_index;
	//	if(op_index==0)return;
	//	if(op_index==1)
	//	{	
	//		for(int i=0;i<100;++i)
	//		{
	//			cout<<"Time "<<i+1<<":";
	//			unsigned char data[1024]={0};
	//			int length=4;
	//			int transferred;
	//			unsigned int timeout=0;//timeout (in milliseconds),unlimited timeout, use value 0
	//			auto ret=interrupt_transfer(handle, (unsigned char)endpoint, data, length, &transferred, timeout);				
	//			if(ret<0)cout<<"Failed:interrupt_transfer(......)"<<ret;
	//			else
	//			{
	//				cout<<"transferred="<<transferred<<":";
	//				for(int t=0;t<transferred;t++)
	//					cout<<" "<<hex<<(int)data[t];
	//			}
	//			cout<<endl;
	//		}
	//	}
	//}
}
void debug_in(int type,void * handle,int endpoint)
{
	while(true)
	{
		cout<<"################Operations################"<<endl;
		cout<<"\t1: Synchronous transfer"<<endl;
		cout<<"\t0: Exit"<<endl;
		
		int op_index=0;
		cout<<"Please input operation:";
		cin>>op_index;
		if(op_index==0)return;
		//if(op_index==1&&type==0X00)control_transfer(handle,endpoint);
		//if(op_index==1&&type==0X01)isochromous_transfer(handle,endpoint);
		//if(op_index==1&&type==0X02)bulk_transfer(handle,endpoint);
		if(op_index==1&&type==0X03)interrupt_transfer(handle,endpoint);
	}
}
void debug_out(int type,void * handle,int endpoint)
{}