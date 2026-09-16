# STM32 Water Tank Control System

## Intelligent Embedded Control System for Water Tank Monitoring and Automation

This project presents an embedded water tank monitoring and control system based on the **STM32F103C8 microcontroller**.

The system integrates multiple sensors, actuators, and user interfaces to automatically monitor water level and temperature, control a DC pump and heating element, and provide real-time feedback through an LCD interface.

The complete system was designed, implemented, and validated through **Proteus simulation**, with embedded firmware developed in **C using STM32 HAL libraries**.

---

## Project Objectives

The main objective of this project is to design an automated control system capable of:

- Monitoring water level in real time
- Measuring temperature conditions
- Automatically controlling water pumping
- Regulating heater power using PWM
- Providing user interaction through keypad input
- Displaying system parameters on an LCD
- Managing system time using RTC
- Communicating system status through UART

---

# System Features

### Water Level Monitoring
- Ultrasonic distance measurement using **HC-SR04 sensor**
- Real-time calculation of tank water level
- Automated pump control based on predefined conditions

### Temperature Monitoring and Control
- Temperature sensing using **LM35 analog sensor**
- Heater control using PWM-based power regulation
- Temperature parameter adjustment through user interface

### Motor and Actuator Control
- DC pump control using **L298 motor driver**
- PWM-based speed regulation
- Automated water circulation management

### User Interface
- LCD-based real-time monitoring
- Keypad menu for parameter configuration
- User-adjustable control thresholds

### Time and Communication
- RTC module for time management
- UART communication for system status monitoring

---

# Hardware Components

| Component | Purpose |
|---|---|
| STM32F103C8 | Main microcontroller and control unit |
| HC-SR04 | Ultrasonic water level measurement |
| LM35 | Temperature sensing |
| L298 | DC motor driver |
| DC Motor | Water pump actuator |
| Heater Resistance | Temperature control actuator |
| LM041L LCD | Display interface |
| Keypad | User input |
| RTC Module | Time management |

---

# System Architecture

```
              +----------------+
              |    HC-SR04     |
              | Water Level    |
              |    Sensor      |
              +-------+--------+
                      |
                      |
                      v

+-------------+   +----------------+   +-------------+
|   LM35      |-->|                |-->|   LCD       |
| Temperature |   |    STM32       |   | Display     |
|   Sensor    |   |   F103C8       |   +-------------+
+-------------+   |                |
                  |                |
                  +-------+--------+
                          |
          +---------------+---------------+
          |                               |
          v                               v
    +-----------+                   +-------------+
    |  L298     |                   |   Heater    |
    | Motor     |                   |   Control   |
    | Driver    |                   |   PWM       |
    +-----------+                   +-------------+
          |
          v
      Water Pump
```

---

# Software Implementation

## Development Environment

- Programming Language: **Embedded C**
- Microcontroller: **STM32F103C8**
- Framework: **STM32 HAL Library**
- Simulation Platform: **Proteus**
- IDE Compatibility: STM32CubeIDE / Keil

---

# Firmware Overview

The firmware is responsible for:

- Initializing STM32 peripherals
- Reading sensor values
- Processing control logic
- Generating PWM signals
- Updating LCD information
- Handling keypad input
- Managing RTC functionality
- Sending status information through UART

Main peripherals used:

- ADC
- Timers
- PWM
- GPIO
- UART
- RTC

---

# Simulation

The complete circuit was simulated using Proteus.

Included in this repository:

- Proteus simulation project
- Circuit design
- Compiled HEX firmware
- Simulation screenshots
- Demonstration video

---

# Repository Structure

```
STM32-Water-Tank-Control-System
│
├── firmware
│   ├── main.c
│   └── proje.hex
│
├── simulation
│   └── Proteus project files
│
├── images
│   ├── circuit-overview.png
│   └── proteus.png
│
├── video
│   └── demonstration video
│
└── documentation
    └── project report
```

---

# Demonstration

The simulation demonstrates:

- Water level detection
- Automatic pump operation
- Temperature monitoring
- Heater control
- LCD status display
- User parameter adjustment

---

# Skills Demonstrated

This project demonstrates practical experience in:

- Embedded Systems Design
- STM32 Microcontroller Programming
- Sensor Integration
- Peripheral Configuration
- PWM Control Techniques
- Digital Control Systems
- Hardware Simulation
- Firmware Development

---

# Future Improvements

Possible improvements:

- Adding IoT connectivity using ESP32/Wi-Fi module
- Cloud-based monitoring dashboard
- Real hardware implementation
- Advanced control algorithms (PID/Fuzzy Control)
- Mobile application interface

---

# Author

**Arghavan Memari**

Embedded Systems | Microcontroller Programming | IoT Applications
