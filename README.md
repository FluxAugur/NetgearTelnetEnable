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

`telnetenable2.exe 192.168.1.1 010203040506 admin password`
