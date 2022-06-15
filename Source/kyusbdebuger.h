#ifndef __KYUSBDEBUGER_H__
#define __KYUSBDEBUGER_H__
extern "C" int init(void **context);
extern "C" void uninit(void *context); 
extern "C" int get_devices(void *** devices);
extern "C" void free_devices(void ** devices);
extern "C" void devices_to_ascii(void **devs,char** buffer);
extern "C" void device_to_ascii(void ** devs,int index,char** buffer);

extern "C" int open_device(void ** devs,int index,int interface_num,void** dev_handle);
extern "C" void close_device(void * handle,int interface_number);
extern "C" int claim_interface(void * handle,int interface_number);
extern "C" int release_interface(void * handle,int interface_number);

extern "C" int get_interfaces(void ** devs,int index,int ** interfaces,int *count);

#endif
