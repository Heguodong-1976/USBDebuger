#include <iostream>
#include "kyusbdebuger.h"
using namespace std;

int main(void)
{	
	auto ret = init();
	if (ret < 0)
		return ret;
	
	void **devs=NULL;
	auto cnt = get_devices(&devs);
	if (cnt < 0){
		uninit();
		return (int) cnt;
	}
	
	char * buffer=NULL;
	devices_to_ascii(devs,&buffer);
	cout<<buffer<<endl;
	delete[] buffer;
	
	wcout<<"Please input opening device index:";
	int index=-1;
	cin>>index;
	
	cout<<"################Selected Device################"<<endl;
	device_to_ascii(devs,index,&buffer);
	cout<<buffer<<endl;
	delete[] buffer;
	cout<<"###############################################"<<endl;

	free_devices(devs);
	uninit();
	return 0;
}
