CC = gcc -o
CFLAGS = -Wall -Wextra -Wpedantic -std=$(STD) -Wno-unused-variable -Wno-unused-parameter
STD= gnu11
RM = rm -f

TARGET = fork_var fork_open fork_print fork_exec fork_wait

.PHONY: all clean

all: $(TARGET)

clean:
	$(RM) $(TARGET) $(TARGET).o $(TARGET).output

fork_var: fork_var.c
	$(CC) fork_var fork_var.c $(CFLAGS)

fork_open: fork_open.c
	$(CC) fork_open fork_open.c $(CFLAGS)

fork_print: fork_print.c
	$(CC) fork_print fork_print.c $(CFLAGS)

fork_exec: fork_exec.c
	$(CC) fork_exec fork_exec.c $(CFLAGS)

fork_wait: fork_wait.c
	$(CC) fork_wait fork_wait.c $(CFLAGS)
