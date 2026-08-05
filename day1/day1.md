Day 1 – Embedded System Fundamentals
1. What Is an Embedded System?
An embedded system is a computer-based system designed to perform a specific or dedicated function within a larger product.
An embedded system generally contains:
•	Microcontroller or microprocessor
•	Sensors
•	Actuators
•	Communication interfaces
•	Memory
•	Firmware
Examples
•	EV charger
•	Washing machine
•	Motor controller
•	Smart meter
•	Battery Management System (BMS)
•	Automotive ECU
•	Medical device
Interview answer
An embedded system is a dedicated computer system integrated into a product to perform specific functions. It consists of hardware and firmware and may operate under real-time constraints.
________________________________________
2. Why Is an EV Charger an Embedded System?
An EV charger is an embedded system because it uses controllers, sensors, communication interfaces, and firmware to perform dedicated charging and monitoring functions.
The embedded firmware can:
•	Initialize the charger
•	Control charging operations
•	Read voltage and current
•	Monitor temperature
•	Check faults
•	Monitor door and emergency-stop status
•	Communicate with the vehicle
•	Communicate with power modules
•	Read energy-meter data
•	Control the HMI
•	Send data to the backend server
•	Start and stop charging safely
Interview answer
An EV charger is an embedded system because its firmware controls dedicated functions such as charging control, safety monitoring, vehicle communication, power-module control, meter reading, HMI operation, and backend communication.
________________________________________
3. Hardware, Software, and Firmware
Hardware
Hardware consists of the physical components of a system.
Examples:
•	PCB
•	STM32 microcontroller
•	Sensor
•	Relay
•	Contactor
•	CAN transceiver
•	Power module
•	HMI
•	Energy meter
Software
Software is a program that usually runs on a general-purpose computer or operating system.
Examples:
•	Visual Studio Code
•	Microsoft Excel
•	Web browser
•	Windows applications
Firmware
Firmware is software designed to run on embedded hardware and directly control or monitor the system.
Firmware is generally stored in non-volatile memory, such as Flash.
Examples:
•	EV charger firmware
•	BMS firmware
•	Motor-controller firmware
•	Router firmware
Comparison
Hardware	Firmware
Physical components	Program running on the hardware
Can be physically touched	Cannot be physically touched
Includes MCU, PCB, sensors	Includes Embedded C code
Provides physical capability	Controls and monitors hardware
Interview answer
Firmware is software stored in non-volatile memory, such as Flash, that controls hardware and performs the required functions of an embedded system.
________________________________________
4. Microcontroller vs Microprocessor
Microcontroller (MCU)
A microcontroller generally contains:
•	CPU
•	Flash
•	RAM
•	GPIO
•	Timers
•	UART
•	SPI
•	I²C
•	ADC
•	PWM
•	Other peripherals
Examples:
•	STM32
•	ESP32
•	PIC
•	AVR
Microprocessor (MPU)
A microprocessor mainly contains a powerful CPU and generally requires external components such as:
•	External RAM
•	External storage
•	External peripherals
Examples:
•	Intel Core processors
•	AMD Ryzen processors
•	ARM Cortex-A processors
Comparison
Microcontroller	Microprocessor
CPU, memory, and peripherals integrated	Mainly CPU; external components are commonly required
Lower power	Usually higher power
Lower system cost	Higher system complexity
Used for dedicated control	Used for complex computing
Common in embedded products	Common in computers and advanced Linux systems
Interview answer
A microcontroller integrates the CPU, memory, and peripherals on a single chip, making it suitable for dedicated embedded applications. A microprocessor mainly provides the CPU and generally requires external memory and peripherals.
________________________________________
5. Real-Time Behavior
A real-time system must respond within a defined time limit.
Real-time does not only mean “fast.” It means:
The system must provide the correct response within the required time.
EV charger examples
•	Detect overcurrent and stop charging within the required time.
•	Process vehicle communication messages within protocol timing requirements.
•	Detect an emergency-stop signal quickly.
•	Respond to communication timeouts.
•	Monitor safety conditions continuously.


