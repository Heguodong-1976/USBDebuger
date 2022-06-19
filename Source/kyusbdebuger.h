#ifndef __KYUSBDEBUGER_H__
#define __KYUSBDEBUGER_H__
extern "C" int init(void **context);
extern "C" void uninit(void *context); 
extern "C" void devices_to_ascii(void *context,char** buffer);
extern "C" void device_to_ascii(void * dev,char** buffer);

extern "C" void* find_device(void *context,int venderid,int productid);
extern "C" int get_interfaces(void *context,int venderid,int productid,int ** interface_numbers,int *count);
extern "C" int get_endpoints(void *context,int venderid,int productid,int interface_number,int ** endpoints,int *count);
extern "C" int get_endpoint_type(void *context,int venderid,int productid,int interface_number,int endpoint,int *type);

extern "C" void* open_device(void *context,int vendor_id,int product_id);
extern "C" void close_device(void * handle);

extern "C" int claim_interface(void * handle,int interface_number);
extern "C" int release_interface(void * handle,int interface_number);

extern "C" int interrupt_transfer (void * handle, unsigned char endpoint, unsigned char *data, int length, int *transferred, unsigned int timeout);

//facade
extern "C" void view_devices(void);
extern "C" void view_device(int venderid,int productid);
extern "C" void debug_device(int venderid, int productid,int interface_num,int endpoint_num);
#endif
