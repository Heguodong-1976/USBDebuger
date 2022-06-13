#include <string>
#include "Include/libusb.h"
using namespace std;
string libusb_descriptor_type_to_string(uint8_t type)
{
	if(type== 0x01)return "LIBUSB_DT_DEVICE";
	if(type== 0x02)return "LIBUSB_DT_CONFIG";
	if(type== 0x03)return "LIBUSB_DT_STRING";
	if(type== 0x04)return "LIBUSB_DT_INTERFACE";
	if(type== 0x05)return "LIBUSB_DT_ENDPOINT";
	if(type== 0x0f)return "LIBUSB_DT_BOS";
	if(type== 0x10)return "LIBUSB_DT_DEVICE_CAPABILITY";
	if(type== 0x21)return "LIBUSB_DT_HID";
	if(type== 0x22)return "LIBUSB_DT_REPORT";
	if(type== 0x23)return "LIBUSB_DT_PHYSICAL";
	if(type== 0x29)return "LIBUSB_DT_HUB";
	if(type== 0x2a)return "LIBUSB_DT_SUPERSPEED_HUB";
	if(type== 0x30)return "LIBUSB_DT_SS_ENDPOINT_COMPANION";
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
	if(deviceclass==0x00)return "LIBUSB_CLASS_PER_INTERFACE";
	if(deviceclass==0x01)return "LIBUSB_CLASS_AUDIO";
	if(deviceclass==0x02)return "LIBUSB_CLASS_COMM";
	if(deviceclass==0x03)return "LIBUSB_CLASS_HID";
	if(deviceclass==0x05)return "LIBUSB_CLASS_PHYSICAL";
	if(deviceclass==0x06)return "LIBUSB_CLASS_IMAGE";
	if(deviceclass==0x06)return "LIBUSB_CLASS_PTP";
	if(deviceclass==0x07)return "LIBUSB_CLASS_PRINTER";
	if(deviceclass==0x08)return "LIBUSB_CLASS_MASS_STORAGE";
	if(deviceclass==0x09)return "LIBUSB_CLASS_HUB";
	if(deviceclass==0x0a)return "LIBUSB_CLASS_DATA";
	if(deviceclass==0x0b)return "LIBUSB_CLASS_SMART_CARD";
	if(deviceclass==0x0d)return "LIBUSB_CLASS_CONTENT_SECURITY";
	if(deviceclass==0x0e)return "LIBUSB_CLASS_VIDEO";
	if(deviceclass==0x0f)return "LIBUSB_CLASS_PERSONAL_HEALTHCARE";
	if(deviceclass==0xdc)return "LIBUSB_CLASS_DIAGNOSTIC_DEVICE";
	if(deviceclass==0xe0)return "LIBUSB_CLASS_WIRELESS";
	if(deviceclass==0xef)return "LIBUSB_CLASS_MISCELLANEOUS";
	if(deviceclass==0xfe)return "LIBUSB_CLASS_APPLICATION";
	if(deviceclass==0xff)return "LIBUSB_CLASS_VENDOR_SPEC";
	return "Unknown";
}

