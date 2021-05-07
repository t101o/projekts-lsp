CC = gcc
CFLAGS = -Wall -Wextra --std=99
LFLAGS = -lraylib -lm -lGL
OBJS =

.c.o :
	${CC} ${CFLAGS} -c $? -o $@ ${LFLAGS}

game : ${OBJS}
	${CC} ${CFLAGS} -o $@ ${OBJS} ${LFLAGS}

clean :
	rm *.o
