# SIM Library v1.0.0

Ultra-low latency shared memory transport for robotics and autonomous systems.

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

---

## ⚠️ Data Disclaimer

> **IMPORTANT:** Benchmark results may contain measurement errors. Data was collected using custom scripts with embedded timestamps—**FastDDS Monitor was NOT used** for the latest tests. Results should be considered indicative. Hardware variations and system load affect actual performance.

**Methodology:** End-to-end latency from `high_resolution_clock`, 12-15s tests at 30Hz, median values.

---

## Overview

**SIM (Sensor-In-Memory)** is a lightweight C++17 library for inter-process communication using shared memory. It provides zero-copy data transfer with minimal overhead.

### Key Features

- **Zero-copy reads** - Reader gets direct pointer to shared memory
- **Lock-free** - Uses atomic operations, no mutexes
- **Double buffering** - Writer and reader never block each other
- **Cache-line aligned** - Optimized for modern CPU architectures
- **Simple API** - Only a few functions to learn

---

## Performance Summary (Benchmark V2 - December 2025)

### Single Subscriber (Median Latency in ms)

| Size | SIM | ROS2 Standard | ROS2 Loaned | vs ROS2 |
|------|-----|---------------|-------------|---------|
| 1KB | 0.0010 | 0.61 | 0.59 | ~600x faster |
| 100KB | 0.0160 | 0.73 | 0.72 | ~45x faster |
| 1MB | 0.136 | 1.56 | 1.34 | ~10x faster |
| 10MB | 1.65 | 8.78 | 9.26 | ~5x faster |
| 50MB | 9.45 | 360 | 544 | ~40x faster |

### Multi-Subscriber Scaling (1MB)

| Subscribers | SIM | ROS2 Standard |
|-------------|-----|---------------|
| 1 | 0.136 ms | 1.56 ms |
| 3 | 0.157 ms | 1.47 ms |
| 5 | 0.164 ms | 1.59 ms |

**SIM: stable latency even with multiple subscribers!**

---

## Quick Start

### Installation

```bash
mkdir build && cd build
cmake ..
make
sudo make install
```

### Basic Usage

```cpp
#include "sim.hpp"

// Writer process
SIM::Writer writer("/sensor_data", 1024);
writer.init();
writer.write(data, size);

// Reader process  
SIM::Reader reader("/sensor_data", 1024);
reader.init();
size_t size;
const void* ptr = reader.readZeroCopy(size);  // Direct pointer!
```

---

## API Reference

### Writer

```cpp
SIM::Writer writer(channel_name, max_size);
writer.init();                    // Initialize shared memory
writer.write(data, size);         // Write data
writer.destroy();                 // Cleanup
```

### Reader

```cpp
SIM::Reader reader(channel_name, max_size);
reader.init();                    // Connect to shared memory
const void* ptr = reader.readZeroCopy(size);  // Zero-copy read
bool alive = reader.isWriterAlive(timeout_ms);
```

---

## Architecture

```
┌─────────────────────────────────────────────────┐
│  SHARED MEMORY (/dev/shm/sim_channel)           │
├─────────────────────────────────────────────────┤
│  Header (cache-line aligned)                    │
│  - atomic<front_index>                          │
│  - sequence, heartbeat                          │
├─────────────────────────────────────────────────┤
│  Buffer A ← Writer writes here                  │
├─────────────────────────────────────────────────┤
│  Buffer B ← Reader reads here                   │
└─────────────────────────────────────────────────┘
```

---

## Requirements

- C++17 (GCC 7+, Clang 5+)
- Linux with POSIX shared memory
- pthreads, rt libraries

---

## Files

```
SIM_Publish/
├── include/sim.hpp     # Header
├── src/sim.cpp         # Implementation
├── examples/           # Example code
├── CMakeLists.txt      # Build
├── README.md           # This file
├── LICENSE             # MIT License
└── ACKNOWLEDGMENTS.md  # Credits
```

---

## Credits

Based on: **"A Faster and More Reliable Middleware for Autonomous Driving Systems"**  
Yuankai He, Weisong Shi - University of Delaware

> **Note:** This is an independent implementation by Nabil Achir, NOT affiliated with the original authors.

See [ACKNOWLEDGMENTS.md](ACKNOWLEDGMENTS.md) for full credits.

---

**Author:** Nabil Achir | **Version:** 1.0.0 | **December 2025**
