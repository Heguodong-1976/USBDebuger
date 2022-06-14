#ifndef __KYUSBDEBUGER_H__
#define __KYUSBDEBUGER_H__
extern "C" int init();
extern "C" void uninit(); 
extern "C" int get_devices(void *** devices);
extern "C" void free_devices(void ** devices);
extern "C" void devices_to_ascii(void **devs,char** buffer);
extern "C" void device_to_ascii(void ** devs,int index,char** buffer);
#endif
