#ifndef __TOSTRING_H__
#define __TOSTRING_H__
#include <string>
using namespace std;
string libusb_descriptor_type_to_string(uint8_t type);
string speed_to_string(int speed);
string bcdUSB_to_string(int usb);
string libusb_class_code_to_string(int deviceclass);
string endpointAddress_to_string(int bEndpointAddress);
#endif
