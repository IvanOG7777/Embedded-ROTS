//
// Created by elder on 3/28/2026.
//

#include <iostream>
#include <bits/stdc++.h>
#include <thread>
#include <chrono>

uint8_t BUFFER_SIZE = 26;
constexpr uint16_t BAUD_RATE = 9600;
double BIT_PERIOD = 1.0 / BAUD_RATE;
double MID_POINT = BIT_PERIOD / 2;

uint8_t *buffer = new uint8_t[BUFFER_SIZE]{0};
uint8_t head = 0;
uint8_t tail = 0;
uint8_t bitArray[10];
uint8_t wire = 1;
uint8_t count = 0;


bool write(uint8_t value) {
    if (count == BUFFER_SIZE) return false;

    buffer[tail] = value;
    tail = (tail + 1) % BUFFER_SIZE;
    count++;
    return true;
}

bool read(uint8_t &readValue) {
    if (count == 0) return false;

    readValue = buffer[head];
    head = (head + 1) % BUFFER_SIZE;
    count--;
    return true;
}

// //transmitter
// uint8_t *TX(const uint8_t value) {
//     bitArray[0] = 0;
//     bitArray[9] = 1;
//     for (int i = 0; i < 8; i++) {
//         bitArray[i+1] = ((value >> i) & 00000001); // right shift value by i times, do & comparison with 1
//     }
//
//     for (uint8_t i : bitArray) {
//         wire = i;
//         std:: this_thread::sleep_for(std::chrono::duration<double>(BIT_PERIOD));
//     }
//     return &wire; // returns as LSB
// }

void STX(const uint8_t value) {
    bitArray[0] = 0;
    bitArray[9] = 1;
    for (int i = 0; i < 8; i++) {
        bitArray[i+1] = ((value >> i) & 00000001); // right shift value by i times, do & comparison with 1
    }


    for (uint8_t i = 0; i < 10; i++) {
        wire = bitArray[i];
        std:: cout << "Wire: " << static_cast<int>(wire) << std:: endl;
        std:: this_thread::sleep_for(std::chrono::duration<double>(BIT_PERIOD));
    }
}

void SRX() {
    while (wire == 1) {
        wire = 0;
        std:: cout << "Waiting on init 0 bit" << std:: endl;
    }
    std:: cout << "Seen starting 0 bit: " << std:: endl;
    std::this_thread::sleep_for(std::chrono::duration<double>(MID_POINT));
    uint8_t reassembledValue = 0;

    for (int i = 0; i < 8; i++) {
        std::this_thread::sleep_for(std::chrono::duration<double>(BIT_PERIOD));
        reassembledValue = reassembledValue  | (wire << i);
    }
    if (bitArray[9] != 1) {
        std:: cout << "BIT ERROR, WRONG ENDING BIT" << std:: endl;
        return;
    }
    wire = bitArray[9];
    write(reassembledValue);
}


// bool RX(const uint8_t *passedBitArray) {
//     if (count == BUFFER_SIZE) return false; // if buffer is already full reject
//     if (passedBitArray[0] != 0) return false; // makes use the start signal is 0
//     if (passedBitArray[9] != 1) return false;
//     uint8_t reassembledValue = 0;
//
//     for (int i = 0; i < 8; i++) {
//         reassembledValue = reassembledValue  | (passedBitArray[i + 1] << i);
//     }
//
//     return write(reassembledValue);
// }


int main() {

    STX(15);
    SRX();

    delete[] buffer;
    return 0;
}
