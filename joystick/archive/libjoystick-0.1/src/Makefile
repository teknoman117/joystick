CC=gcc

OBJECTS= joystick.o
LIB=libjoystick.so
CFLAGS= -fPIC

all: library

clean:
	@echo "Cleaning up directory."
	rm -f *.o *.so *~ js_info

# Applications:
library: $(OBJECTS)
	$(CC) $(CFLAGS) -shared -Wl,-soname,$(LIB) -o $(LIB) $(OBJECTS)
	$(CC) $(CFLAGS) -L. -ljoystick js_info.c -o js_info 

install:
	cp $(LIB) /usr/lib
	cp joystick.h /usr/include
	cp js_info /usr/bin
