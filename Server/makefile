TARGET := server test

main: $(TARGET)

server: server.cpp myutil.cpp ../common/mypacket.cpp
	g++ -std=c++11 -o server server.cpp myutil.cpp ../common/mypacket.cpp -lrt

test:	test.cpp myutil.cpp ../common/mypacket.cpp
	g++ -std=c++11 -o test test.cpp myutil.cpp ../common/mypacket.cpp -lrt

clean :
	rm -f $(TARGET) *.o *.txt
	rm -rf serverout