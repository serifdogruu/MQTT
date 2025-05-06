// client.c
#include "MQTTClient.h"
#include <stdio.h>
#include <string.h>

#define ADDRESS     "ssl://138.68.189.176:8883"
#define CLIENTID    "TestClient"
#define TOPIC       "test/topic"
#define PAYLOAD     "Hello MQTT TLS!"
#define QOS         1
#define TIMEOUT     10000L

int main() {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_SSLOptions ssl_opts = MQTTClient_SSLOptions_initializer;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    ssl_opts.trustStore = "certs/ca.crt";
    ssl_opts.keyStore = "certs/client.crt";
    ssl_opts.privateKey = "certs/client.key";
    ssl_opts.enableServerCertAuth = 1;

    conn_opts.ssl = &ssl_opts;

    int rc;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        return -1;
    }

    MQTTClient_publish(client, TOPIC, strlen(PAYLOAD), PAYLOAD, QOS, 0, NULL);
    printf("Published message over TLS\n");

    MQTTClient_disconnect(client, TIMEOUT);
    MQTTClient_destroy(&client);
    return 0;
}
