# Source & header directories
SRCDIR = src
INCDIR = include

# Source files (wildcard pattern)
SRC = $(wildcard $(SRCDIR)/*.c)

# Compiler & flags
CC = gcc
CFLAGS = -std=c99 -Wall -I$(INCDIR)

# Linker flags
LDFLAGS = -ledit -lm

# Output binary
OUTPUT = risc

all: $(OUTPUT)

$(OUTPUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(OUTPUT)

clean:
	rm -f $(OUTPUT)
