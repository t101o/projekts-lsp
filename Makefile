CC = gcc
CFLAGS = -Wall -Wextra --std=c99
LFLAGS = -lraylib -lm -lGL
OBJS = lsp-client.o lsp-server.o

.c.o :
	${CC} ${CFLAGS} -c $? -o $@ ${LFLAGS}

client : lsp-client.o 
	${CC} ${CFLAGS} -o $@ lsp-client.o ${LFLAGS}

server : lsp-server.o
	${CC} ${CFLAGS} -o $@ lsp-server.o -lm

clean :
	rm *.o
