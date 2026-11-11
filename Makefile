CC      = cc
CFLAGS  = -std=c99 -Wall -Wextra -pedantic -O2 -Iinclude

TARGET  = banker
SRCS    = src/main.c src/banker.c src/input.c src/output.c src/parser.c src/util.c
HEADERS = include/banker.h include/input.h include/output.h include/parser.h include/util.h
OBJS    = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
