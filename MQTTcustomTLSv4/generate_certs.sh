#!/bin/bash
set -e
mkdir -p certs
cd certs

# === 1. Create CA (self-signed)
openssl genrsa -out ca.key 2048
openssl req -x509 -new -nodes -key ca.key -sha256 -days 3650 -out ca.crt \
    -subj "/C=US/ST=Test/L=Test/O=Test CA/OU=Dev/CN=Test CA"

# === 2. Create Server Key and CSR
openssl genrsa -out server.key 2048
openssl req -new -key server.key -out server.csr \
    -subj "/C=US/ST=Test/L=Test/O=Test Server/OU=Dev/CN=localhost"

# === 3. Sign Server CSR with CA
openssl x509 -req -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial \
    -out server.crt -days 1000 -sha256

# === 4. Create Client Key and CSR
openssl genrsa -out client.key 2048
openssl req -new -key client.key -out client.csr \
    -subj "/C=US/ST=Test/L=Test/O=Test Client/OU=Dev/CN=Test Client"

# === 5. Sign Client CSR with CA
openssl x509 -req -in client.csr -CA ca.crt -CAkey ca.key -CAcreateserial \
    -out client.crt -days 1000 -sha256

# === 6. Clean up CSRs
rm *.csr

echo "✅ Certificates generated in ./certs:"
ls -l

