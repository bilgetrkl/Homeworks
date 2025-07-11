#pragma once

#include <string>
#include <vector>

enum class InstructionType {
    SET,    // Direct Set
    CPY,    // Direct Copy
    CPYI,   // Indirect Copy
    CPYI2,  // Double Indirect Copy
    ADD,    // Direct Add
    ADDI,   // Indirect Add
    SUBI,   // Indirect Subtraction
    JIF,    // Jump If
    PUSH,   // Push to Stack
    POP,    // Pop from Stack
    CALL,   // Call Subroutine
    RET,    // Return from Subroutine
    HLT,    // Halt CPU
    USER,   // Switch to User Mode
    SYSCALL // System Call
};

enum class SyscallType {
    PRN,    // Print to console
    HLT,    // Halt
    YIELD   // Yield CPU
};

struct Instruction {
    InstructionType type;
    SyscallType syscallType;  // Only used when type is SYSCALL
    std::vector<long> operands;
    
    Instruction() : type(InstructionType::HLT) {}
    
    Instruction(InstructionType t) : type(t) {}
    
    Instruction(InstructionType t, const std::vector<long>& ops) 
        : type(t), operands(ops) {}
    
    Instruction(InstructionType t, SyscallType st, const std::vector<long>& ops) 
        : type(t), syscallType(st), operands(ops) {}
};