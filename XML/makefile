TARGET := readXmlTest writeXmlTest
LIB := ./lib/tinyxml.cpp ./lib/tinystr.cpp ./lib/tinyxmlerror.cpp ./lib/tinyxmlparser.cpp

main: $(TARGET)

readXmlTest: xmlParser.cpp $(LIB)
	g++ -std=c++11 -o $@ $@.cpp $(LIB) -lrt

writeXmlTest: xmlParser.cpp $(LIB)
	g++ -std=c++11 -o $@ $@.cpp $(LIB) -lrt

clean :
	rm -f $(TARGET)