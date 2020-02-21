telnetenable2 : blowfish.c blowfish.h md5.c md5.h telnetenable.c
	cc -o telnetenable2 telnetenable.c blowfish.c md5.c

clean :
	rm telnetenable2
