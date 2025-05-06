CC = gcc
CFLAGS = -I./paho.mqtt.c/src -Wall

all: broker client

broker: broker.c
	gcc broker.c -o broker -lssl -lcrypto

client: client.c
	$(CC) client.c -o client $(CFLAGS) ./paho.mqtt.c/build/src/libpaho-mqtt3cs.a -lssl -lcrypto

clean:
	rm -f broker client

