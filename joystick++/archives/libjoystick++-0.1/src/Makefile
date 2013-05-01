CC=g++

OBJECTS=joystick.o
LIB=libjoystick++.so
CPPFLAGS=-fPIC

all: library

clean:
	@echo "Cleaning up directory."
	rm -f *.o *.so *~ js_info

# Applications:
library: $(OBJECTS)
	$(CC) $(CPPFLAGS) -shared -Wl,-soname,$(LIB) -o $(LIB) $(OBJECTS)
	$(CC) $(CPPFLAGS) -L. -ljoystick++ js_info.cpp -o js_info 

install:
	cp $(LIB) /usr/lib
	cp joystick.h /usr/include
	cp js_info /usr/bin