Interview answer
Embedded systems often require real-time behavior because they interact with physical hardware and must respond to events within defined timing limits to maintain correct operation and safety.
________________________________________
Day 1 Part 2 – CPU, Memory, and Firmware Execution
6. CPU
CPU stands for:
Central Processing Unit
The CPU executes firmware instructions.
Its main operations are:
Fetch → Decode → Execute
Example:
if (temperature > 70)
{
    stop_charging();
}
The CPU:
1.	Reads the temperature.
2.	Compares it with 70.
3.	Determines whether the condition is true.
4.	Executes the required instruction.
Interview answer
The CPU fetches, decodes, and executes program instructions. It performs calculations, logical operations, and controls the operation of the embedded system.
________________________________________
7. ALU
ALU stands for:
Arithmetic Logic Unit
The ALU performs:
•	Addition
•	Subtraction
•	Multiplication
•	Division
•	Comparison
•	Logical operations
•	Bitwise operations
Example:
power = voltage * current;
________________________________________
8. Control Unit
The Control Unit coordinates instruction execution.
It controls:
•	Which instruction is executed
•	Which data is read
•	Which operation is performed
•	Where the result is stored
________________________________________
9. CPU Registers
Registers are very small and very fast storage locations inside the CPU.
They temporarily store:
•	Data
•	Addresses
•	Calculation results
•	Function parameters
•	Instruction information
Important registers
Program Counter (PC)
Stores the address of the next instruction to execute.
Stack Pointer (SP)
Stores the current stack location.
________________________________________
10. Flash Memory
Flash is non-volatile memory.
This means: Data remains stored after power is removed.
Flash generally stores:
•	Firmware code
•	Program instructions
•	Constants
•	Startup information
Example:
Power OFF
   ↓
Firmware remains stored in Flash
Interview answer
Flash is non-volatile memory used to store firmware and program code. Its contents remain available after power is removed.
________________________________________
11. RAM
RAM stands for:
Random Access Memory
RAM is volatile memory.
This means:
Data stored in RAM is lost when power is removed.
RAM stores runtime information such as:
•	Voltage
•	Current
•	Temperature
•	Communication buffers
•	CAN data
•	Modbus data
•	OCPP message data
•	Charging status
•	Temporary calculations
Example:
uint16_t charging_voltage = 750;
uint16_t charging_current = 200;
Interview answer
RAM is volatile memory used to store runtime variables, communication buffers, temporary data, and other information required while firmware is executing.
________________________________________
12. Flash vs RAM
Feature	Flash	RAM
Memory type  	Non-volatile	Volatile
Data after power OFF	 Remains	Lost
Main use	Firmware storage	Runtime data
Typical content	Code and constants	Variables and buffers
Speed	Generally slower	Generally faster
________________________________________
13. Stack
The Stack is an area of RAM mainly used for:
•	Local variables
•	Function parameters
•	Function-call information
•	Return information
Example:
void check_current(void)
{
    int current = 150;
}
The local variable current is typically stored on the stack.
Stack overflow
The stack has limited size.
A large local array can consume too much stack memory:
void process_data(void)
{
    uint8_t buffer[100000];
}
This can cause:
•	Stack overflow
•	Memory corruption
•	HardFault
•	Unexpected reset
•	Firmware crash
•	Unpredictable behavior
Interview answer
The stack stores local variables, function parameters, and function-call information. Excessive stack usage can cause stack overflow and lead to firmware failure.
________________________________________
14. Heap
The Heap is an area of RAM used for dynamic memory allocation.
Common functions:
malloc();
calloc();
realloc();
free();
Example:
uint8_t *buffer;

buffer = malloc(256);
After use:
free(buffer);
Risks in embedded systems
•	Memory leaks
•	Memory fragmentation
•	Allocation failure
•	Difficult debugging
•	Less predictable memory behavior
The heap is not always forbidden, but it must be used carefully.
________________________________________
15. Stack vs Heap
Stack	Heap
Used for local variables and function calls	 Used for dynamic memory
Automatically managed	Usually manually managed
Fast allocation	More flexible allocation
Limited size	Uses available heap memory
Can overflow	Can fragment or leak
________________________________________
16. How C Code Becomes Firmware
The complete process is:
Source Code (.c)
        ↓
