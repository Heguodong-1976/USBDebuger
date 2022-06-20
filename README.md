
# <center>说明文档</center>
## 生成
>### Windows X64 MSVC
>---
>><b>Build:</b>make -f msvc-x64 <br>
>><b>Clean</b>  make -f msvc-x64 CLEAN<br>
>>在msvc X64 命令行模式下运行 
>#### 生成目录
>><b>位置:</b>Out\WIN-X64<br>
>><b>ud.exe:</b>用来进行usb调试的命令行工具<br>
>><b>zadig.exe:</b>驱动转换工具，将WCI驱动转换为WinUSB驱动<br>
>><b>kyusbdebuger.h:</b>二次开发的头文件(C语言)<br>
>><b>kyusbdebuger.lib:</b>二次开发的库文件<br>
>---
#### 命令行
>><b>浏览所有的USB设备信息 : </b>ud.exe view devices<br>
>>>实例：<br>
>>>
>>|1|2|3|4|5|6|7|
>>|----|----|----|----|----|----|----|
>>|0|Vendor:5c8 |Product:3ec |Bus:1|Address:4|Path : .5 |      Failed libusb_open(...)==-5                          |
>>|1|Vendor:8087|Product:26  |Bus:1|Address:1|Path : .10|      Failed libusb_open(...)==-12                         |
>>|2|Vendor:27c6|Product:581a|Bus:1|Address:2|Path : .6 |      Failed libusb_open(...)==-3                          |
>>|3|Vendor:8086|Product:9a13|Bus:2|Address:0|Path :    |      Failed libusb_open(...)==fffffff4                    |
>>|4|Vendor:2717|Product:501e|Bus:1|Address:3|Path : .3 |      Success:0123456789AB:Mi Smart Mouse's Dongle:Actions |
>>|5|Vendor:8086|Product:a0ed|Bus:1|Address:0|Path :    |      Failed libusb_open(...)==fffffff4                    |
>><b>浏览特定的USB设备信息 : </b>ud.exe view device -|v venderid -p productid<br>
