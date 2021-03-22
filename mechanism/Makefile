CC = gcc -o
CFLAGS = -Wall -Wextra -Wpedantic -std=$(STD) -Wno-unused-variable -Wno-unused-parameter
STD = gnu11
RM = rm -f

TARGET =  syscall

.PHONY: all clean

all: $(TARGET)

clean:
	$(RM) $(TARGET) $(TARGET).o

syscall: syscall.c
	$(CC) syscall syscall.c $(CFLAGS)

