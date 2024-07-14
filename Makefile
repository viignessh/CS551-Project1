CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

# Object files
OBJS = memory_manager.o bitmap.o test_main.o

# Static library
LIBRARY = memory_manager.a

# Header files
HEADERS = common.h interposition.h memory_manager.h

# Source files
SOURCES = memory_manager.c bitmap.c test_main.c

# Default target
all: $(LIBRARY) test_program

# Rule to create the static library
$(LIBRARY): $(OBJS)
	ar rcs $@ $^

# Rule to create object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to compile the test program
test: test_main.o $(LIBRARY)
	$(CC) $(CFLAGS) test_main.o -L. -l:$(LIBRARY) -o test_program

# Rule to create test_main.o
test_main.o: test_main.c $(HEADERS)
	$(CC) $(CFLAGS) -c test_main.c -o test_main.o

# Clean up the build files
clean:
	rm -f $(OBJS) $(LIBRARY) test_program

.PHONY: all clean
