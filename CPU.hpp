#ifndef CPU_H
#define CPU_H

#include<stdint.h>
#include<stdio.h>
#include"MMU.hpp"

class CPU{
private:
    MMU* mmu;
    //ports+custom hardware
    uint16_t reg_SHIFT;
    uint8_t ShiftOFFSET;
    
    //temp vars
    uint16_t WorkValue;
    
    // Registers
    uint8_t* reg_M;
    uint8_t true_reg_A;
    
    uint16_t reg_BC;
    uint16_t reg_DE;
    uint16_t reg_HL;
    
    uint8_t* reg_A = &true_reg_A; // Pointer uniformity
    uint8_t* reg_B = ((uint8_t*) &reg_BC) + 1;
    uint8_t* reg_C = ((uint8_t*) &reg_BC);
    uint8_t* reg_D = ((uint8_t*) &reg_DE) + 1;
    uint8_t* reg_E = ((uint8_t*) &reg_DE);
    uint8_t* reg_H = ((uint8_t*) &reg_HL) + 1;
    uint8_t* reg_L = ((uint8_t*) &reg_HL);
    
    uint16_t PC, SP;//the same pc and sp we know and love
    
    //flags
    
    uint8_t ZSP[0X100];
    uint8_t Z;// ZERO FLAG; set to 1 if current operation results in Zero, or two values match on a CMP operation
    uint8_t S;// SUBTRACT FLAG; set to 1 if a subtraction was performed
    uint8_t P;//parity or half carry
    uint8_t C;// CARRY FLAG; set to 1 if a carry occured in the last operation or if A is the smaller value on CP instruction

    uint8_t AC;//AUXILARY CARRY
    // Functions - Control
    void Syscall (uint8_t ID);
    void Execute (uint8_t Instruction);
        
    // Functions - Memory Management
    uint8_t GetByteAt (uint16_t Address);
    void SetByteAt (uint16_t Address, uint8_t Value);
    uint16_t GetWordAt (uint16_t Address);
    void SetWordAt (uint16_t Address, uint16_t Value);
    void PushPSW ();
    void PopPSW ();
    
    // Functions - Stack Management
    uint16_t StackPop ();
    void StackPush (uint16_t Value);

    // Functions - Flags Management
    void SetFlagsAdd (uint8_t OpA, uint8_t OpB, uint8_t Carry, uint8_t CarryMode);
    void SetFlagsSub (uint8_t OpA, uint8_t OpB, uint8_t Carry, uint8_t CarryMode);
    void SetZSP (uint8_t Value);
    
    // Functions - Debugging
    void Debug ();
public:
    CPU (MMU* _mmu, uint8_t _ConsoleMode);
    void Clock ();
    void Interrupt (uint8_t ID);
        
    // I/O
    uint8_t InPort[4];
    uint8_t OutPort[7];
    
    // Status
    uint8_t InterruptsEnabled;
    uint8_t Halt;
    uint64_t ClockCount;
    uint64_t InstructionCount;
    uint8_t ConsoleMode;
    
};

#endif
