#include <iostream>
#include "Include/libusb.h"
#include "struct.h"
#include "tostring.h"
using namespace std;
using namespace Kiyun;
void print_devs(libusb_device **devs,ostream& s);
void print_dev(libusb_device * dev,ostream& s);
int main(void)
{	
	auto ret = libusb_init(NULL);
	if (ret < 0)
		return ret;
	
	libusb_device **devs=NULL;
	auto cnt = libusb_get_device_list(NULL, &devs);
	if (cnt < 0){
		libusb_exit(NULL);
		return (int) cnt;
	}
	
	print_devs(devs,cout);
	
	wcout<<"Please input opening device index:";
	int index=-1;
	cin>>index;
	
	cout<<"################Selected Device################"<<endl;
	print_dev(devs[index],cout);
	cout<<"###############################################"<<endl;

	libusb_free_device_list(devs, 1);
	libusb_exit(NULL);
	return 0;
	cout<<"Hello World!"<<endl;
	
}
static void print_endpoint(const libusb_endpoint_descriptor* descs,int index,ostream& s)
{
	auto desc=descs[index];
	s<<"\t\t\t\tIndex:\t........."<<index<<"........."<<endl;
	s<<"\t\t\t\tEndpointAddress:\t"<<(int)desc.bEndpointAddress<<endl;
	s<<"\t\t\t\tAttributes:\t"<<(int)desc.bmAttributes<<endl;
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
	s<<"\t\t\tNumber of Endpoints:\t"<<(int)desc.bNumEndpoints<<endl;
	for(int i=0;i<desc.bNumEndpoints;++i)print_endpoint(desc.endpoint,i,s);
}
static void print_Interface(const libusb_interface* interfaces,int index,ostream& s)
{
	auto interface=interfaces[index];
	s<<"\t\tIndex:\t........."<<index<<"........."<<endl;
	s<<"\t\tNumber Of Alt Setting:\t"<<interface.num_altsetting<<endl;
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

	s<<"\tNumber of Interfaces:\t"<<(int)config->bNumInterfaces<<endl;
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
static void print_dev(libusb_device * dev,ostream& s)
{	
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
	s<<"Number of Configurations:"<<(int)desc.bNumConfigurations<<endl;
	for(auto i = 0; i < desc.bNumConfigurations; i++) print_configuration(dev,i,s);
}
static void print_devs(libusb_device **devs,ostream& s)
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