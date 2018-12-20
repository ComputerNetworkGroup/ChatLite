TARGET := server

main: $(TARGET)

server: server.cpp myutil.cpp ../common/common.cpp
	g++ -std=c++11 -o server server.cpp myutil.cpp ../common/common.cpp -lrt


clean :
	rm -f $(TARGET) *.o *.txt
	rm -rf serverout