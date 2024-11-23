EXEC = snake.out

CC = clang
CFLAGS = -I${INCLUDES} -lSDL2 -lm -w

INCLUDES = ./includes/
SRCS = ${wildcard ./srcs/*.c}
OBJS = ${SRCS:.c=.o} 

all: ${OBJS}
	${CC} ${OBJS} ${CFLAGS} -o ${EXEC}

clean:
	rm -f ${OBJS}

fclean: clean
	rm -f ${EXEC}

re: fclean all