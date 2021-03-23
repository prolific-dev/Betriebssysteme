CC = gcc -o
CFLAGS = -Wall -Wextra -Wpedantic -std=$(STD) -Wno-unused-variable -Wno-unused-parameter
STD = gnu11
RM = rm -f

TARGET =  syscall context_switch context_switch_v1

.PHONY: all clean

all: $(TARGET)

clean:
	$(RM) $(TARGET) $(TARGET).o

syscall: syscall.c
	$(CC) syscall syscall.c $(CFLAGS)

context_switch: context_switch.c
	$(CC) context_switch context_switch.c $(CFLAGS)

context_switch_v1: context_switch_v1.c
	$(CC) context_switch_v1 context_switch_v1.c $(CFLAGS)
