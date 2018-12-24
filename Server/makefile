TARGET := server

RELY= ../common/mypacket.cpp ../common/common.cpp ../mysql/sql.cpp ../XML/lib/tinyxml.cpp ../XML/lib/tinystr.cpp ../XML/lib/tinyxmlerror.cpp ../XML/lib/tinyxmlparser.cpp ../XML/XmlLog.cpp
main: $(TARGET)

server: server.cpp  $(RELY)
	g++ -std=c++11 -o server server.cpp  $(RELY) -L /usr/lib64/mysql -lmysqlclient -I /usr/include/mysql


clean :
	rm -f $(TARGET) *.o *.txt
	rm -rf serverout
