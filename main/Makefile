
CFLAGS = -std=c++1z -lstdc++ -Wall -Werror -I../src/ -L../

all:
	$(CXX) $(CFLAGS) main.cpp -o main -lsqlparser
	$(CXX) $(CFLAGS) sqlclient.cpp -o sqlclient -lpthread -lsqlparser
	$(CXX) $(CFLAGS) sqlserver.cpp -o sqlserver -lpthread -lsqlparser

