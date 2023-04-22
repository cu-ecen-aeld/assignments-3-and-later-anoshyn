# Set the compiler based on the CROSS_COMPILE variable (default to gcc)
ifeq ($(CROSS_COMPILE),)
CC=gcc
else
CC=$(CROSS_COMPILE)gcc
endif

# Set the target name
TARGET=writer

# Set the source files
SRC=$(wildcard *.c)

# Set the object files
OBJ=$(SRC:.c=.o)

# Set compiler flags
CFLAGS=-Wall -Wextra

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)

