//
// Created by elder on 3/28/2026.
//

#include <iostream>
#include <cstdint>
#include <thread>

constexpr uint8_t BUFFER_SIZE = 26;
constexpr uint8_t BIT_ARRAY_SIZE = 10;

uint8_t *buffer = new uint8_t[BUFFER_SIZE]{0};
uint8_t head = 0;
uint8_t tail = 0;
volatile uint8_t count = 0;

uint8_t bitArray[BIT_ARRAY_SIZE];


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

//transmitter function
// pass in unsinged 8-bit number
// function will strip bits from passed number into a bit array, adding start/stop buffer bits
uint8_t *TX(const uint8_t value) {
    bitArray[0] = 0;
    bitArray[9] = 1;
    for (int i = 0; i < 8; i++) {
        bitArray[i+1] = ((value >> i) & 00000001); // right shift value by i times, do & comparison with 1
    }

    return bitArray; // returns as full 10 bit array as LSB
} // technically it isn't "transmitting" like real UART, since it's only supposed to send bits through a wire

//receiver function
// pass in pointer to the bit array TX sends out
bool RX(const uint8_t *passedBitArray) {
    if (count == BUFFER_SIZE) return false; // if buffer is already full reject
    if (passedBitArray[0] != 0) return false; // makes use the start signal is 0
    if (passedBitArray[9] != 1) return false; // makes sure the stop signal is 1
    uint8_t reassembledValue = 0; // dummy value used to reshift bits into

    // loop though the byte within the bit array
    //technically this part is also not true to UART
    for (int i = 0; i < 8; i++) {
        reassembledValue = reassembledValue  | (passedBitArray[i + 1] << i);
    }

    return write(reassembledValue); // try to write the reconstructed value into the buffer, will return T OR F
} // technically it isn't "receiving" like real UART, since it's only supposed to read one bit at a time through a "wire" send from TX


int main() {

    uint8_t readValue = 0;

    // fake simulation
    while (count != BUFFER_SIZE) {
        auto bits = TX(readValue++);
        RX(bits);
    }

    while (read(readValue)) {
        std::cout << "Buffer value: " << static_cast<int>(readValue) << std::endl;
    }

    delete[] buffer;
    return 0;
}
