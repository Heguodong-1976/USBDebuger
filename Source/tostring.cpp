#include <iostream>
#include <string>
#include "Include/libusb.h"
using namespace std;
string libusb_descriptor_type_to_string(uint8_t type)
{
	if(type== 0x01)return "DEVICE";
	if(type== 0x02)return "CONFIG";
	if(type== 0x03)return "STRING";
	if(type== 0x04)return "INTERFACE";
	if(type== 0x05)return "ENDPOINT";
	if(type== 0x0f)return "BOS";
	if(type== 0x10)return "DEVICE_CAPABILITY";
	if(type== 0x21)return "HID";
	if(type== 0x22)return "REPORT";
	if(type== 0x23)return "PHYSICAL";
	if(type== 0x29)return "HUB";
	if(type== 0x2a)return "SUPERSPEED_HUB";
	if(type== 0x30)return "SS_ENDPOINT_COMPANION";
	return NULL;
}
string speed_to_string(int speed)
{
	if(speed== LIBUSB_SPEED_LOW)return "1.5M";
	if(speed== LIBUSB_SPEED_FULL)return "12M";
	if(speed== LIBUSB_SPEED_HIGH)return "480M";
	if(speed== LIBUSB_SPEED_SUPER)return "5G";
	if(speed== LIBUSB_SPEED_SUPER_PLUS)return "10G";
	return "Unknown";	
}
string bcdUSB_to_string(int usb)
{
	if(usb==0x0200)return "USB 2.0";
	if(usb==0x0110)return "USB 1.2";
	return "Unknown";
}
string libusb_class_code_to_string(int deviceclass)
{
	if(deviceclass==0x00)return "PER_INTERFACE";
	if(deviceclass==0x01)return "AUDIO";
	if(deviceclass==0x02)return "COMM";
	if(deviceclass==0x03)return "HID";
	if(deviceclass==0x05)return "PHYSICAL";
	if(deviceclass==0x06)return "IMAGE";
	if(deviceclass==0x06)return "PTP";
	if(deviceclass==0x07)return "PRINTER";
	if(deviceclass==0x08)return "MASS_STORAGE";
	if(deviceclass==0x09)return "HUB";
	if(deviceclass==0x0a)return "DATA";
	if(deviceclass==0x0b)return "SMART_CARD";
	if(deviceclass==0x0d)return "CONTENT_SECURITY";
	if(deviceclass==0x0e)return "VIDEO";
	if(deviceclass==0x0f)return "PERSONAL_HEALTHCARE";
	if(deviceclass==0xdc)return "DIAGNOSTIC_DEVICE";
	if(deviceclass==0xe0)return "WIRELESS";
	if(deviceclass==0xef)return "MISCELLANEOUS";
	if(deviceclass==0xfe)return "APPLICATION";
	if(deviceclass==0xff)return "VENDOR_SPEC";
	return "Unknown";
}
string direction_to_string(int dir)
{
	if(dir==0X80)return "IN";
	if(dir==0X00)return "OUT";
	return "Unknown";
}
string endpointAddress_to_string(int bEndpointAddress)
{
	int number=0b1111&bEndpointAddress;
	int dir=0B10000000&bEndpointAddress;
	char t[100];
	sprintf(t,"Number=%d Direction=%s",number,direction_to_string(dir).c_str());
	return t;
}
string transfer_type_to_string(int type)
{
	if(type==0X00)return "CONTROL";
	if(type==0X01)return "ISOCHRONOUS";
	if(type==0X02)return "BULK";
	if(type==0X03)return "INTERRUPT";
	return "Unknown";	
}
string iso_sync_type_to_string_to_string(int type)
{
	if(type==0X00)return "NONE";
	if(type==0X01)return "ASYNC";
	if(type==0X02)return "ADAPTIVE";
	if(type==0X03)return "SYNC";
	return "Unknown";	
}
string iso_usage_type_to_string_to_string_to_string(int type)
{
	if(type==0X00)return "DATA";
	if(type==0X01)return "FEEDBACK";
	if(type==0X02)return "IMPLICIT";
	return "Unknown";	
}
string attributes_to_string(int bmAttributes)
{
	int transferType= bmAttributes&0B11;
	int iso_sync_type=bmAttributes&0B1100;
	int iso_usage_type=bmAttributes&0B110000;
	char t[255];
	sprintf(t,"TransferType=%s SyncType=%s SyncUsageType=%s",
				transfer_type_to_string(transferType).c_str(),
				iso_sync_type_to_string_to_string(iso_sync_type).c_str(),
				iso_usage_type_to_string_to_string_to_string(iso_usage_type).c_str());
	return t;
}

