EXEC = Snake

CFLAGS = -Wall -Wextra -Wpedantic
LDFLAGS = -lSDL2

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
