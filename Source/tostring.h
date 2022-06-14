#ifndef __TOSTRING_H__
#define __TOSTRING_H__
#include <string>
using namespace std;
string libusb_descriptor_type_to_string(uint8_t type);
string speed_to_string(int speed);
string bcdUSB_to_string(int usb);
string libusb_class_code_to_string(int deviceclass);
string endpointAddress_to_string(int bEndpointAddress);
string attributes_to_string(int bmAttributes);
string transfer_type_to_string(int type);
string iso_sync_type_to_string(int type);
string iso_usage_type_to_string(int type);
void print_devs(libusb_device **devs,ostream& s);
void print_dev(libusb_device ** dev,int index, ostream& s);
#endif
