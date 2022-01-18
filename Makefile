EXE=xc
CC=gcc
SRC=src/*.c
STD=-std=c99
OPT=-O2
WFLAGS=-Wall -Wextra
INC=-I.
CFLAGS=$(STD) $(OPT) $(WFLAGS) $(INC)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

clean: $(EXE)
	rm $^
