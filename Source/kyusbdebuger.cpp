#include <iostream>
#include<sstream>
#include "Include/libusb.h"
#include "tostring.h"
extern "C" int init()
{
	return libusb_init(NULL);
}
extern "C" void uninit()
{
	libusb_exit(NULL);
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
	stringstream ss;
	print_devs((libusb_device **)devices,ss);
	string str=ss.str();
	int len=str.size();
	*buffer=new char[len+1];
	strcpy(*buffer,str.c_str());
}
extern "C" void device_to_ascii(void ** devices,int index,char** buffer)
{
	stringstream ss;
	print_dev((libusb_device **)devices,index,ss);
	string str=ss.str();
	int len=str.size();
	*buffer=new char[len+1];
	strcpy(*buffer,str.c_str());
}
