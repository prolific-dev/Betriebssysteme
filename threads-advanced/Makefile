CFLAGS = -Wall -Wno-unused-variable -Wno-implicit-function-declaration -Wno-pointer-sign

all: tcp_server tcp_client tcp_server_select tcp_client_select tcp_server_file tcp_client_file tcp_server_async tcp_client_async

clean:
	rm -f tcp_server tcp_client tcp_server_select tcp_client_select tcp_server_file tcp_client_file tcp_server_async tcp_client_async

tcp_server: tcp_server.c
	gcc -o tcp_server tcp_server.c $(CFLAGS)

tcp_client: tcp_client.c
	gcc -o tcp_client tcp_client.c $(CFLAGS)

tcp_server_select: tcp_server_select.c
	gcc -o tcp_server_select tcp_server_select.c $(CFLAGS)

tcp_client_select: tcp_client_select.c
	gcc -o tcp_client_select tcp_client_select.c $(CFLAGS)

tcp_server_file: tcp_server_file.c
	gcc -o tcp_server_file tcp_server_file.c $(CFLAGS)

tcp_client_file: tcp_client_file.c
	gcc -o tcp_client_file tcp_client_file.c $(CFLAGS)

tcp_server_async: tcp_server_async.c
	gcc -o tcp_server_async tcp_server_async.c $(CFLAGS)

tcp_client_async: tcp_client_async.c
	gcc -o tcp_client_async tcp_client_async.c $(CFLAGS)
