//
// Created by elder on 3/29/2026.
//

#include <iostream>
#include <cstdint>

constexpr uint8_t BUFFER_SIZE = 26;
constexpr uint8_t BIT_ARRAY_SIZE = 10;

constexpr uint16_t BAUD_RATE = 9600;
constexpr double BIT_PERIOD = 1.0 / BAUD_RATE;
constexpr double MID_POINT = BIT_PERIOD / 2;

// buffer and buffer values to read/write
uint8_t *buffer = new uint8_t[BUFFER_SIZE]{0};
uint8_t head = 0;
uint8_t tail = 0;
uint8_t count = 0;

// communication values
uint8_t bitArray[BIT_ARRAY_SIZE];
volatile uint8_t wire = 1; // allows for up-to-date wire value for multipe threads to use
bool write(uint8_t value) {
    if (count == BUFFER_SIZE) return false;

    buffer[tail] = value;
    tail = (tail + 1) % BUFFER_SIZE;
    count++;

    return true;
}

bool read(uint8_t &value) {
    if (count == 0) return false;

    value = buffer[head];
    head = (head + 1) % BUFFER_SIZE;
    count--;

    return true;
}

void TX(uint8_t value) {
    if (wire == 0) return; // Wire not ready to transmit

    bitArray[0] = 0;
    bitArray[9] = 1;
    for (int i = 0; i < 8; i++) {
        bitArray[i+1] = ((value >> i) & 1);
    }

    for (int i = 0; i < BIT_ARRAY_SIZE; i++) {
        wire = bitArray[i];
        std:: cout << "Wire: " << static_cast<int>(wire) << std:: endl;
    }
}

void RX() {
    while (wire == 1) {

    }
}

int main() {

    TX(15);
    for (int i = 0; i < BIT_ARRAY_SIZE; i++) {
        std:: cout << static_cast<int>(bitArray[i]) << std:: endl;
    }
    return 0;
}
