#include <iostream>
#include<sstream>
#include "Include/libusb.h"
#include "tostring.h"
#include <vector>
using namespace std;
extern "C" int init(void **context)
{
	return libusb_init((libusb_context **)context);
}
extern "C" void uninit(void *context)
{
	libusb_exit((libusb_context *)context);
}
extern "C" int get_devices(void *context,void *** devices)
{
	return libusb_get_device_list((libusb_context *)context, (libusb_device ***)devices);	
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

extern "C" int get_interfaces(void ** devs,int index,int ** interface_numbers,int *count)
{	
	libusb_device * dev=(libusb_device *)(devs[index]);
	struct libusb_device_descriptor desc;
	auto ret = libusb_get_device_descriptor(dev, &desc);
	vector<int> arr;
	for(auto config_index = 0; config_index < desc.bNumConfigurations; config_index++)
	{
		libusb_config_descriptor *config=NULL;
		auto ret = libusb_get_config_descriptor(dev, config_index, &config);
		for(int interface_index=0;interface_index<config->bNumInterfaces;++interface_index)
		{
			auto interface=config->interface[interface_index];
			for(int altsetting_index=0;altsetting_index<interface.num_altsetting;++altsetting_index)
			{
				auto interface_descriptor=interface.altsetting[altsetting_index];
				arr.push_back(interface_descriptor.bInterfaceNumber);				
			}
		}		
		libusb_free_config_descriptor(config);
	}
	*count=arr.size();
	* interface_numbers=new int[*count];
	for(int i=0;i<*count;++i)(* interface_numbers)[i]=arr[i];
	return 0;
}
extern "C" int get_endpoints(void ** devs,int index,int interface_number,int ** endpoints,int *count)
{
	libusb_device * dev=(libusb_device *)(devs[index]);
	struct libusb_device_descriptor desc;
	auto ret = libusb_get_device_descriptor(dev, &desc);
	vector<int> arr;
	for(auto config_index = 0; config_index < desc.bNumConfigurations; config_index++)
	{
		libusb_config_descriptor *config=NULL;
		auto ret = libusb_get_config_descriptor(dev, config_index, &config);
		for(int interface_index=0;interface_index<config->bNumInterfaces;++interface_index)
		{
			auto interface=config->interface[interface_index];
			for(int altsetting_index=0;altsetting_index<interface.num_altsetting;++altsetting_index)
			{
				auto interface_descriptor=interface.altsetting[altsetting_index];
				if(interface_number!=interface_descriptor.bInterfaceNumber)continue;
				for(int endpoint_index=0;endpoint_index<interface_descriptor.bNumEndpoints;++endpoint_index)
				{
					auto endpoint_descriptor=interface_descriptor.endpoint[endpoint_index];
					//cout<<"\t\t\t\tDescriptorType:\t"<<libusb_descriptor_type_to_string(endpoint_descriptor.bDescriptorType)<<endl;
					//cout<<"\t\t\t\tEndpointAddress:\t"<<endpointAddress_to_string(endpoint_descriptor.bEndpointAddress)<<endl;
					//cout<<"\t\t\t\tAttributes:\t"<<attributes_to_string(endpoint_descriptor.bmAttributes)<<endl;
					//cout<<"\t\t\t\tMaxPacketSize:\t"<<(int)endpoint_descriptor.wMaxPacketSize<<endl;
					//cout<<"\t\t\t\tInterval:\t"<<(int)endpoint_descriptor.bInterval<<endl;
					//cout<<"\t\t\t\tRefresh:\t"<<(int)endpoint_descriptor.bRefresh<<endl;
					//cout<<"\t\t\t\tSynchAddress:\t"<<(int)endpoint_descriptor.bSynchAddress<<endl;
					arr.push_back(endpoint_descriptor.bEndpointAddress);
				
				}
			}
		}		
		libusb_free_config_descriptor(config);
	}
	*count=arr.size();
	* endpoints=new int[*count];
	for(int i=0;i<*count;++i)(* endpoints)[i]=arr[i];
	return 0;
}
//extern "C" int get_endpoint_direction(void ** devs,int index,int interface_number,int endpoint,int *direction)
//{
//	libusb_device * dev=(libusb_device *)(devs[index]);
//	struct libusb_device_descriptor desc;
//	auto ret = libusb_get_device_descriptor(dev, &desc);
//	for(auto config_index = 0; config_index < desc.bNumConfigurations; config_index++)
//	{
//		libusb_config_descriptor *config=NULL;
//		auto ret = libusb_get_config_descriptor(dev, config_index, &config);
//		for(int interface_index=0;interface_index<config->bNumInterfaces;++interface_index)
//		{
//			auto interface=config->interface[interface_index];
//			for(int altsetting_index=0;altsetting_index<interface.num_altsetting;++altsetting_index)
//			{
//				auto interface_descriptor=interface.altsetting[altsetting_index];
//				if(interface_number!=interface_descriptor.bInterfaceNumber)continue;
//				for(int endpoint_index=0;endpoint_index<interface_descriptor.bNumEndpoints;++endpoint_index)
//				{
//					auto endpoint_descriptor=interface_descriptor.endpoint[endpoint_index];
//					int number=0b1111&endpoint_descriptor.bEndpointAddress;
//					if(number!=endpoint)continue;
//					*direction=0B10000000&endpoint_descriptor.bEndpointAddress;					
//				}
//				break;
//			}
//		}		
//		libusb_free_config_descriptor(config);
//	}
//	return 0;
//}
extern "C" int get_endpoint_type(void ** devs,int index,int interface_number,int endpoint,int *type)
{
	libusb_device * dev=(libusb_device *)(devs[index]);
	struct libusb_device_descriptor desc;
	auto ret = libusb_get_device_descriptor(dev, &desc);
	for(auto config_index = 0; config_index < desc.bNumConfigurations; config_index++)
	{
		libusb_config_descriptor *config=NULL;
		auto ret = libusb_get_config_descriptor(dev, config_index, &config);
		for(int interface_index=0;interface_index<config->bNumInterfaces;++interface_index)
		{
			auto interface=config->interface[interface_index];
			for(int altsetting_index=0;altsetting_index<interface.num_altsetting;++altsetting_index)
			{
				auto interface_descriptor=interface.altsetting[altsetting_index];
				if(interface_number!=interface_descriptor.bInterfaceNumber)continue;
				for(int endpoint_index=0;endpoint_index<interface_descriptor.bNumEndpoints;++endpoint_index)
				{
					auto endpoint_descriptor=interface_descriptor.endpoint[endpoint_index];
					int number=endpoint_descriptor.bEndpointAddress;
					if(number!=endpoint)continue;
					//cout<<"\t\t\t\tDescriptorType:\t"<<libusb_descriptor_type_to_string(endpoint_descriptor.bDescriptorType)<<endl;
					//cout<<"\t\t\t\tEndpointAddress:\t"<<endpointAddress_to_string(endpoint_descriptor.bEndpointAddress)<<endl;
					//cout<<"\t\t\t\tAttributes:\t"<<attributes_to_string(endpoint_descriptor.bmAttributes)<<endl;
					//cout<<"\t\t\t\tMaxPacketSize:\t"<<(int)endpoint_descriptor.wMaxPacketSize<<endl;
					//cout<<"\t\t\t\tInterval:\t"<<(int)endpoint_descriptor.bInterval<<endl;
					//cout<<"\t\t\t\tRefresh:\t"<<(int)endpoint_descriptor.bRefresh<<endl;
					//cout<<"\t\t\t\tSynchAddress:\t"<<(int)endpoint_descriptor.bSynchAddress<<endl;
					*type=endpoint_descriptor.bmAttributes&0B11;					
				}
				break;
			}
		}		
		libusb_free_config_descriptor(config);
	}
	return 0;	
}
extern "C" int interrupt_transfer (void * handle, unsigned char endpoint, unsigned char *data, int length, int *transferred, unsigned int timeout)
{
	auto ret= libusb_interrupt_transfer((libusb_device_handle *)handle,endpoint,data,length,transferred,timeout);
	cout<<"libusb_interrupt_transfer()===="<<ret<<endl;
	return ret;
}
