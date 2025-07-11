#include "cpu.h"

CPU::CPU() : halted(false), kernelMode(true) { // Constructor initializes CPU state
    initInstructionHandlers();
}

void CPU::initInstructionHandlers() { // Initialize instruction handlers
    instructionHandlers[InstructionType::SET] = [this](const Instruction& instr) { executeSET(instr); };
    instructionHandlers[InstructionType::CPY] = [this](const Instruction& instr) { executeCPY(instr); };
    instructionHandlers[InstructionType::CPYI] = [this](const Instruction& instr) { executeCPYI(instr); };
    instructionHandlers[InstructionType::CPYI2] = [this](const Instruction& instr) { executeCPYI2(instr); };
    instructionHandlers[InstructionType::ADD] = [this](const Instruction& instr) { executeADD(instr); };
    instructionHandlers[InstructionType::ADDI] = [this](const Instruction& instr) { executeADDI(instr); };
    instructionHandlers[InstructionType::SUBI] = [this](const Instruction& instr) { executeSUBI(instr); };
    instructionHandlers[InstructionType::JIF] = [this](const Instruction& instr) { executeJIF(instr); };
    instructionHandlers[InstructionType::PUSH] = [this](const Instruction& instr) { executePUSH(instr); };
    instructionHandlers[InstructionType::POP] = [this](const Instruction& instr) { executePOP(instr); };
    instructionHandlers[InstructionType::CALL] = [this](const Instruction& instr) { executeCALL(instr); };
    instructionHandlers[InstructionType::RET] = [this](const Instruction& instr) { executeRET(instr); };
    instructionHandlers[InstructionType::HLT] = [this](const Instruction& instr) { executeHLT(instr); };
    instructionHandlers[InstructionType::USER] = [this](const Instruction& instr) { executeUSER(instr); };
    instructionHandlers[InstructionType::SYSCALL] = [this](const Instruction& instr) { executeSYSCALL(instr); };
}

// Load program and data into CPU
void CPU::loadProgram(const std::map<size_t, Instruction>& instructionMap, const std::vector<std::pair<size_t, long>>& data) {
    instructions = instructionMap;
    memory.loadData(data);
    reset();
}

void CPU::reset() { // Reset CPU state
    halted = false;
    kernelMode = true;
    memory.write(SYSCALL_RESULT, 0);
    memory.write(INSTR_COUNT, 0);
    memory.write(PC, 0);
}

bool CPU::isHalted() const { // Check if CPU is halted
    return halted;
}

Memory& CPU::getMemory() { // Get memory for debugging
    return memory;
}

