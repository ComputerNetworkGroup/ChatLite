TARGET := server test

RELY= ../common/mypacket.cpp ../common/common.cpp ../mysql/sql.cpp

main: $(TARGET)

server: server.cpp  $(RELY)
	g++ -std=c++11 -o server server.cpp  $(RELY) -L /usr/lib64/mysql -lmysqlclient -I /usr/include/mysql

test:	test.cpp myutil.cpp ../common/mypacket.cpp
	g++ -std=c++11 -o test test.cpp myutil.cpp ../common/mypacket.cpp -lrt

clean :
	rm -f $(TARGET) *.o *.txt
	rm -rf serverout
