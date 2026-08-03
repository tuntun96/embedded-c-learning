## Learning Objectives

By the end of Day 6, you should understand:

- What is a pointer
- Memory address concept
- Pointer declaration and initialization
- Address operator (`&`)
- Dereference operator (`*`)
- Reading and modifying data using pointers
- Passing variables by address to functions
- Pass by value vs pass by address
- Relationship between arrays and pointers
- Pointer arithmetic
- Embedded applications of pointers


# 1. Introduction to Pointer

A pointer is a variable that stores the memory address of another variable.

Normal variable:


Stores data value


Pointer:


Stores memory address


Example:

```c
uint16_t voltage = 650U;

uint16_t *voltage_pointer;

voltage_pointer = &voltage;

Memory:

voltage
+---------+
|   650   |
+---------+
    ^
    |
voltage_pointer
(stores address)
2. Pointer Declaration

Syntax:

data_type *pointer_name;

Example:

uint16_t *voltage_pointer;

The pointer data type should match the variable type.

Example:

uint16_t voltage;
uint16_t *ptr;
3. Address Operator (&)

& is used to get the address of a variable.

Example:

uint16_t voltage = 650U;

printf("%p",&voltage);

&voltage gives the memory address where voltage is stored.

4. Pointer Initialization

Example:

uint16_t voltage = 650U;

uint16_t *ptr;

ptr = &voltage;

Now:

ptr
 |
 |
 v
Address of voltage
5. Dereference Operator (*)

*pointer accesses the value stored at the address.

Example:

uint16_t voltage = 650U;

uint16_t *ptr;

ptr = &voltage;

printf("%u",*ptr);

Output:

650

Important:

ptr
→ stores address

*ptr
→ accesses value
6. Updating Value Using Pointer

Example:

uint16_t voltage = 650U;

uint16_t *ptr;

ptr = &voltage;

*ptr = 700U;

After execution:

voltage = 700

Because pointer modifies the original memory location.

7. Pass by Value

C normally passes arguments by value.

Example:

void update_voltage(uint16_t voltage)
{
    voltage = 700U;
}

Function receives a copy.

Original variable does not change.

8. Pass by Address

To modify the original variable, pass the address.

Example:

void update_voltage(uint16_t *voltage)
{
    *voltage = 700U;
}

Function call:

update_voltage(&charger_voltage);

Now original value changes.

9. Pointer With Arrays

Array name represents the address of the first element.

Example:

uint16_t voltage_samples[5]=
{
650,
660,
670,
680,
690
};

Array:

Index     Value

0         650
1         660
2         670
3         680
4         690

Array name:

voltage_samples

represents:

Address of voltage_samples[0]
10. Array and Pointer Relationship

These are equivalent:

voltage_samples[2]

and

*(voltage_samples + 2)

Example:

uint16_t *ptr;

ptr = voltage_samples;

printf("%u",*(ptr+2));

Output:

670
11. Pointer Arithmetic

Pointer arithmetic allows movement through array elements.

Example:

ptr + 1

moves to the next array element.

Relationship:

array[index]

is equal to:

*(array + index)

Example:

ptr[3]

is equal to:

*(ptr+3)
12. Embedded Applications of Pointer
Hardware Register Access

Pointers allow direct access to hardware registers.

Example:

*GPIO_REGISTER = 1U;

Used in:

GPIO control
ADC configuration
Timer registers
Peripheral drivers
Communication Buffers

Used with:

CAN
UART
SPI
I2C

Example:

uint8_t rx_buffer[8];

void CAN_Read(uint8_t *buffer)
{
    buffer[0]=0x10;
}
Sensor Data Processing

Example:

uint16_t voltage_samples[100];

Pointers help process large sensor buffers efficiently.

Pointer Rules
Symbol	Meaning
&variable	Address of variable
pointer	Stores address
*pointer	Value at address

Example:

uint16_t voltage=650U;

uint16_t *ptr=&voltage;

Relationship:

voltage
→ 650


&voltage
→ address


ptr
→ address


*ptr
→ 650
Day 6 Programs Completed
pointer_basic.c

Covered:

Pointer declaration
Address printing
Dereferencing
pointer_function.c

Covered:

Pass by address
Updating original variable
pointer_array.c

Covered:

Array pointer relationship
Pointer arithmetic
Sensor buffer processing
Interview Questions
What is a pointer?

A pointer is a variable that stores the memory address of another variable.

Difference between ptr and *ptr?
ptr  → address

*ptr → value stored at address
Why pointers are used in embedded systems?

Pointers provide:

Direct memory access
Hardware register access
Efficient buffer handling
Communication data processing
Reduced memory copying
Day 6 Summary

Completed:

✅ Pointer fundamentals
✅ Address operator
✅ Dereferencing
✅ Pointer update
✅ Function pointer basics
✅ Pass by address
✅ Array pointer relationship
✅ Pointer arithmetic
✅ Embedded applications