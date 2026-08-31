# STM32 Water Tank Monitoring and Control System

An embedded control system developed around the **STM32F103C8** microcontroller for monitoring water level and temperature and controlling actuators using PWM.

The system combines ultrasonic distance measurement, analog temperature sensing, LCD and keypad interaction, real-time clock functionality, and UART communication in a single embedded application.

## Features

- Water level measurement using HC-SR04 ultrasonic sensor
- Temperature measurement using LM35 analog sensor
- PWM-based control of the water pump
- PWM-based control of the heating element
- 4×16 LCD user interface
- 4×4 keypad for runtime configuration
- Adjustable water-level setpoint
- RTC-based time display and configuration
- UART status messages
- Proteus simulation
- STM32 firmware implementation

## Hardware and Peripherals

| Component | Function |
|---|---|
| STM32F103C8 | Main microcontroller |
| HC-SR04 | Ultrasonic water-level sensing |
| LM35 | Temperature sensing |
| L298 | Motor driver |
| 4×16 LCD | System status display |
| 4×4 Keypad | User input and configuration |
| RTC | Real-time clock |
| UART | Status message transmission |

## System Operation

### Water Level Measurement

The HC-SR04 sensor is used to measure the distance between the sensor and the water surface.

The Echo pulse is measured using **Timer 2 Input Capture with interrupts**. The captured time difference is converted into distance in centimeters.

The measured distance is then used to calculate the control error relative to the selected setpoint.

### Pump Control

The water pump is controlled using a PWM signal.

The PWM duty cycle is adjusted according to the difference between the measured water level and the selected setpoint, providing proportional control of the pump.

### Temperature Measurement and Control

The LM35 sensor provides an analog voltage corresponding to temperature.

The STM32 ADC reads the sensor output and converts it into a temperature value. A PWM signal is then used to control the heating element according to the measured temperature.

The implemented control range is based on the project requirements, with a target temperature setpoint of 60°C.

### User Interface

The LCD displays:

- Current water-level distance
- Current setpoint
- Current temperature
- Current time

A 4×4 keypad allows the user to:

- Change the water-level setpoint
- Modify the RTC hour
- Modify the RTC minute
- Modify the RTC second

### UART Communication

After successful changes to the setpoint or RTC time, the system sends a confirmation message through UART.

## System Schematic

![System Schematic](images/circuit-overview.png)

The complete schematic is also available as a PDF in the `documentation` directory.

## Firmware

The firmware was developed in **C using the STM32 HAL library**.

The implementation uses:

- ADC
- Timers
- Input Capture
- PWM
- GPIO interrupts
- RTC
- UART
- LCD interface
- Keypad input

The firmware source code and compiled HEX file are available in the [`firmware`](firmware/) directory.

## Proteus Simulation

The complete system was simulated in **Proteus**.

The Proteus project file is available in the [`simulation`](simulation/) directory.

## Documentation

The project report includes the system architecture, sensor operation, control logic, menu design, and implementation details.

[View Project Report](documentation/project-report-fa.pdf)

## Project Structure

```text
STM32-Water-Tank-Control-System/
│
├── README.md
│
├── firmware/
│   ├── main.c
│   └── proje.hex
│
├── simulation/
│   └── proje.pdsprj
│
├── documentation/
│   └── project-report-fa.pdf
│
└── images/
    └── circuit-overview.png