static void print_endpoint(const libusb_endpoint_descriptor* descs,int index,ostream& s)
{
	auto desc=descs[index];
	s<<"\t\t\t\tIndex:\t........."<<index<<"........."<<endl;
	s<<"\t\t\t\tDescriptorType:\t"<<libusb_descriptor_type_to_string(desc.bDescriptorType)<<endl;
	s<<"\t\t\t\tEndpointAddress:\t"<<endpointAddress_to_string(desc.bEndpointAddress)<<endl;
	s<<"\t\t\t\tAttributes:\t"<<attributes_to_string(desc.bmAttributes)<<endl;
	s<<"\t\t\t\tMaxPacketSize:\t"<<(int)desc.wMaxPacketSize<<endl;
	s<<"\t\t\t\tInterval:\t"<<(int)desc.bInterval<<endl;
	s<<"\t\t\t\tRefresh:\t"<<(int)desc.bRefresh<<endl;
	s<<"\t\t\t\tSynchAddress:\t"<<(int)desc.bSynchAddress<<endl;
}
static void print_altsetting(const libusb_interface_descriptor* descs,int index,ostream& s)
{
	auto desc=descs[index];
	s<<"\t\t\tIndex:\t........."<<index<<"........."<<endl;
	s<<"\t\t\tDescriptorType:\t"<<libusb_descriptor_type_to_string(desc.bDescriptorType)<<endl;
	s<<"\t\t\tAlternateSetting:\t"<<(int)desc.bAlternateSetting<<endl;
	s<<"\t\t\tNumber:\t"<<(int)desc.bInterfaceNumber<<endl;
	s<<"\t\t\tClass:\t"<<libusb_class_code_to_string(desc.bInterfaceClass)<<endl;
	s<<"\t\t\tSubClass:\t"<<(int)desc.bInterfaceSubClass<<endl;
	s<<"\t\t\tInterfaceProtocol:\t"<<(int)desc.bInterfaceProtocol<<endl;
	s<<"\t\t\tInterfaceIndex:\t"<<(int)desc.iInterface<<endl;
	s<<"\t\t\tCount of Endpoints:\t"<<(int)desc.bNumEndpoints<<endl;
	for(int i=0;i<desc.bNumEndpoints;++i)print_endpoint(desc.endpoint,i,s);
}
static void print_Interface(const libusb_interface* interfaces,int index,ostream& s)
{
	auto interface=interfaces[index];
	s<<"\t\tIndex:\t........."<<index<<"........."<<endl;
	s<<"\t\tCount of Alt Setting:\t"<<interface.num_altsetting<<endl;
	for(int i=0;i<interface.num_altsetting;++i)print_altsetting(interface.altsetting,i,s);
}
static void print_configuration(libusb_device * dev,int index,ostream& s)
{
	libusb_config_descriptor *config=NULL;

	auto ret = libusb_get_config_descriptor(dev, index, &config);
	if (LIBUSB_SUCCESS != ret) {
		s<<"Couldn't retrieve descriptors"<<endl;
		return;
	}
	s<<"\tIndex:\t........."<<index<<"........."<<endl;
	
	s<<"\tDescriptorType:\t"<<libusb_descriptor_type_to_string(config->bDescriptorType)<<endl;
	s<<"\tTotalLength:\t"<<config->wTotalLength<<endl;
	s<<"\tConfigurationValue:\t"<<(int)config->bConfigurationValue<<endl;
	s<<"\tConfiguration:\t"<<(int)config->iConfiguration<<endl;
	s<<"\tAttributes:\t"<<(int)config->bmAttributes<<endl;
	s<<"\tMaxPower:\t"<<(int)config->MaxPower<<endl;

	s<<"\tCount of Interfaces:\t"<<(int)config->bNumInterfaces<<endl;
	for(int i=0;i<config->bNumInterfaces;++i)print_Interface(config->interface,i,s);
	
	libusb_free_config_descriptor(config);
}
static void print_dev_inf(libusb_device * dev,ostream& s)
{
	libusb_device_descriptor desc;
	auto ret = libusb_get_device_descriptor(dev, &desc);
	if(ret<0)
	{
		s<<"failed to get device descriptor"<<endl;
		return;		
	}
	
	libusb_device_handle *dev_handle=NULL;
	ret=libusb_open(dev,&dev_handle);
	if(ret != 0)
	{
		s<<"\tFailed libusb_open(...)=="<<ret<<endl;
		return;
	}
	
	unsigned char product[255]={0};
	unsigned char serialNumber[255]={0};
	unsigned char manufacturer[255]={0};
	ret=libusb_get_string_descriptor_ascii(dev_handle,desc.iProduct,product,255);	
	ret=libusb_get_string_descriptor_ascii(dev_handle,desc.iSerialNumber,serialNumber,255);
	ret=libusb_get_string_descriptor_ascii(dev_handle,desc.iManufacturer,manufacturer,255);
	s<<"Product:\t"<<product<<endl;
	s<<"Manufacturer:\t"<<manufacturer<<endl;
	s<<"Serial Number:\t"<<serialNumber<<endl;
	libusb_close(dev_handle);
}
void print_dev(libusb_device ** devs,int index, ostream& s)//(libusb_device * dev,ostream& s)
{	
	libusb_device * dev=devs[index];
	struct libusb_device_descriptor desc;
	auto ret = libusb_get_device_descriptor(dev, &desc);
	if(ret<0)
	{
		s<<"failed to get device descriptor"<<endl;
		return;		
	}
	s<<"DescriptorType:\t"<<libusb_descriptor_type_to_string(desc.bDescriptorType)<<endl;
	auto speed=libusb_get_device_speed(dev);
	s<<"Vendor:\t"<<hex<<desc.idVendor<<endl;
	s<<"Product:\t"<<desc.idProduct<<endl;
	s<<"Speed:\t"<<speed_to_string(speed)<<endl;
	s<<"Type:\t"<<bcdUSB_to_string(desc.bcdUSB)<<endl;
	s<<"Class:\t"<<libusb_class_code_to_string(desc.bDeviceClass)<<endl;
	s<<"SubClass:\t"<<(int)desc.bDeviceSubClass<<endl;
	s<<"DeviceProtocol:\t"<<(int)desc.bDeviceProtocol<<endl;
	s<<"MaxPacketSize0:\t"<<(int)desc.bMaxPacketSize0<<endl;
	s<<"BCDDevice:\t"<<(int)desc.bcdDevice<<endl;
	print_dev_inf(dev,s);
	s<<"Count of Configurations:"<<(int)desc.bNumConfigurations<<endl;
	for(auto i = 0; i < desc.bNumConfigurations; i++) print_configuration(dev,i,s);
}
void print_devs(libusb_device **devs,ostream& s)
{	
	for(int i = 0;;++i)
	{
		auto dev = devs[i];
		if(dev==NULL)break;
		s<<dec<<i;
		struct libusb_device_descriptor desc;
		auto ret = libusb_get_device_descriptor(dev, &desc);
		if (ret < 0) {
			s<<"Failed libusb_get_device_descriptor(...)="<<ret<<"at file:"<<__FILE__<<"line:"<<__LINE__<<endl;
			continue;
		}
		
		s<<"\tVendor:"<<hex<<desc.idVendor<<"\tProduct:"<<desc.idProduct;
		auto bus=libusb_get_bus_number(dev);
		auto address=libusb_get_device_address(dev);
		s<<"\tBus:"<<(int)bus<<"\tAddress:"<<(int)address;
		
		uint8_t path[8]; 
		ret = libusb_get_port_numbers(dev, path, sizeof(path));
		s<<"\tPath : ";
		for (int j = 0; j < ret; j++)
			s<<dec<<"."<<(int)path[j];
		
		libusb_device_handle *dev_handle=NULL;
		ret=libusb_open(dev,&dev_handle);
		if(ret != 0)
		{
			s<<"\tFailed libusb_open(...)=="<<ret<<endl;
			continue;
		}
		
		unsigned char product[255]={0};
		unsigned char serialNumber[255]={0};
		unsigned char manufacturer[255]={0};
		ret=libusb_get_string_descriptor_ascii(dev_handle,desc.iProduct,product,255);	
		ret=libusb_get_string_descriptor_ascii(dev_handle,desc.iSerialNumber,serialNumber,255);
		ret=libusb_get_string_descriptor_ascii(dev_handle,desc.iManufacturer,manufacturer,255);
		libusb_close(dev_handle);
		s<<"\tSuccess:"<<serialNumber<<":"<<product<<":"<<manufacturer;
		s<<endl;
	}
}