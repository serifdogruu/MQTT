cmake_minimum_required(VERSION 3.15)
project(MQTT_TLS_STATIC C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Set Paho location
set(PAHO_PREFIX ${CMAKE_SOURCE_DIR}/paho.mqtt.c/install)
set(PAHO_INCLUDE_DIR ${PAHO_PREFIX}/include)
set(PAHO_LIB_DIR ${PAHO_PREFIX}/lib)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Include and link paths
include_directories(${PAHO_INCLUDE_DIR})
link_directories(${PAHO_LIB_DIR})

# Define executable targets
add_executable(mqtt_client mqtt_client.c)
add_executable(mqtt_server mqtt_server.c)

# Link statically with Paho and OpenSSL
target_link_libraries(mqtt_client
    ${PAHO_LIB_DIR}/libpaho-mqtt3cs-static.a
    OpenSSL::SSL
    OpenSSL::Crypto
    pthread
)

target_link_libraries(mqtt_server
    ${PAHO_LIB_DIR}/libpaho-mqtt3cs-static.a
    OpenSSL::SSL
    OpenSSL::Crypto
    pthread
)

