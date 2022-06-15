#include <iostream>
#include<sstream>
#include "Include/libusb.h"
#include "tostring.h"
#include <list>
using namespace std;
extern "C" int init(void **context)
{
	return libusb_init((libusb_context **)context);
}
extern "C" void uninit(void *context)
{
	libusb_exit((libusb_context *)context);
}
extern "C" int get_devices(void *** devices)
{
	return libusb_get_device_list(NULL, (libusb_device ***)devices);	
}
extern "C" void free_devices(void ** devices)
{
	libusb_free_device_list((libusb_device **)devices,0);
}
extern "C" void devices_to_ascii(void **devices,char** buffer)
{
	ostringstream ss;
	print_devs((libusb_device **)devices,ss);
	string str=ss.str();
	int len=str.size();
	*buffer=new char[len+1];
	strcpy(*buffer,str.c_str());
}
extern "C" void device_to_ascii(void ** devices,int index,char** buffer)
{
	ostringstream ss;
	print_dev((libusb_device **)devices,index,ss);
	string str=ss.str();
	int len=str.size();
	*buffer=new char[len+1];
	strcpy(*buffer,str.c_str());
}
extern "C" int open_device(void ** devs,int index,int interface_num,void** dev_handle)
{
	auto ret=libusb_open(((libusb_device **)devs)[index],(libusb_device_handle **)dev_handle);
	if(ret < 0)return -10000+ret;
	ret=libusb_kernel_driver_active((libusb_device_handle *)(*dev_handle), interface_num);
	cout<<"libusb_kernel_driver_active(...)="<<ret<<endl;
	//if(ret==0)return 0;	
	//if(ret<0)return -20000+ret;
	
	ret=libusb_detach_kernel_driver((libusb_device_handle *)(*dev_handle), interface_num);
	cout<<"libusb_detach_kernel_driver(...)="<<ret<<endl;
	//if(ret==0)return 0;
	
	//libusb_close((libusb_device_handle *)(*dev_handle));
	return 0;
}
extern "C" void close_device(void * handle,int interface_number)
{
	libusb_attach_kernel_driver((libusb_device_handle *)handle, interface_number);
	libusb_close((libusb_device_handle *)handle);
}
extern "C" int claim_interface(void * handle,int interface_number)
{
	return libusb_claim_interface((libusb_device_handle *)handle, interface_number);
}
extern "C" int release_interface(void * handle,int interface_number)
{
	return libusb_release_interface((libusb_device_handle *)handle, interface_number);
}

extern "C" int get_interfaces(void ** devs,int index,int ** interfaces,int *count)
{
	struct libusb_device_descriptor desc;
	auto ret = libusb_get_device_descriptor(((libusb_device**)devs)[index], &desc);
	list<int> lst;
	//libusb_config_descriptor *config=NULL;
	//auto ret = libusb_get_config_descriptor(((libusb_device**)devs), index, &config);
	//if (ret<0) return 40000+ret;
	//*count=config->bNumInterfaces;
	//*interfaces=new int[*count];
	//for(int i=0;i<config->bNumInterfaces;++i)
	//{
	//	auto interface=config->interface[i];
	//	for(int j=0;j<interface.num_altsetting;++j)
	//	{
	//		auto desc=descs[j];
	//	}
	//}
	return 0;
}