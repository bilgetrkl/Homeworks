#include "cpu.h"
#include "memory.h"
#include "instruction.h"
#include "parser.h"

void printUsage() { // Function to print usage instructions
    std::cout << "CPU Simulator" << std::endl;
    std::cout << "Usage: ./gtu_cpu <filename> -D <debug_mode>" << std::endl;
    std::cout << "       ./gtu_cpu <filename> <debug_mode>" << std::endl;
    std::cout << "Parameters:" << std::endl;
    std::cout << "  filename    - Program file to simulate (required)" << std::endl;
    std::cout << "  debug_mode  - Debug flag (0, 1, 2, or 3)" << std::endl;
    std::cout << "Debug modes:" << std::endl;
    std::cout << "  0 - Display memory contents to stderr after program halts" << std::endl;
    std::cout << "  1 - Display memory contents to stderr after each CPU instruction" << std::endl;
    std::cout << "  2 - Display memory contents to stderr after each instruction + wait for keypress" << std::endl;
    std::cout << "  3 - Display thread table to stderr after each context switch or system call" << std::endl;
    std::cout << std::endl;
}

void displayStatistics(CPU& cpu) { // Function to display CPU statistics
    std::cout << "\nCPU Statistics" << std::endl;
    std::cout << "Instruction count: " << cpu.getMemory().read(3) << std::endl;
}

void dumpNonZeroMemoryToStderr(const Memory& memory) { // Function to dump non-zero memory contents to stderr
    std::cerr << "MEMORY DUMP:" << std::endl;
    const auto& rawMemory = memory.getRawMemory();
    for (size_t i = 0; i < rawMemory.size(); ++i) {
        if (rawMemory[i] != 0) {
            std::cerr << "Memory[" << i << "]: " << rawMemory[i] << std::endl;
        }
    }
}

void dumpThreadTableToStderr(const Memory& memory) { // Function to dump the thread table to stderr
    std::cerr << "THREAD TABLE DUMP: " << std::endl;
    
    const std::string threadTableLabels[6] = { "ID", "Start time", "Count", "State", "PC", "SP"};
    
    for (int thread = 0; thread < 10; ++thread) {
        std::cerr << "Thread " << thread << ":" << std::endl;
        for (int offset = 0; offset < 6; ++offset) {
            size_t address = 500 + (thread * 10) + offset;
            std::cerr << "  Memory[" << address << "] (" << threadTableLabels[offset] << "): " << memory.read(address);
            
            // If it's the state field, print the state description
            if (offset == 3) {
                long state = memory.read(address);
                std::cerr << " (";
                switch (state) {
                    case 0: std::cerr << "ready"; break;
                    case 1: std::cerr << "running"; break;
                    case 2: std::cerr << "blocked"; break;
                    case 3: std::cerr << "terminated"; break;
                }
                std::cerr << ")";
            }
            
            std::cerr << std::endl;
        }
        std::cerr << std::endl;
    }
}

int main(int argc, char* argv[]) {
    std::cout << "CPU Simulator:" << std::endl;

    if (argc < 2) { // Check if the program filename is provided
        std::cerr << "Error: Program filename is required." << std::endl;
        printUsage();
        return 1;
    }
    
    std::string programFile;
    int debugMode = 1; // Default debug mode is 1
    
    if (argc >= 2) { // Parse command line arguments
        programFile = argv[1];
    }
    
    if (argc >= 3) {
        if (argc >= 4 && std::string(argv[2]) == "-D") {
            try {
                debugMode = std::stoi(argv[3]);
            } catch (const std::exception& e) {
                std::cerr << "Error: Invalid debug mode." << std::endl;
                printUsage();
                return 1;
            }
        } else {
            try {
                debugMode = std::stoi(argv[2]);
            } catch (const std::exception& e) {
                std::cerr << "Error: Invalid debug mode." << std::endl;
                printUsage();
                return 1;
            }
        }
    }
    
    if (debugMode < 0 || debugMode > 3) { // Validate debug mode
        std::cerr << "Error: Debug mode must be 0, 1, 2, or 3." << std::endl;
        printUsage();
        return 1;
    }
    
    std::cout << "Loading program from: " << programFile << std::endl;
    std::cout << "Debug mode: " << debugMode << std::endl;
    
    Parser parser;
    if (!parser.parseFile(programFile)) { // Parse the program file
        std::cerr << "Error: Failed to parse program file '" << programFile << "'." << std::endl;
        return 1;
    }
    
    std::cout << "Parsed " << parser.getInstructions().size() << " instructions and " 
              << parser.getDataSegment().size() << " data entries." << std::endl;
    
    // Initialize CPU and load the program
    CPU cpu;
    cpu.loadProgram(parser.getInstructionMap(), parser.getDataSegment());
    
    std::cout << "Starting CPU execution..." << std::endl;
    
    int cycleCount = 0;
    auto startTime = std::chrono::high_resolution_clock::now();
    
    while (!cpu.isHalted()) {
        cpu.execute();
        if (cpu.isHalted()) {
            break;
        }
        cycleCount++;
        
        if (debugMode == 1) { // Send memory contents to stderr after each instruction
            cpu.printCurrentState();
            dumpNonZeroMemoryToStderr(cpu.getMemory());
        }
        
        if (debugMode == 2) { // Send memory contents to stderr and wait for keypress
            cpu.printCurrentState();
            dumpNonZeroMemoryToStderr(cpu.getMemory());
            std::cout << "Press Enter to continue to next instruction..." << std::endl;
            std::cin.get();
        }
        
        if (debugMode == 3) { // Display thread table after each context switch or system call
            
            long currentPC = cpu.getMemory().read(0);
            auto instrIt = parser.getInstructionMap().find(currentPC);
            
            if (currentPC == 794) { // Check if it's a context switch instruction
                std::cerr << "CONTEXT SWITCH FINISHED AT PC: " << currentPC << std::endl;
                cpu.printCurrentState(); // Print current CPU state
                dumpThreadTableToStderr(cpu.getMemory());
            }
            
            // Check if it's a system call
            if (instrIt != parser.getInstructionMap().end() && instrIt->second.type == InstructionType::SYSCALL) { 
                std::cerr << "SYSTEM CALL DETECTED at PC=" << currentPC << std::endl;
                cpu.printCurrentState(); // Print current CPU state
                dumpThreadTableToStderr(cpu.getMemory());
            }
        }
        
        // Safety check to prevent infinite loops
        if (cycleCount > 100000) {
            std::cerr << "Warning: Execution exceeded 100,000 cycles." << std::endl;
            break;
        }
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    
    std::cout << "CPU halted after " << cycleCount << " cycles." << std::endl;
    std::cout << "Execution time: " << duration << " ms" << std::endl;
    
    // Debug mode 0: Send memory contents to stderr after CPU halts
    if (debugMode == 0) {
        dumpNonZeroMemoryToStderr(cpu.getMemory());
    }

    displayStatistics(cpu);
    
    return 0;
}