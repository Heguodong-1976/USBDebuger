#include <iostream>
#include<sstream>
#include "Include/libusb.h"
#include "tostring.h"
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
	cout<<"device index::"<<index<<endl;
	auto ret=libusb_open(((libusb_device **)devs)[index],(libusb_device_handle **)dev_handle);
	if(ret != 0)return -10000+ret;
	cout<<"111111111111111111111"<<endl;
	ret=libusb_kernel_driver_active((libusb_device_handle *)dev_handle, interface_num);
	if(ret==0)return 0;
	cout<<"222222222222222222222"<<endl;
	if(ret<0)return -20000+ret;
	ret=libusb_detach_kernel_driver((libusb_device_handle *)dev_handle, interface_num);
	if(ret==0)return 0;
	cout<<"3333333333333333333"<<endl;
	libusb_close((libusb_device_handle *)dev_handle);
	return -30000+ret;
}
