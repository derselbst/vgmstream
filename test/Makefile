export SHELL = /bin/sh
export CFLAGS=-I../ext_includes -DUSE_ALLOCA $(OPTFLAGS)
# -DVGM_USE_G7221 -DVGM_USE_G719 
export LDFLAGS=-L../src -L ../ext_libs -lvgmstream -lvorbisfile -lvorbis -lmpg123  -lpthread -lm
#-lg719_decode -lg7221_decode -lsiren_decode
export CC=gcc
export AR=ar
export STRIP=strip

.PHONY: libvgmstream.a

test.exe: libvgmstream.a
	$(CC) $(CFLAGS) $(LDFLAGS) "-DVERSION=\"r1050-40-g33563f4\"" test.c $(LDFLAGS) -o vgmstream
# 	$(STRIP) vgmstream

libvgmstream.a:
	$(MAKE) -C ../src $@

libvgmstream.so:
	$(MAKE) -C ../src $@

clean:
	rm -f test test.o
	$(MAKE) -C ../src $@
