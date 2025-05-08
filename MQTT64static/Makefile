# Compiler settings
CC = gcc
CFLAGS = -I./include -Wall -O2
LDFLAGS = -L./lib -lpaho-mqtt3c -lssl -lcrypto -lpthread -static

# For async version, change to -lpaho-mqtt3a above

# Source files
CLIENT_SRC = mqtt_client.c
SERVER_SRC = mqtt_server.c

# Output binaries
CLIENT_BIN = mqtt_client
SERVER_BIN = mqtt_server

# Targets
all: $(CLIENT_BIN) $(SERVER_BIN)

$(CLIENT_BIN): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(SERVER_BIN): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(CLIENT_BIN) $(SERVER_BIN)

