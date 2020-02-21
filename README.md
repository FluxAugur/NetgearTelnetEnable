# NetgearTelnetEnable

**NetgearTelnetEnable is a tool for enables telnet access on certain Netgear devices**

- This code was test pass on Windows7
- It may be use on Linux

**Compile:**

`gcc -W -Wall -O2 -fno-strict-aliasing blowfish.c md5.c telnetenable.c -o telnetenable2`

`gcc -W -Wall -O2 -fno-strict-aliasing blowfish.c md5.c telnetenable.c -o telnetenable2 -lws2_32`

**Usage:**

`telnetenable2.exe <IP> <MAC> <user> <pass>`

```
              IP - the IP of the LAN of the router
             MAC - the MAC address of the LAN of the router
            user - usually admin
        password - the admin password (the one used to login into the WEB configuration pages)
```

**Example:**

`telnetenable2.exe 192.168.1.1 12345678EEFF admin password`



# Original Information
- Modified to work with newer Netgear routers R7000 R7500 by insanid
- Windows telnetenable-win.c and binary by LuKePicci
==================================================================

Based on https://github.com/davejagoda/NetgearTelnetEnable
For more information: http://wiki.openwrt.org/toh/netgear/telnet.console

Enables telnet access with newer Netgear routers R7000 and R7500
Should enable telnet on other Netgear routers that accept payload over UDP

**Binaries:**
Binaries are available for Linux x86-64 and Windows. They are located in the binaries section of this github repository:
https://github.com/insanid/NetgearTelnetEnable/tree/master/binaries/

**Linux Users:**
Download binary
chmod a+x telnetenable
Refer to usage instructions below.


**Windows Users:**
Download telnetenable.exe
Refer to usage instructions below.


**Linux Usage:**
./telnetenable <IP> <MAC> <Username> <Password>


**Windows Usage:**
telnetenable.exe <IP> <MAC> <Username> <Password>

IP - The IP of your Netgear device, usually 192.168.1.1

MAC - The mac address should be the MAC address of the LAN port on your Netgear device, WITHOUT the ":". e.g. "00:40:5E:21:14:4E" would be written as "00405E21144E".



**For newer Netgear routers:**
Username - 'admin'
Password = Use password you set in web interface


**Linux Build Instructions:**
`git clone https://github.com/insanid/NetgearTelnetEnable.git`
`gcc -o telnetenable md5.c blowfish.c telnetenable.c`


**Windows Build Instructions:**
Coming soon!

Comments from original README.md:
This code did not quite work for me on OS X, but it was OK on Debian:

[http://www.seattlewireless.net/telnetenable.c]

I've made changes so it works on OS X 10.7.4.  I've also added some comments to clarify what was wrong.
