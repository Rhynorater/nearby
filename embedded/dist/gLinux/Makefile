CC = gcc
CFLAGS = -Wall -g -MD -Werror -Wno-deprecated-declarations -DARCH_GLINUX -fsanitize=address -fno-omit-frame-pointer -DNEARBY_ALL_MODULE_DEBUG -DNEARBY_UNIT_TEST_ENABLED -I.
LDFLAGS = -L. -lasan

all: demo

demo: demo.c libnearby.a
	$(CC) $(CFLAGS) -o demo demo.c $(LDFLAGS) -lnearby -lpthread

clean:
	rm -f demo 