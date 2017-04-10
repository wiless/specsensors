#include <xc.h>
#include "delay.h"

int __attribute__ ((space(eedata))) eeData; // Global variable located in EEPROM

void writeNVM(int addr, int newData)
{
    unsigned int offset;
    NVMCON = 0x4004;
    // Set up a pointer to the EEPROM location to be erased
    TBLPAG = __builtin_tblpage(&eeData); // Initialize EE Data page pointer
    offset = __builtin_tbloffset(&eeData) + (addr & 0x01ff); // Initizlize lower word of address
    __builtin_tblwtl(offset, newData); // Write EEPROM data to write latch
    asm volatile ("disi #5"); // Disable Interrupts For 5 Instructions
    __builtin_write_NVM(); // Issue Unlock Sequence & Start Write Cycle
    while(NVMCONbits.WR==1); // Optional: Poll WR bit to wait for
    // write sequence to complete
    delay(100);
}

int readNVM (int addr)
{
    int data; // Data read from EEPROM
    unsigned int offset;

    // Set up a pointer to the EEPROM location to be erased
    TBLPAG = __builtin_tblpage(&eeData); // Initialize EE Data page pointer
    offset = __builtin_tbloffset(&eeData) + (addr & 0x01ff); // Initizlize lower word of address
    data = __builtin_tblrdl(offset); // Write EEPROM data to write latch
    delay(10);
    return data;
}
