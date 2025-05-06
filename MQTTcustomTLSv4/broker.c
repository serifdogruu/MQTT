// mqtt_broker.c - minimal TLS MQTT broker in C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define PORT 8883
#define CERT_FILE "certs/server.crt"
#define KEY_FILE "certs/server.key"
#define CA_FILE "certs/ca.crt"

void init_openssl() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

SSL_CTX *create_context() {
    const SSL_METHOD *method = TLS_server_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    if (!ctx) {
        perror("SSL_CTX_new failed");
        exit(EXIT_FAILURE);
    }
    return ctx;
}

void configure_context(SSL_CTX *ctx) {
    if (SSL_CTX_use_certificate_file(ctx, CERT_FILE, SSL_FILETYPE_PEM) <= 0 ||
        SSL_CTX_use_PrivateKey_file(ctx, KEY_FILE, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    if (!SSL_CTX_load_verify_locations(ctx, CA_FILE, NULL)) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
}

void send_connack(SSL *ssl) {
    // MQTT CONNACK packet (Connection Accepted)
    unsigned char connack[] = { 0x20, 0x02, 0x00, 0x00 }; // CONNACK + 2-byte remaining length + flags + return code
    SSL_write(ssl, connack, sizeof(connack));
    printf("Sent CONNACK\n");
}

void handle_connection(SSL *ssl) {
    unsigned char buffer[1024] = {0};
    int bytes = SSL_read(ssl, buffer, sizeof(buffer));
    if (bytes <= 0) {
        ERR_print_errors_fp(stderr);
        return;
    }

    printf("Received %d bytes\n", bytes);
    if ((buffer[0] & 0xF0) == 0x10) {
        printf("CONNECT packet received\n");
        send_connack(ssl);
    } else {
        printf("Not a CONNECT packet\n");
    }
}

int main() {
    init_openssl();
    SSL_CTX *ctx = create_context();
    configure_context(ctx);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = INADDR_ANY
    };

    bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    listen(sock, 1);
    printf("MQTT TLS Broker running on port %d\n", PORT);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int client = accept(sock, (struct sockaddr*)&client_addr, &len);

        SSL *ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client);

        if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
        } else {
            handle_connection(ssl);
        }

        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(client);
    }

    close(sock);
    SSL_CTX_free(ctx);
    EVP_cleanup();
}

