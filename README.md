# Register-Level STM32F407 Weather Station

A bare-metal, register-level C implementation of a real-time weather station. This project interfaces an **STM32F407G-DISC1** board with a **DHT11** temperature/humidity sensor and displays the live readings on a **2.8" ILI9341 TFT display** over SPI.

To gain a deep, fundamental understanding of the hardware, **no HAL, LL, or third-party middleware libraries were used**. Every driver—from basic GPIO up to the display's font rendering engine—was written entirely from scratch using direct register manipulation.

---

## 🛠 Hardware Setup

* **Microcontroller:** STM32F407G-DISC1 (ARM Cortex-M4 @ 168MHz)
* **Sensor:** DHT11 Temperature & Humidity Sensor (Single-wire timing protocol)
* **Display:** 2.8" ILI9341 TFT LCD (240x320 resolution, SPI interface)
* **Debugging Tools:** Logic Analyzer (for signal verification and protocol timing analysis)

---

## 🔬 Core Architectural Details & Drivers

### 1. Custom DHT11 Timing Driver

The DHT11 utilizes a proprietary single-wire protocol where bit values are encoded based on the duration of high-level pulses in microseconds.

* **Implementation:** Written using direct hardware timer register configurations.
* **The Prescaler Shadow Register Hurdle:** During development, I discovered a subtle STM32 timer behavior: the Prescaler Register (`TIMx_PSC`) is buffered. Writing directly to the register does not apply the division factor immediately. Instead, it waits for an update event, which meant the timer was initially scaling all microsecond timing measurements by 16x.
* **The Solution:** I resolved this by manually forcing an Update Generation event—setting the `UG` bit in the `TIMx_EGR` register—to immediately load the prescaler's shadow register.

### 2. Full SPI & TFT Display Driver (ILI9341)

The TFT screen is driven by a custom-written SPI driver configured entirely at the register level.

* **GPIO Configurations:** Hand-configured GPIO pins to their respective Alternate Function (AF5) modes for hardware SPI1.
* **NSS & The Master Mode Fault (MODF):** Debugged a silent SPI hardware lockup. By digging into the SPI status registers, I identified a Master Mode Fault (`MODF`). When the SPI is configured as a master, the hardware expects the Slave Select (`NSS`) pin to be driven high. If left unconfigured, the peripheral assumes another master has claimed the bus and disables itself. This was fixed by enabling Software Slave Management (`SSM`) and setting the Internal Slave Select (`SSI`) bit to force the master state.

### 3. Custom Graphics Pipeline

Without the convenience of an existing graphics framework, the entire rendering pipeline was built from scratch:

* **Font Renderer:** Implemented a lightweight, custom 5x7 bitmap font engine that translates character arrays into coordinate-mapped pixel blocks on the screen.
* **Flicker-Free UI Refresh:** To avoid the harsh, periodic screen flickering common in basic display implementations, the driver utilizes active state tracking. It compares new DHT11 readings against the current display buffer, only redrawing the specific bounding boxes containing the changing numerical values (e.g., updating only `29.5` instead of clearing the whole screen).

---

## 📁 Repository Structure

```text
├── Inc/                  # Header files (Register definitions, driver configs)
│   ├── gpio.h            # Bare-metal GPIO register offsets
│   ├── spi.h             # SPI peripheral register mappings
│   ├── timer.h           # Hardware timer configurations
│   ├── dht11.h           # DHT11 timing protocol definitions
│   └── ili9341.h         # TFT screen commands and font maps
├── Src/                  # Source files
│   ├── main.c            # Application entry & main execution loop
│   ├── gpio.c            # GPIO initialization & alternate functions
│   ├── spi.c             # Register-level SPI transmit/receive routines
│   ├── timer.c           # Hardware microsecond delay & measurement 
│   ├── dht11.c           # DHT11 data capture state-machine
│   └── ili9341.c         # TFT controller commands & font renderer
└── README.md             # Project documentation

```

---

## 📈 Lessons Learned

* **Hardware Verifies Code:** Working with bare-metal C reinforces that the datasheet is the ultimate single source of truth. Software debugging can only take you so far—having a logic analyzer connected to the SPI and single-wire bus lines was invaluable for confirming the exact edge transitions and timing limits.
* **Respect the Shadow Registers:** Microcontroller peripherals often utilize dual-buffered registers to prevent glitches during active operations. Understanding when and how changes propagate to active hardware (e.g., trigger events, buffering) is critical for precise timing control.
