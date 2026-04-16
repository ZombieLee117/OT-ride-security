# OT Ride Security Simulation

Simulated theme park ride control system demonstrating OT (Operational Technology) concepts, industrial protocols, and safety-focused cybersecurity scenarios.

---

## Overview

This project simulates a real-world industrial control system (ICS) environment using a microcontroller-based ride controller and Modbus TCP communication.

The goal is to demonstrate how network-level control commands can influence physical systems, and how proper safety logic prevents unsafe or unauthorized operations.

---

## Key Features

- Physical ride control system using Arduino  
- Start, Stop, Emergency Stop, and Fault Reset functionality  
- LCD display for system state visibility  
- Status indicators (green = running, red = fault)  
- Modbus TCP simulation using QModMaster and Modbus Slave  
- Network traffic analysis using Wireshark  
- Control integrity attack simulation and mitigation  

---

## System Architecture

    Modbus Master (QModMaster)
            ↓
    Modbus Slave (Simulated PLC)
            ↓
    Wireshark (Traffic Monitoring)
            ↓
    Serial Bridge (PowerShell)
            ↓
    Arduino Ride Controller
            ↓
    Physical Outputs (LEDs + LCD)

---

## Hardware Components

- Arduino Uno  
- Breadboard  
- 16x2 LCD Display  
- Push Buttons:
  - Start  
  - Emergency Stop  
  - Fault Reset  
- LEDs:
  - Green (Running)  
  - Red (Fault)  
- Resistors and wiring  

---

## Software & Tools

- Arduino IDE  
- QModMaster (Modbus TCP Master)  
- Modbus Slave (PLC Simulation)  
- Wireshark (Network Analysis)  
- PowerShell (Serial communication bridge)  

---

## How It Works

### Normal Operation

1. Modbus command is sent (Write Single Register - Function Code 6)  
2. Command is observed in Wireshark  
3. Command is translated via serial to Arduino  
4. Arduino updates system state:  
   - START → Ride Running  
   - STOP → Ride Stopped  

---

### Attack Scenario: Unsafe Command Injection

This project simulates a realistic OT security scenario where a valid control command is issued during an unsafe system state.

**Scenario:**
- System is placed in Emergency Stop  
- A START command is sent via Modbus  

**Result:**
- Command is rejected  
- System remains in fault state  
- Physical system does not start  

**Why this matters:**
In many OT environments, protocols like Modbus lack authentication. Systems must rely on internal safety logic to prevent dangerous operations.

---

## Security Concept Demonstrated

This project demonstrates **control integrity**:

> A command may be valid at the protocol level but unsafe at the system level.

Key principle:

    System State > External Command

---

## Example Commands

### Modbus
- Function Code: 0x06 (Write Single Register)  
- Register 1 = 1 → START  
- Register 1 = 0 → STOP  

### Serial Commands
- START  
- STOP  
- STATUS  

---

## Demonstration

See included media:

- RideFunctions.mp4 — full system demonstration  
- Before_Attack.png — normal operation  
- Start_Command_Blocked.png — attack scenario  
- Serial_Monitor_Commands.png — command output  
- TinkerCad_Wiring_Diagram.png — system wiring  

---

## Learning Outcomes

- Understanding of OT vs IT security differences  
- Hands-on experience with Modbus TCP  
- Ability to analyze industrial traffic in Wireshark  
- Implementation of safety interlocks  
- Demonstration of real-world control system risks  

---

## Future Improvements

- Direct Modbus-to-Arduino integration (no manual bridge)  
- Authentication or command validation layer  
- Intrusion detection logic for anomalous commands  
- Logging and alerting system  
- Integration with SCADA-style interface  

---

## Author

Adrian Benton  
Cybersecurity (SOC / OT Security Focus)

Charlie Caputo
Intelligent System Engineer

---

## Notes

This project is intended for educational purposes to demonstrate OT security concepts in a controlled environment.
