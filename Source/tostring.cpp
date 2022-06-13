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
string endpointAddress_to_string(int bEndpointAddress)
{
	int number=0b1111&bEndpointAddress;
	int dir=0B10000000&bEndpointAddress;
	char t[100];
	sprintf(t,"Number=%d Direction=%s",number,dir==0?"Out":"IN");
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

