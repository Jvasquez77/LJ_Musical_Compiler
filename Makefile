CXX = g++
CXXFLAGS = -Wall -std=c++17 -I.
LDFLAGS = 

SRCS = main.cpp declaration.cpp expression.cpp statement.cpp ast_node_interface.cpp symbol_table.cpp
OBJS = $(SRCS:.cpp=.o) scanner.o token.o

TARGET = compilador_musical

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

token.cpp token.hpp: parser.bison
	bison -d -o token.cpp parser.bison

scanner.cpp: scanner.flex token.hpp
	flex -o scanner.cpp scanner.flex

scanner.o: scanner.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

token.o: token.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET) token.cpp token.hpp scanner.cpp *.abc

run: $(TARGET)
	./$(TARGET) test.mus

.PHONY: all clean run 