CC = gcc -o
CFLAGS = -Wall -Wextra -Wpedantic -std=$(STD) -Wno-unused-variable -Wno-unused-parameter
STD= gnu11
RM = rm -f

TARGET = fork_var fork_open fork_print fork_exec fork_wait fork_waitpid fork_close fork_pipe

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

fork_waitpid: fork_waitpid.c
	$(CC) fork_waitpid fork_waitpid.c $(CFLAGS)

fork_close: fork_close.c
	$(CC) fork_close fork_close.c $(CFLAGS)

fork_pipe: fork_pipe.c
	$(CC) fork_pipe fork_pipe.c $(CFLAGS)
