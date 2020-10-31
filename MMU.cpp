#include "MMU.hpp"
#include <cstring>
#include <stdio.h>

//just doing some memory management here sir nothing to see here

MMU::MMU(uint8_t ConsoleMode) {
    memset (Memory, 0, sizeof(Memory));
    
    for (int i = 0x0000; i < 0x4000; i++) { // ROM + RAM + VRAM
        MemoryMap[i] = Memory + i;
    }
    
    if (!ConsoleMode) {
        for (int i = 0x4000; i < 0x4400; i++) // Mirrored RAM
            MemoryMap[i] = Memory + i - 0x2000;
    } else {
        for (int i = 0x4000; i <= 0xFFFF; i++) // Unmirrored
            MemoryMap[i] = Memory + i;
    }
}

void MMU::LoadinMemoryI (uint8_t* Buffer, uint16_t Address, int BufferSize) {
    for (int i = 0; i < BufferSize; i++)
        *(MemoryMap[Address + i]) = Buffer[i];
}
