//
// Created by elder on 3/29/2026.
//

#include <atomic>
#include <iostream>
#include <cstdint>
#include <thread>

constexpr uint8_t BUFFER_SIZE = 5;
constexpr uint8_t BIT_ARRAY_SIZE = 10;

// constexpr uint16_t BAUD_RATE = 9600;
constexpr double BIT_PERIOD = 0.1;
constexpr double MID_POINT = BIT_PERIOD / 2;

// buffer and buffer values to read/write
uint8_t *buffer = new uint8_t[BUFFER_SIZE]{0};
uint8_t head = 0;
uint8_t tail = 0;
std::atomic<uint8_t> count = 0;

// communication values
uint8_t bitArray[BIT_ARRAY_SIZE];
std::atomic<uint8_t> wire = 1; // allows for up-to-date wire value for multiple threads to use


std::atomic<bool> write(uint8_t value) {
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
        std::this_thread::sleep_for(std::chrono::duration<double>(BIT_PERIOD));
        std:: cout << "Wire: " << static_cast<int>(wire) << " " << static_cast<int>(wire.load()) << std:: endl;
    }
}

void RX() {
    while (wire == 1) { // while the wire is in high state keep waiting
        std:: cout << "WAITING ON START BIT" << std:: endl;
        std::this_thread::sleep_for(std::chrono::duration<double>(MID_POINT));
    } // break once wire lowers
    std:: cout << "SEEN STARTING BIT" << std:: endl;
    uint8_t reassembledValue = 0;
    std::this_thread::sleep_for(std::chrono::duration<double>(MID_POINT));
    for (int i = 0; i < 8; i++) {
        std::this_thread::sleep_for(std::chrono::duration<double>(BIT_PERIOD));
        reassembledValue = reassembledValue | (wire << i);
    }

    write(reassembledValue);
}

int main() {

    uint8_t value = 1;
    uint8_t readValue = 0;
    while (value <= 5) {
        std::thread txThread(TX, value++);
        std::thread rxThread(RX);

        txThread.join();
        rxThread.join();
    }

    while (read(readValue)) {
        std:: cout << static_cast<int>(readValue) << " ";
    }
    return 0;
}
