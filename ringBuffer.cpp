#include <iostream>
#include <cstdint>

constexpr uint8_t BUFFER_SIZE = 10;

class RingBuffer {
public:
    uint8_t head;
    uint8_t tail;
    uint8_t count;
public:
    uint8_t buffer[BUFFER_SIZE] = {0};

public:
    RingBuffer() : head(0), tail(0), count(0){}

    bool isFull() {
        return count == BUFFER_SIZE;
    }

    bool isEmpty() {
        return count == 0;
    }

    // rejects on full new numbers;
    uint8_t write(uint8_t value) {
        if (isFull()) {
            std:: cout << "Buffer is full" << std:: endl;
            return 0;
        }; // signals that buffer is full

        buffer[tail] = value; // writes into the buffer
        tail = (tail + 1) % BUFFER_SIZE; // moves index plus 1, wraps back to 0 once tail is equal to BUFFER_SIZE
        count++;
        return 1; // success input
    }

    uint8_t overwrite(uint8_t value) {
        if (isFull()) {
            buffer[tail] = value;
            head = (head + 1) % BUFFER_SIZE;
            tail = (tail + 1) % BUFFER_SIZE;
        } else {
            buffer[tail] = value;
            tail = (tail + 1) % BUFFER_SIZE;
            count++;
        }
        return 1;
    }

    // essentially resets head, tail, count when reading from stream
    uint8_t read(uint8_t &value) {
        if (isEmpty()) {
            std:: cout << "Buffer is empty" << std:: endl;
            return 0;
        }

        value = buffer[head];
        head = (head + 1) % BUFFER_SIZE;
        count--;
        return 1;
    }

    uint8_t peekHead() {
        if (count == 0) return 0;
        return buffer[head];
    }

    uint8_t peekTail() {
        if (count == 0) return 0;
        return buffer[tail];
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

    RingBuffer ringBuffer;
    uint8_t value = 1;
    uint8_t readValue;


    while (ringBuffer.write(value++));
    std:: cout << "Available slots: " << static_cast<int>(ringBuffer.availableSlots()) << std:: endl;
    ringBuffer.overwrite(15);
    ringBuffer.overwrite(16);
    ringBuffer.overwrite(17);
    while (ringBuffer.read(readValue)) {
        std:: cout << static_cast<int>(readValue) << " ";
    }

    ringBuffer.overwrite(18);
    ringBuffer.overwrite(19);
    ringBuffer.overwrite(20);
    std:: cout << static_cast<int>(ringBuffer.buffer[0]) << std:: endl;
    std:: cout << static_cast<int>(ringBuffer.buffer[1]) << std:: endl;
    std:: cout << static_cast<int>(ringBuffer.buffer[2]) << std:: endl;

    std:: cout << "Head: " << static_cast<int>(ringBuffer.head) << ", value at head: " << static_cast<int>(ringBuffer.buffer[ringBuffer.head]) <<  std:: endl;
    std:: cout << "Tail: " << static_cast<int>(ringBuffer.tail) << ", value at head: " << static_cast<int>(ringBuffer.buffer[ringBuffer.tail]) <<  std:: endl;
    std:: cout << "Count: " << static_cast<int>(ringBuffer.count) << std:: endl;

    ringBuffer.flush();

    for (auto &val : ringBuffer.buffer) {
        std:: cout << static_cast<int>(val) << " ";
    }

    uint8_t newValue = 20;
    while (ringBuffer.write(newValue++));
    while (ringBuffer.read(readValue)) {
        std:: cout << static_cast<int>(readValue) << " ";
    }

    std:: cout << "Available slots: " << static_cast<int>(ringBuffer.availableSlots()) << std:: endl;
}