void CPU::execute() { // Execute one instruction
    long pc = memory.read(PC);
    long currentThread = memory.read(22);

    if (pc == 795 && onContextSwitchOrSyscall) { // Context switch instruction
        onContextSwitchOrSyscall();
    }

    // Update thread instruction count
    long threadTableBegin = 500;
    long threadTableSize = 10;
    long offset = 2;
    
    // Calculate the range for the current thread
    long start = currentThread * 1000;
    long end = ((currentThread + 1) * 1000) - 1;

    if (pc >= start && pc <= end) { // Check if PC is within the current thread's data segment
        long countAddr = threadTableBegin + currentThread * threadTableSize + offset;
        memory.write(countAddr, memory.read(countAddr) + 1);
    }

    if (halted)
        return;
    
    auto instrIt = instructions.find(pc);
    if (instrIt == instructions.end()) { // Check if instruction exists at PC
        std::cerr << "Error: No instruction found at PC (" << pc << ")." << std::endl;
        halted = true;
        return;
    }
    
    const Instruction& instr = instrIt->second;
    
    // Execute the instruction
    try {
        auto handler = instructionHandlers.find(instr.type);
        if (handler != instructionHandlers.end()) {
            handler->second(instr);
        } else {
            std::cerr << "Error: Unknown instruction type." << std::endl;
            halted = true;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error executing instruction: " << e.what() << std::endl;
        halted = true;
        return;
    }
    
    // Increment instruction count
    long count = memory.read(INSTR_COUNT);
    memory.write(INSTR_COUNT, count + 1);
    
    // Increment PC if it wasn't modified by the instruction
    if (pc == memory.read(PC)) {
        memory.write(PC, pc + 1);
    }
}

void CPU::executeSET(const Instruction& instr) { // Execute SET instruction
    if (instr.operands.size() != 2) { 
        throw std::runtime_error("SET instruction requires 2 operands");
    }
    
    long value = instr.operands[0];
    long address = instr.operands[1];
    
    if (!memory.isAddressValid(address, kernelMode)) { 
        std::cerr << "Access violation: Address " << address << " not accessible in current mode" << std::endl;
        halted = true;
        return;
    }
    
    memory.write(address, value);
}

void CPU::executeCPY(const Instruction& instr) { // Execute CPY instruction
    if (instr.operands.size() != 2) {
        throw std::runtime_error("CPY instruction requires 2 operands");
    }
    
    long srcAddr = instr.operands[0];
    long destAddr = instr.operands[1];
    
    if (!memory.isAddressValid(srcAddr, kernelMode) || !memory.isAddressValid(destAddr, kernelMode)) {
        std::cerr << "Access violation: Memory address not accessible in current mode" << std::endl;
        halted = true;
        return;
    }
    
    long value = memory.read(srcAddr);
    memory.write(destAddr, value);
}

void CPU::executeCPYI(const Instruction& instr) { // Execute CPYI instruction
    if (instr.operands.size() != 2) {
        throw std::runtime_error("CPYI instruction requires 2 operands");
    }
    
    long indirectAddr = instr.operands[0];
    long destAddr = instr.operands[1];
    
    if (!memory.isAddressValid(indirectAddr, kernelMode) || !memory.isAddressValid(destAddr, kernelMode)) {
        std::cerr << "Access violation: Memory address not accessible in current mode" << std::endl;
        halted = true;
        return;
    }
    
    // Get the indirect address value
    long srcAddr = memory.read(indirectAddr);
    
    if (!memory.isAddressValid(srcAddr, kernelMode)) {
        std::cerr << "Access violation: Indirect memory address not accessible in current mode" << std::endl;
        halted = true;
        return;
    }
    
    long value = memory.read(srcAddr);
    memory.write(destAddr, value);
}

void CPU::executeCPYI2(const Instruction& instr) { // Execute CPYI2 instruction
    if (instr.operands.size() != 2) {
        throw std::runtime_error("CPYI2 instruction requires 2 operands");
    }
    long a1 = instr.operands[0];
    long a2 = instr.operands[1];
    if (!memory.isAddressValid(a1, kernelMode) || !memory.isAddressValid(a2, kernelMode)) {
        std::cerr << "Access violation: Memory address not accessible in current mode" << std::endl;
        halted = true;
        return;
    }
    long srcAddr = memory.read(a1);
    long destAddr = memory.read(a2);
    if (!memory.isAddressValid(srcAddr, kernelMode) || !memory.isAddressValid(destAddr, kernelMode)) {
        std::cerr << "Access violation: Indirect memory address not accessible in current mode" << std::endl;
        halted = true;
        return;
    }
    long value = memory.read(srcAddr);
    memory.write(destAddr, value);
}

void CPU::executeADD(const Instruction& instr) { // Execute ADD instruction
    if (instr.operands.size() != 2) {
        throw std::runtime_error("ADD instruction requires 2 operands");
    }
    
    long address = instr.operands[0];
    long value = instr.operands[1];
    
    if (!memory.isAddressValid(address, kernelMode)) {
        std::cerr << "Access violation: Address not accessible in current mode" << std::endl;
        halted = true;
        return;
    }
    
    long current = memory.read(address);
    memory.write(address, current + value);
}

void CPU::executeADDI(const Instruction& instr) { // Execute ADDI instruction
    if (instr.operands.size() != 2) {
        throw std::runtime_error("ADDI instruction requires 2 operands");
    }
    
    long destAddr = instr.operands[0];
    long srcAddr = instr.operands[1];
    
    if (!memory.isAddressValid(destAddr, kernelMode) || !memory.isAddressValid(srcAddr, kernelMode)) {
        std::cerr << "Access violation: Address not accessible in current mode" << std::endl;
        halted = true;
        return;
    }
    
    long value = memory.read(srcAddr);
    long current = memory.read(destAddr);
    memory.write(destAddr, current + value);
}

void CPU::executeSUBI(const Instruction& instr) { // Execute SUBI instruction
    if (instr.operands.size() != 2) {
        throw std::runtime_error("SUBI instruction requires 2 operands");
    }
    
    long srcAddr = instr.operands[0];   // A1: source address
    long destAddr = instr.operands[1];  // A2: destination address
    
    if (!memory.isAddressValid(srcAddr, kernelMode) || !memory.isAddressValid(destAddr, kernelMode)) {
        std::cerr << "Access violation: Address not accessible in current mode" << std::endl;
        halted = true;
        return;
    }
    
    long srcValue = memory.read(srcAddr);    // memory[A1] value
    long destValue = memory.read(destAddr);  // memory[A2] value
    
    memory.write(destAddr, srcValue - destValue);
}

void CPU::executeJIF(const Instruction& instr) { // Execute JIF instruction
    if (instr.operands.size() != 2) {
        throw std::runtime_error("JIF instruction requires 2 operands");
    }
    
    long memAddr = instr.operands[0];
    long jumpAddr = instr.operands[1];
    
    if (!memory.isAddressValid(memAddr, kernelMode)) {
        std::cerr << "Access violation: Address not accessible in current mode" << std::endl;
        halted = true;
        return;
    }
    
    long value = memory.read(memAddr);
    
    if (value <= 0) {
        memory.write(PC, jumpAddr);
    }
}

void CPU::executePUSH(const Instruction& instr) { // Execute PUSH instruction
    if (instr.operands.size() != 1) {
        throw std::runtime_error("PUSH instruction requires 1 operand");
    }
    
    long address = instr.operands[0];
    
    if (!memory.isAddressValid(address, kernelMode)) {
        std::cerr << "Access violation: Address not accessible in current mode" << std::endl;
        halted = true;
        return;
    }
    
    long value = memory.read(address);
    long sp = memory.read(SP);
    
    // Stack grows downward
    sp--;
    if (!memory.isAddressValid(sp, kernelMode)) {
        std::cerr << "Stack overflow" << std::endl;
        halted = true;
        return;
    }
    
    memory.write(sp, value);
    memory.write(SP, sp);
}

void CPU::executePOP(const Instruction& instr) { // Execute POP instruction
    if (instr.operands.size() != 1) {
        throw std::runtime_error("POP instruction requires 1 operand");
    }
    
    long address = instr.operands[0];
    
    if (!memory.isAddressValid(address, kernelMode)) {
        std::cerr << "Access violation: Address not accessible in current mode" << std::endl;
        halted = true;
        return;
    }
    
    long sp = memory.read(SP);
    
    if (!memory.isAddressValid(sp, kernelMode)) {
        std::cerr << "Stack underflow" << std::endl;
        halted = true;
        return;
    }
    
    long value = memory.read(sp);
    memory.write(address, value);
    
    memory.write(SP, sp + 1);
}

void CPU::executeCALL(const Instruction& instr) { // Execute CALL instruction
    if (instr.operands.size() != 1) {
        throw std::runtime_error("CALL instruction requires 1 operand");
    }
    
    long jumpAddr = instr.operands[0];
    
    // Push return address (PC + 1) onto stack
    long pc = memory.read(PC);
    long returnAddr = pc + 1;
    
    long sp = memory.read(SP);
    sp--;
    
    if (!memory.isAddressValid(sp, kernelMode)) {
        std::cerr << "Stack overflow" << std::endl;
        halted = true;
        return;
    }
    
    memory.write(sp, returnAddr);
    memory.write(SP, sp);
    
    // Jump to subroutine
    memory.write(PC, jumpAddr);
    if (onContextSwitchOrSyscall) onContextSwitchOrSyscall();
}

void CPU::executeRET(const Instruction& instr) { // Execute RET instruction
    (void) instr;
    long sp = memory.read(SP);
    
    if (!memory.isAddressValid(sp, kernelMode)) {
        std::cerr << "Stack underflow" << std::endl;
        halted = true;
        return;
    }
    
    // Pop return address and set PC
    long returnAddr = memory.read(sp);
    memory.write(PC, returnAddr);
    
    memory.write(SP, sp + 1);
}

void CPU::executeHLT(const Instruction& instr) { // Execute HLT instruction
    (void) instr; 
    halted = true;
}

void CPU::executeUSER(const Instruction& instr) { // Execute USER instruction
    if (instr.operands.size() == 1) { // USER instruction with one operand
        kernelMode = false;
        long a = instr.operands[0];
        if (!memory.isAddressValid(a, true)) {
            std::cerr << "Access violation: USER operand address not accessible" << std::endl;
            halted = true;
            return;
        }
        long jumpAddr = memory.read(a);
        if (!memory.isAddressValid(jumpAddr, false)) {
            std::cerr << "Access violation: USER jump address not accessible in user mode" << std::endl;
            halted = true;
            return;
        }
        memory.write(PC, jumpAddr);
    } else { // Default user mode
        kernelMode = false;
    }
}

void CPU::executeSYSCALL(const Instruction& instr) { // Execute SYSCALL instruction
    kernelMode = true;

    if (onContextSwitchOrSyscall) onContextSwitchOrSyscall();

    if (instr.syscallType == SyscallType::PRN) { // Handle PRN syscall
        int currentThreadId = memory.read(22); 
        int currentThreadTable = 500 + currentThreadId * 10;
        int currentThreadBlockTime = currentThreadTable + 1;
        memory.write(currentThreadTable + 3, 2); 
        memory.write(currentThreadBlockTime, memory.read(3));
        memory.write(0, memory.read(PC) + 1);
        std::cout << "Thread " << currentThreadId << ": Adress: " <<  instr.operands[0] << " :" << memory.read(instr.operands[0]) << std::endl;
        kernelMode = false; 
        return;
    } else if (instr.syscallType == SyscallType::HLT) { // Handle HLT syscall
        memory.write(4, memory.read(PC) + 1);
        memory.write(0, 430);
        return;
    } else if (instr.syscallType == SyscallType::YIELD) { // Handle YIELD syscall
        long pc = memory.read(PC);
        memory.write(4, pc + 1);
        memory.write(0, 400);
        return;
    } else {
        std::cerr << "Error: Unknown syscall type." << std::endl;
        halted = true;
    }
}

void CPU::setDebugCallback(std::function<void()> cb) { // Set callback for debug mode 3
    onContextSwitchOrSyscall = cb;
}

void CPU::printCurrentState() const {
    std::cerr << "CPU STATE" << std::endl;
    std::cerr << "PC: " << memory.read(PC) << std::endl;
    
    // Try to get the current instruction at PC
    long currentPC = memory.read(PC);
    auto instrIt = instructions.find(currentPC);
    if (instrIt != instructions.end()) {
        const Instruction& currentInstr = instrIt->second;
        std::cerr << "Instruction at: ";
        
        // Print instruction type
        switch(currentInstr.type) {
            case InstructionType::SET: std::cerr << "SET"; break;
            case InstructionType::CPY: std::cerr << "CPY"; break;
            case InstructionType::CPYI: std::cerr << "CPYI"; break;
            case InstructionType::CPYI2: std::cerr << "CPYI2"; break;
            case InstructionType::ADD: std::cerr << "ADD"; break;
            case InstructionType::ADDI: std::cerr << "ADDI"; break;
            case InstructionType::SUBI: std::cerr << "SUBI"; break;
            case InstructionType::JIF: std::cerr << "JIF"; break;
            case InstructionType::PUSH: std::cerr << "PUSH"; break;
            case InstructionType::POP: std::cerr << "POP"; break;
            case InstructionType::CALL: std::cerr << "CALL"; break;
            case InstructionType::RET: std::cerr << "RET"; break;
            case InstructionType::HLT: std::cerr << "HLT"; break;
            case InstructionType::USER: std::cerr << "USER"; break;
            case InstructionType::SYSCALL: 
                std::cerr << "SYSCALL ";
                switch(currentInstr.syscallType) {
                    case SyscallType::PRN: std::cerr << "PRN"; break;
                    case SyscallType::HLT: std::cerr << "HLT"; break;
                    case SyscallType::YIELD: std::cerr << "YIELD"; break;
                    default: std::cerr << "UNKNOWN"; break;
                }
                break;
            default: std::cerr << "UNKNOWN"; break;
        }
        
        // Print operands
        if (!currentInstr.operands.empty()) {
            for (size_t i = 0; i < currentInstr.operands.size(); ++i) {
                std::cerr << " " << currentInstr.operands[i];
            }
        }
        std::cerr << std::endl;
    } else {
        std::cerr << "No instruction found at PC " << currentPC << std::endl;
    }
    
    std::cerr << std::endl;
}