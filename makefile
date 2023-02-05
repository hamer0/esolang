CC = gcc
CFLAGS = -c -std=c99 -g -I include
LDFLAGS = -g
SRC = $(wildcard src/*.c)
HDR = $(wildcard include/*.h)
OBJ = $(SRC:.c=.o)
EXEC = zxb1

all: $(SRC) $(OBJ) $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c $(HDR)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

re: clean all
