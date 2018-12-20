CC = g++
.PHONY : build clean

build : sql
sql : sql.cpp sql_test.cpp
	$(CC) `mysql_config --include` `mysql_config --libs` $^ -o $@

clean:
	rm -f sql