Preprocessor
        ↓
Compiler
        ↓
Assembly Code (.s)
        ↓
Assembler
        ↓
Object File (.o)
        ↓
Linker
        ↓
ELF File (.elf)
        ↓
HEX/BIN File
        ↓
Programmed into MCU Flash
        ↓
CPU Executes Firmware
Source code
Example:
int main(void)
{
    return 0;
}
Preprocessor
Handles:
#include
#define
#ifdef
#if
Compiler
Converts C code into lower-level instructions.
Assembler
Converts assembly code into an object file.
Example:
main.o
Linker
Combines object files and connects functions and libraries.
Example:
main.o
can.o
modbus.o
charger.o
      ↓
    Linker
      ↓
firmware.elf
HEX/BIN
The firmware is converted into a format that can be programmed into the microcontroller.
Examples:
firmware.hex
firmware.bin
________________________________________
17. What Happens When an MCU Powers ON?
Typical sequence:
Power ON
   ↓
Microcontroller Reset
   ↓
CPU reads startup information
   ↓
Stack Pointer is initialized
   ↓
Program Counter is initialized
   ↓
Startup code executes
   ↓
RAM sections are initialized
   ↓
System clock is configured
   ↓
main() starts
   ↓
Firmware initializes peripherals
   ↓
Main application runs continuously
________________________________________
18. EV Charger Startup Sequence
A simplified EV charger sequence is:
Power ON
   ↓
Controller and firmware start
   ↓
HMI and status indicators initialize
   ↓
Hardware and communication interfaces initialize
   ↓
Fault and safety checks
   ↓
Power-module and meter communication
   ↓
Backend connection using OCPP
   ↓
Gun status becomes Available
   ↓
Vehicle connected
   ↓
Vehicle communication and handshake
   ↓
Safety validation
   ↓
Power path enabled
   ↓
Charging starts
   ↓
Voltage, current, temperature, and faults monitored
   ↓
Charging data sent to backend
   ↓
Stop request / Vehicle full / Fault
   ↓
Power delivery stopped safely
   ↓
Transaction closed
   ↓
Gun returns to Available
________________________________________
19. Main Loop Example
int main(void)
{
    system_init();

    while (1)
    {
        read_sensors();

        check_faults();

        update_hmi();

        communicate_with_vehicle();

        communicate_with_server();
    }
}
The statement:
while (1)
creates an infinite loop.
The firmware continuously:
•	Reads inputs
•	Checks faults
•	Updates outputs
•	Handles communication
•	Controls the system
________________________________________
Day 1 Key Revision
Embedded System
→ Hardware + firmware designed for a dedicated function

Firmware
→ Software that controls embedded hardware

CPU
→ Executes instructions

ALU
→ Performs arithmetic and logical operations

Registers
→ Very fast storage inside the CPU

Flash
→ Non-volatile memory
→ Stores firmware

RAM
→ Volatile memory
→ Stores runtime data

Stack
→ Local variables and function-call information

Heap
→ Dynamic memory allocation

Large local array
→ May cause stack overflow

Build flow
→ .c → Preprocessor → Compiler → Assembly
→ Assembler → .o → Linker → .elf
→ .hex/.bin → MCU Flash
________________________________________
Day 1 Self-Check
You should now be able to answer:
1.	What is an embedded system?
2.	Why is an EV charger an embedded system?
3.	What is firmware?
4.	What is the difference between hardware and firmware?
5.	What is the difference between an MCU and an MPU?
6.	What is real-time behavior?
7.	What is the role of the CPU?
8.	What is the difference between Flash and RAM?
9.	What is the Stack?
10.	What is the Heap?
11.	Why are large local arrays risky?
12.	How does C code become firmware?
13.	What happens when a microcontroller powers ON?
