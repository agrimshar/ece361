# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -pedantic -std=c99

# Executable names
SERVER = server
CLIENT = deliver

# Source files
SERVER_SRC = server.c
CLIENT_SRC = deliver.c

# Build targets
all: $(SERVER) $(CLIENT)

# Rule to build the server
$(SERVER): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $(SERVER) $(SERVER_SRC)

# Rule to build the client
$(CLIENT): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $(CLIENT) $(CLIENT_SRC)

# Clean up build files
clean:
	rm -f $(SERVER) $(CLIENT)

# Phony targets
.PHONY: all clean

