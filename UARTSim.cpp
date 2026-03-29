//
// Created by elder on 3/28/2026.
//

#include <iostream>
#include <cstdint>
#include <bits/stdc++.h>

uint8_t BUFFER_SIZE = 26;
constexpr uint16_t BAUD_RATE = 9600;
double BIT_PERIOD = 1.0 / BAUD_RATE;
double MID_POINT = BIT_PERIOD / 2;

uint8_t *buffer = new uint8_t[BUFFER_SIZE]{0};
uint8_t head = 0;
uint8_t tail = 0;
uint8_t bitArray[10];

uint8_t count = 0;


bool write(uint8_t value) {
    if (count == BUFFER_SIZE) return false;

    buffer[tail] = value;
    tail = (tail + 1) % BUFFER_SIZE;
    count++;
    return true;
}

//transmitter
uint8_t* TX(uint8_t value) {
    bitArray[0] = 0;
    bitArray[9] = 1;
    for (int i = 0; i < 8; i++) {
        bitArray[i+1] = ((value >> i) & 00000001); // right shift value by i times, do & comparison with 1
    }
    return bitArray; // returns as LSB
}


bool RX(uint8_t *bitArray) {
    if (count == BUFFER_SIZE) return false; // if buffer is already full reject
    if (bitArray[0] != 0) return false; // makes use the start signal is 0
    uint8_t reassembledValue = 0;

    for (int i = 0; i < 8; i++) {
        reassembledValue = reassembledValue  | (bitArray[i + 1] << i);
    }

    return write(reassembledValue);
}


int main() {

    std:: cout << BIT_PERIOD << std:: endl;
    std:: cout << MID_POINT << std:: endl;

    for (int i = 0; i < BUFFER_SIZE; i++) {
        uint8_t *bits = TX(i + 1);
        RX(bits);
    }

    for (int i = 0; i < BUFFER_SIZE; i++) {
        std:: cout << static_cast<int>(buffer[i]) << " ";
    }

    delete[] buffer;
    return 0;
}
