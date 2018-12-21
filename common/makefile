TARGET=test
.PHONY: all clean

all : test

test: test.cpp mypacket.cpp
	g++ test.cpp mypacket.cpp -o test

clean :
	rm -f *.o $(TARGET)
