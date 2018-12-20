TARGET := server

main: $(TARGET)

server: server.cpp myutil.cpp ../common/mypacket.cpp ../sql2/sql.cpp
	g++ -std=c++11 -o server server.cpp myutil.cpp ../common/mypacket.cpp  ../sql2/sql.cpp -L /usr/lib64/mysql -lmysqlclient -I /usr/include/mysql


clean :
	rm -f $(TARGET) *.o *.txt
	rm -rf serverout
