#include <iostream>
#include<sstream>
#include "Include/libusb.h"
#include "tostring.h"
#include <vector>
#include <map>
using namespace std;
map<libusb_context*,libusb_device**> context_dic;
extern "C" int init(void **context)
{
	auto ret=libusb_init((libusb_context **)context);
	if(ret!=LIBUSB_SUCCESS)return 10000+ret;
	libusb_device ** devices=nullptr;
	ret=libusb_get_device_list((libusb_context *)(*context), &devices);
	if(ret<0)return 20000+ret;
	context_dic[(libusb_context *)(*context)]=devices;
	
	return 0;
}
extern "C" void uninit(void *context)
{	
	auto devices=context_dic[(libusb_context*)context];
	libusb_free_device_list(devices,0);
	auto itor=context_dic.find((libusb_context*)context);
	context_dic.erase(itor);	
	libusb_exit((libusb_context *)context);
}
extern "C" void devices_to_ascii(void *context,char** buffer)
{
	ostringstream ss;
	auto devices=context_dic[(libusb_context*)context];
	print_devs((libusb_device **)devices,ss);
	string str=ss.str();
	int len=str.size();
	*buffer=new char[len+1];
	strcpy(*buffer,str.c_str());
}
extern "C" void device_to_ascii(void * device,char** buffer)
{
	ostringstream ss;
	print_dev((libusb_device *)device,ss);
	string str=ss.str();
	int len=str.size();
	*buffer=new char[len+1];
	strcpy(*buffer,str.c_str());
}

extern "C" void* open_device(void *context,int vendor_id,int product_id)
{
	auto ret= libusb_open_device_with_vid_pid((libusb_context *)context,(uint16_t)vendor_id,(uint16_t)product_id);
	if(ret==NULL)return nullptr;
	libusb_set_auto_detach_kernel_driver((libusb_device_handle *)ret, TRUE);
	return ret;
}
extern "C" void close_device(void * handle)
{	
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
extern "C" void* find_device(void *context,int venderid,int productid)
{
	auto devices=context_dic[(libusb_context*)context];
	for(int i=0;;++i)
	{
		auto device=devices[i];
		if(device==nullptr)break;
		libusb_device_descriptor desc;
		auto ret = libusb_get_device_descriptor(device, &desc);
		if(venderid!=desc.idVendor)continue;
		if(productid!=desc.idProduct)continue;
		return device;		
	}
	return nullptr;	
}
extern "C" int get_interfaces(void *context,int venderid,int productid,int ** interface_numbers,int *count)
{	
	libusb_device * dev=(libusb_device *)find_device(context,venderid,productid);
	if(dev==nullptr)return -10000;
	libusb_device_descriptor desc;
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
extern "C" int get_endpoints(void *context,int venderid,int productid,int interface_number,int ** endpoints,int *count)
{
	libusb_device * dev=(libusb_device *)find_device(context,venderid,productid);
	if(dev==nullptr)return -10000;
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

extern "C" int get_endpoint_type(void *context,int venderid,int productid,int interface_number,int endpoint,int *type)
{
	libusb_device * dev=(libusb_device *)find_device(context,venderid,productid);
	if(dev==nullptr)return -10000;
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
					libusb_free_config_descriptor(config);
					return 0;
				}
				break;
			}
		}		
		libusb_free_config_descriptor(config);
	}
	return -10001;	
}
extern "C" int interrupt_transfer (void * handle, unsigned char endpoint, unsigned char *data, int length, int *transferred, unsigned int timeout)
{
	return libusb_interrupt_transfer((libusb_device_handle *)handle,endpoint,data,length,transferred,timeout);
}
