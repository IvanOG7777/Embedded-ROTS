//
// Created by elder on 3/28/2026.
//

#include <iostream>
#include <cstdint>
#include <bits/stdc++.h>

uint8_t BUFFER_SIZE = 26;

uint8_t *buffer;
uint8_t head = 0;
uint8_t tail = 0;
uint8_t bitArray[10];

uint8_t count = 0;

bool RX(uint8_t value) {
    if (count == BUFFER_SIZE) return false;

    std:: bitset<8> bitValue(value);
    bitValue.flip();

    return true;
}

//transmitter
uint8_t* TX(uint8_t value) {
    bitArray[0] = 0;
    bitArray[9] = 1;
    for (int i = 0; i < 8; i++) {
        bitArray[i+1] = ((value >> i) & 1);
    }
    return bitArray; // returns as LSB
}



int main() {

    uint8_t *array = TX(255);
    for (int i = 0; i < 10; i++) {
        std:: cout << static_cast<int>(array[i]);
    }
    std:: cout << std:: endl;
    std:: cout << std::bitset<8>(15);
    delete[] buffer;
    return 0;
}
