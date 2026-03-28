//
// Created by elder on 3/28/2026.
//

#include <iostream>
#include <cstdint>

template <typename T> class RingBuffer {
public:
    uint8_t head;
    uint8_t tail;
    uint8_t count;
    uint8_t BUFFER_SIZE;
    T *buffer;

    RingBuffer() : head(0), tail(0), count(0), BUFFER_SIZE(10) {
        buffer = new T[BUFFER_SIZE]{};
    }

    RingBuffer(uint8_t bufferSize) : head(0), tail(0), count(0), BUFFER_SIZE(bufferSize) {
        buffer = new T[BUFFER_SIZE]{};
    }

    ~RingBuffer() {
        delete[] buffer;
    }

    bool isEmpty() {
        return count == 0;
    }

    bool isFull() {
        return count == BUFFER_SIZE;
    }

    bool write(T value) {
        if (isFull()) {
            std:: cout << "Buffer is full" << std:: endl;
            return false;
        }

        buffer[tail] = value;
        tail = (tail + 1) % BUFFER_SIZE;
        count++;
        return true;
    }

    bool overwrite(T value) {
        if (isFull()) {
            buffer[tail] = value;
            tail = (tail + 1) % BUFFER_SIZE;
            head = (head + 1) % BUFFER_SIZE;
        } else {
            buffer[tail] = value;
            tail = (tail + 1) % BUFFER_SIZE;
            count++;
        }
        return true;
    }

    bool read(T &value) {
        if (isEmpty()) {
            std:: cout << "Buffer is empty" << std:: endl;
            return false;
        }

        value = buffer[head];
        head = (head + 1) % BUFFER_SIZE;
        count--;
        return true;
    }

    T peekHead() {
        return buffer[head];
    }

    void flush() {
        head = 0;
        tail = 0;
        count = 0;
    }

    uint8_t availableSlots() {
        return BUFFER_SIZE - count;
    }
};

int main() {

    RingBuffer<int> ringBuffer(26);
    int data = 1;
    int readValue;

    while (ringBuffer.write(data++));
    ringBuffer.overwrite(27);
    while (ringBuffer.read(readValue)) {
        std:: cout << readValue << " ";
    }

    while (ringBuffer.read(readValue)) {
        std:: cout << readValue << " ";
    }
}
