//
// Created by elder on 3/27/2026.
//

#include <iostream>
#include <cstdint>


// Ring buffer class
class RingBuffer {
public:
    // index trackers to move along the buffer
    uint8_t head;
    uint8_t tail;
    uint8_t count;
    uint8_t BUFFER_SIZE;
    uint8_t *buffer;

    RingBuffer() : head(0), tail(0), count(0), BUFFER_SIZE(10) {
        buffer = new uint8_t[BUFFER_SIZE]{0};
    } // class constructor,

    RingBuffer(uint8_t passedBufferSize) : head(0), tail(0), count(0), buffer(nullptr) {
        buffer = new uint8_t[passedBufferSize]{0};
        BUFFER_SIZE = passedBufferSize;
    } // class constructor,

    ~RingBuffer() {
        delete []buffer;
    }

    void deleteBuffer() {
        std:: cout << "Buffer pointer array deleted" << std:: endl;
        delete[] buffer;
    }

    // function to check if buffer is full
    // no parameters
    bool isFull() {
        return count == BUFFER_SIZE;
    }

    // function to check if buffer is empty;
    // no parameters
    bool isEmpty() {
        return count == 0;
    }

    // function to write into the buffer, hard stops when buffer is full
    // Parameters: uint8_t
    uint8_t write(uint8_t value) {
        if (isFull()) { // check if buffer is full
            std:: cout << "Buffer is full" << std:: endl;
            return 0; // "failure to input"
        }; // signals that buffer is full

        buffer[tail] = value; // writes into the buffer at index tail
        tail = (tail + 1) % BUFFER_SIZE; // moves index plus 1, wraps back to 0 once tail is equal to BUFFER_SIZE
        count++;
        return 1; // successful input
    }

    // function that also writes into buffer, but doesn't care if buffer is full, overwrites previous data.
    // Parameters: uint8_t
    uint8_t overwrite(uint8_t value) {
        if (isFull()) { // checks if buffer is full
            buffer[tail] = value; // overwrites buffer at tail index
            // moves head and tail up an index
            head = (head + 1) % BUFFER_SIZE;
            tail = (tail + 1) % BUFFER_SIZE;
        } else { // if not full, treat as write()
            buffer[tail] = value;
            tail = (tail + 1) % BUFFER_SIZE;
            count++;
        }
        return 1;  // successful input
    }

    // function to read from buffer
    // Parameters: uint8_t
    uint8_t read(uint8_t &value) {
        if (isEmpty()) { // check if buffer is empty
            std:: cout << "Buffer is empty" << std:: endl;
            return 0;
        }

        // pass values from head index to value's address
        value = buffer[head];
        head = (head + 1) % BUFFER_SIZE; // move head up
        count--;
        return 1;
    }

    // function used to peal element at current head
    uint8_t peekHead() {
        if (isEmpty()) return 0; // returns early if buffer is empty
        return buffer[head];
    }

    uint8_t peekTail() {
        if (isEmpty()) return 0; // returns early if buffer is empty
        return buffer[tail]; // TODO fix this, doesn't return properly
    }

    // function to "flush" the buffer
    void flush() {
        // reset the indices back to front of buffer and reset count
        head = 0;
        tail = 0;
        count = 0;
    }

    // function used to show available slots in the buffer
    uint8_t availableSlots() {
        return BUFFER_SIZE - count;
    }
};



int main() {

    RingBuffer ringBuffer(10);
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

    uint8_t newValue = 20;
    while (ringBuffer.write(newValue++));
    while (ringBuffer.read(readValue)) {
        std:: cout << static_cast<int>(readValue) << " ";
    }

    std:: cout << "Available slots: " << static_cast<int>(ringBuffer.availableSlots()) << std:: endl;
}