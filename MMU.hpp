
#ifndef MMU_hpp
#define MMU_hpp
#include <stdint.h>

class MMU{
    private:
    uint8_t JunkValue;
    uint8_t Memory[0x10000];
    
    public:
    MMU(uint8_t ConsoleMode);
    //Memory layout or more of where we wanna store the stuff
    /*
     ROM: from 0x0000 to 0x1FFF
     RAM: from 0x2000 to 0x23FF
     VRAM: from 0x2400 to 0x3FFF
     RAMMIRROR: from 0x4000 to 0x43FF
     */
    uint16_t ROMaddress=0X0000;
    uint16_t RAMaddress=0X2000;
    uint16_t VRAMaddress=0X2400;
    uint16_t RAMMIRRORaddress=0X4000;
    
    //convinience pointer
    uint8_t* VRAM= Memory + VRAMaddress;
    uint8_t* MemoryMap[0x10000]; //64k magic in the aaaaaiiiirrrrrrr
    void LoadinMemoryI(uint8_t* Buffer, uint16_t address, int bufferSize);
};

#endif /* MMU_hpp */
