TARGET = server  

main: $(TARGET) 

server: server.cpp myutil.cpp
	g++ server.cpp myutil.cpp -o server

clean :
	rm -f $(TARGET) *.o *.txt
	rm -rf serverout