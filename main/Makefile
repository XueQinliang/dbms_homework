
CFLAGS = -std=c++1z -lstdc++ -I../src/ -L../

all:
	$(CXX) $(CFLAGS) main.cpp -o main -lsqlparser
	$(CXX) $(CFLAGS) sqlclient.cpp -o sqlclient -lpthread -lsqlparser
	$(CXX) $(CFLAGS) sqlserver.cpp -o sqlserver -lpthread -lsqlparser
	$(CXX) $(CFLAGS) gui.cpp -o gui -lpthread -lsqlparser

