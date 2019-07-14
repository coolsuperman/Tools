all:SQL 
SQL:MySQL.hpp SQL.cc
	g++ $^ -o $@ -std=c++11 `mysql_config --cflags --libs` 
