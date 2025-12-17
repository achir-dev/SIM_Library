/**
 * SIM Writer Example
 * Demonstrates how to publish data using SIM transport
 */

#include <chrono>
#include <cstring>
#include <thread>
#include <cstdio>
#include "sim.hpp"

int main() {
    // Create writer with 1MB max buffer
    SIM::Writer writer("/example_channel", 1024 * 1024);
    
    if (!writer.init()) {
        printf("Failed to initialize writer\n");
        return 1;
    }
    
    printf("SIM Writer started. Publishing at 30 Hz...\n");
    
    uint64_t frame = 0;
    while (true) {
        // Create sample data
        char data[256];
        snprintf(data, sizeof(data), "Frame %lu - timestamp %ld", 
                 frame, std::chrono::system_clock::now().time_since_epoch().count());
        
        // Write data
        writer.write(data, strlen(data) + 1);
        
        printf("Published frame %lu\n", frame);
        ++frame;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(33));  // ~30 Hz
    }
    
    writer.destroy();
    return 0;
}
