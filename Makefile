CC      = cc
CFLAGS  = -std=c99 -Wall -Wextra -pedantic -O2

TARGET  = banker
SRCS    = main.c banker.c input.c output.c parser.c util.c
HEADERS = banker.h input.h output.h parser.h util.h
OBJS    = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
