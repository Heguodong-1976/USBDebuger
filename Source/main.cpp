#include <iostream>
#include "Include/libusb.h"
#include "struct.h"
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
static void print_configuration(libusb_device * dev,int index,ostream& s)
{
	libusb_config_descriptor *config=NULL;

	auto ret = libusb_get_config_descriptor(dev, 0, &config);
	if (LIBUSB_SUCCESS != ret) {
		s<<"Couldn't retrieve descriptors"<<endl;
		return;
	}
	uint8_t i;
	//################Configuration################
	s<<"............Index:\t"<<index<<"............"<<endl;
	s<<"\tTotalLength:\t"<<config->wTotalLength<<endl;
	s<<"\tNumInterfaces:\t"<<(int)config->bNumInterfaces<<endl;
	s<<"\tConfigurationValue:\t"<<(int)config->bConfigurationValue<<endl;
	s<<"\tConfiguration:\t"<<(int)config->iConfiguration<<endl;
	s<<"\tAttributes:\t"<<(int)config->bmAttributes<<endl;
	s<<"\tMaxPower:\t"<<(int)config->MaxPower<<endl;
	libusb_free_config_descriptor(config);
}
static void print_dev(libusb_device * dev,ostream& s)
{	
	auto speed=libusb_get_device_speed(dev);
	s<<"Speed:\t";
	switch (speed) {
		case LIBUSB_SPEED_LOW:		s << "1.5M"; break;
		case LIBUSB_SPEED_FULL:		s << "12M"; break;
		case LIBUSB_SPEED_HIGH:		s << "480M"; break;
		case LIBUSB_SPEED_SUPER:	s << "5G"; break;
		case LIBUSB_SPEED_SUPER_PLUS:	s << "10G"; break;
		default:	s<< "Unknown";
	}
	s<<endl;
	struct libusb_device_descriptor desc;
	auto ret = libusb_get_device_descriptor(dev, &desc);
	if(ret<0)
	{
		s<<"failed to get device descriptor"<<endl;
		return;		
	}
	s<<"Vendor:\t"<<hex<<desc.idVendor<<endl;
	s<<"Product:\t"<<desc.idProduct<<endl;
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