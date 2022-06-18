#include "Include/CLI/CLI.hpp"
#include "kyusbdebuger.h"
#include <iostream>
using namespace std;
int main(int argc, char **argv) {
    CLI::App app{"Kiyun USBDebuger......"};
    app.footer(".........................................................."); 
    app.get_formatter()->column_width(40); 
    app.require_subcommand(1); 
	app.ignore_case(true);
	
	auto sub1 = app.add_subcommand("view", "view infomations,et device list or spiecial device");
	auto sub1_1=sub1->add_subcommand("devices", "view allocate device");
	auto sub1_2=sub1->add_subcommand("device", "view allocate device");
	auto sub2 = app.add_subcommand("debug", "debug a spiecial device");

    int venderid = 0,productid=0,interface_num=0,endpoint_num=0;	
	sub1_2->add_option("-v",venderid,"VenderID");
	sub1_2->add_option("-p",productid,"ProductID");
	
	sub2->add_option("-v",venderid,"VenderID");
	sub2->add_option("-p",productid,"ProductID");
	sub2->add_option("-i",interface_num,"Interface number");
	sub2->add_option("-e",endpoint_num,"Endpoint number");

    CLI11_PARSE(app, argc, argv);

	if (sub1_1->parsed())//ud view devices
    {
        view_devices();
    }
	if(sub1_2->parsed())//ud view device  -v 111 -p 222
	{
		view_device(venderid, productid);
	}

    //std::cout << "Parameter value: " << p << std::endl;
    return 0;
}

//main2 view device  -v 111 -p 222
