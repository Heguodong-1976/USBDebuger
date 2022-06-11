#include <iostream>
#include "Include/libusb.h"
#include "struct.h"
using namespace std;
using namespace Kiyun;
Error* print_devs(libusb_device **devs,ostream& s);
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
	
	auto error=print_devs(devs,cout);
	if(error!=NULL)
	{
		delete error;
		return 1;
	}

	libusb_free_device_list(devs, 1);
	libusb_exit(NULL);
	return 0;
	cout<<"Hello World!"<<endl;
	
}
static Error* print_devs(libusb_device **devs,ostream& s)
{
	libusb_device *dev;
	int i = 0;
	while ((dev = devs[i++]) != NULL) {
		struct libusb_device_descriptor desc;
		auto ret = libusb_get_device_descriptor(dev, &desc);
		if (ret < 0) {
			cerr<<"Failed libusb_get_device_descriptor(...)"<<endl;
			//return new Error{"Failed libusb_get_device_descriptor(...)",ret,__FILE__,__LINE__};
		}
		
		cout<<dec<<i<<"  Vendor:"<<hex<<desc.idVendor<<"\tProduct:"<<desc.idProduct;
		auto bus=libusb_get_bus_number(dev);
		auto address=libusb_get_device_address(dev);
		cout<<"\tBus:"<<(int)bus<<"\tAddress:"<<(int)address;
		
		uint8_t path[8]; 
		ret = libusb_get_port_numbers(dev, path, sizeof(path));
		cout<<"\tPath:";
		for (int j = 0; j < ret; j++)
			cout<<"."<<(int)path[j];
		cout<<"\t";		
		
		libusb_device_handle *dev_handle=NULL;
		ret=libusb_open(dev,&dev_handle);
		if(ret==libusb_error::LIBUSB_ERROR_NOT_SUPPORTED)
		{
			cout<<"\tFailed libusb_open(...)==LIBUSB_ERROR_NOT_SUPPORTED"<<endl;
			continue;
		}
		
		unsigned char product[255]={0};
		unsigned char serialNumber[255];
		unsigned char manufacturer[255];
		ret=libusb_get_string_descriptor_ascii(dev_handle,desc.iProduct,product,255);
		ret=libusb_get_string_descriptor_ascii(dev_handle,desc.iSerialNumber,serialNumber,255);
		ret=libusb_get_string_descriptor_ascii(dev_handle,desc.iManufacturer,manufacturer,255);
		libusb_close(dev_handle);
		cout<<"\tSuccess:"<<serialNumber<<":"<<product<<":"<<manufacturer;

		cout<<endl;
	}
	return NULL;	
}