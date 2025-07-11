CC = gcc
CFLAGS = -Wall -Wextra -pthread -g
LDFLAGS = -pthread

SRCS = main.c buffer.c
OBJS = $(SRCS:.c=.o)
TARGET = LogAnalyzer

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.c buffer.h
buffer.o: buffer.c buffer.h

clean:
	rm -f $(OBJS) $(TARGET)