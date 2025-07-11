#pragma once

#include "instruction.h"
#include "memory.h"
#include <map>
#include <functional>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <chrono>

class CPU {
private:
    Memory memory;
    std::map<size_t, Instruction> instructions; 
    bool halted;
    bool kernelMode;
    
    static constexpr size_t PC = 0; 
    static constexpr size_t SP = 1;      
    static constexpr size_t SYSCALL_RESULT = 2; 
    static constexpr size_t INSTR_COUNT = 3;
    
    std::map<InstructionType, std::function<void(const Instruction&)>> instructionHandlers;
    std::function<void()> onContextSwitchOrSyscall;

    void initInstructionHandlers();
    void executeSET(const Instruction& instr);
    void executeCPY(const Instruction& instr);
    void executeCPYI(const Instruction& instr);
    void executeCPYI2(const Instruction& instr);
    void executeADD(const Instruction& instr);
    void executeADDI(const Instruction& instr);
    void executeSUBI(const Instruction& instr);
    void executeJIF(const Instruction& instr);
    void executePUSH(const Instruction& instr);
    void executePOP(const Instruction& instr);
    void executeCALL(const Instruction& instr);
    void executeRET(const Instruction& instr);
    void executeHLT(const Instruction& instr);
    void executeUSER(const Instruction& instr);
    void executeSYSCALL(const Instruction& instr);
public:
    CPU();
    void loadProgram(const std::map<size_t, Instruction>& instructionMap, const std::vector<std::pair<size_t, long>>& data);
    void execute();
    bool isHalted() const;
    void reset();
    Memory& getMemory();
    void setDebugCallback(std::function<void()> cb);
    void printCurrentState() const;
};