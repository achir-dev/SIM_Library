/**
 * SIM Reader Example
 * Demonstrates how to receive data using SIM transport
 */

#include <chrono>
#include <thread>
#include <cstdio>
#include "sim.hpp"

int main() {
    // Create reader with same max buffer as writer
    SIM::Reader reader("/example_channel", 1024 * 1024);
    
    printf("Waiting for writer...\n");
    
    while (!reader.init()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    printf("SIM Reader connected. Receiving data...\n");
    
    uint64_t count = 0;
    while (true) {
        size_t size = 0;
        
        // Zero-copy read - get direct pointer to shared memory
        const void* data = reader.readZeroCopy(size);
        
        if (data && size > 0) {
            printf("Received [%zu bytes]: %s\n", size, (const char*)data);
            ++count;
        }
        
        // Check if writer is still alive
        if (!reader.isWriterAlive(2000)) {
            printf("Writer disconnected. Received %lu messages.\n", count);
            break;
        }
        
        std::this_thread::yield();
    }
    
    return 0;
}
