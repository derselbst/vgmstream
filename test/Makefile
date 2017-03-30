export SHELL = /bin/sh
export CFLAGS=-Wall -g -fPIC -I../ext_includes -DUSE_ALLOCA  $(OPTFLAGS)
export LDFLAGS=-L../src -L ../ext_libs -lvgmstream -lvorbisfile -lvorbis -lmpg123 -lpthread -lm
#-lavutil -lavformat -lavcodec
export CC=gcc
export AR=ar
export STRIP=strip

.PHONY: libvgmstream.a libvgmstream.so

test.exe: libvgmstream.a
	$(CC) $(CFLAGS) $(LDFLAGS) "-DVERSION=\"`../version.sh`\"" test.c $(LDFLAGS) -o vgmstream

libvgmstream.a:
	$(MAKE) -C ../src $@
 
libvgmstream.so:
	$(MAKE) -C ../src $@

clean:
	$(MAKE) -C ../src $@
	rm -f test test.o
