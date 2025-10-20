EXEC = CSnake

CFLAGS = -Wall -Wextra -Wpedantic -Ilib
LDFLAGS = -lSDL3

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $(EXEC) $(LDFLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(EXEC)

re: fclean all
