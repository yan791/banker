CC      = cc
CFLAGS  = -std=c99 -Wall -Wextra -pedantic -O2 -Iinclude

TARGET  = banker
SRCS    = src/main.c src/util.c src/parser.c src/banker.c src/input.c
HEADERS = include/util.h include/parser.h include/banker.h include/input.h
OBJS    = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean