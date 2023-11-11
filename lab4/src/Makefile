CC = g++
CFLAGS = -std=c++11 -Wall -g

SRCS = banker.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = banker

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

banker.o: banker.cpp banker.h
	$(CC) $(CFLAGS) -c banker.cpp

main.o: main.cpp banker.h
	$(CC) $(CFLAGS) -c main.cpp

clean:
	rm -f $(OBJS) $(TARGET)