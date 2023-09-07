# Define source and header directories
SRCDIR = src
INCDIR = include

# List source files
SRC = $(SRCDIR)/risc.c $(SRCDIR)/decoder.c $(SRCDIR)/encoder.c

# Compiler and flags
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