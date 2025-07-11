#include "parser.h"

Parser::Parser() : parsingDataSection(false), parsingInstructionSection(false) {}

bool Parser::parseFile(const std::string& filePath) { // Parse the given file
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << filePath << std::endl;
        return false;
    }
    
    // Clear previous state
    dataSegment.clear();
    instructions.clear();
    instructionMap.clear();  // Clear instruction map too
    parsingDataSection = false;
    parsingInstructionSection = false;
    
    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines
        if (line.empty()) {
            continue;
        }
        
        // Check for section markers
        if (line.find("Begin Data Section") != std::string::npos) {
            parsingDataSection = true;
            parsingInstructionSection = false;
            continue;
        }
        else if (line.find("End Data Section") != std::string::npos) {
            parsingDataSection = false;
            continue;
        }
        else if (line.find("Begin Instruction Section") != std::string::npos) {
            parsingDataSection = false;
            parsingInstructionSection = true;
            continue;
        }
        else if (line.find("End Instruction Section") != std::string::npos) {
            parsingInstructionSection = false;
            continue;
        }
        
        // Process line based on current section
        if (parsingDataSection) {
            processDataLine(line);
        }
        else if (parsingInstructionSection) {
            processInstructionLine(line);
        }
    }
    
    return true;
}

void Parser::processDataLine(const std::string& line) { // Process a line in the data section
    std::string cleanLine = removeComment(line);
    std::vector<std::string> tokens = tokenize(cleanLine);
    
    if (tokens.size() < 2) {
        return;
    }
    
    try {
        size_t address = std::stoul(tokens[0]);
        long value = std::stol(tokens[1]);
        dataSegment.emplace_back(address, value);
    }
    catch (const std::exception& e) {
        std::cerr << "Error parsing data line: " << line << std::endl;
    }
}

void Parser::processInstructionLine(const std::string& line) { // Process a line in the instruction section
    std::string cleanLine = removeComment(line);
    std::vector<std::string> tokens = tokenize(cleanLine);
    
    if (tokens.size() < 2) {
        return;
    }
    
    try {
        size_t instrAddr = std::stoul(tokens[0]); 
        std::string instrType = tokens[1];
        
        InstructionType type;
        SyscallType syscallType;
        std::vector<long> operands;
        
        if (instrType == "SET") {
            type = InstructionType::SET;
            if (tokens.size() >= 4) {
                operands.push_back(std::stol(tokens[2])); // B: value
                operands.push_back(std::stol(tokens[3])); // A: address
            }
        }
        else if (instrType == "CPY") {
            type = InstructionType::CPY;
            if (tokens.size() >= 4) {
                operands.push_back(std::stol(tokens[2])); // A1: source
                operands.push_back(std::stol(tokens[3])); // A2: destination
            }
        }
        else if (instrType == "CPYI") {
            type = InstructionType::CPYI;
            if (tokens.size() >= 4) {
                operands.push_back(std::stol(tokens[2])); // A1: indirect source
                operands.push_back(std::stol(tokens[3])); // A2: destination
            }
        }
        else if (instrType == "CPYI2") {
            type = InstructionType::CPYI2;
            if (tokens.size() >= 4) {
                operands.push_back(std::stol(tokens[2])); // A1: double indirect source
                operands.push_back(std::stol(tokens[3])); // A2: destination
            }
        }
        else if (instrType == "ADD") {
            type = InstructionType::ADD;
            if (tokens.size() >= 4) {
                operands.push_back(std::stol(tokens[2])); // A: address
                operands.push_back(std::stol(tokens[3])); // B: value
            }
        }
        else if (instrType == "ADDI") {
            type = InstructionType::ADDI;
            if (tokens.size() >= 4) {
                operands.push_back(std::stol(tokens[2])); // A1: destination
                operands.push_back(std::stol(tokens[3])); // A2: source
            }
        }
        else if (instrType == "SUBI") {
            type = InstructionType::SUBI;
            if (tokens.size() >= 4) {
                operands.push_back(std::stol(tokens[2])); // A1: source
                operands.push_back(std::stol(tokens[3])); // A2: destination
            }
        }
        else if (instrType == "JIF") {
            type = InstructionType::JIF;
            if (tokens.size() >= 4) {
                operands.push_back(std::stol(tokens[2])); // A: address
                operands.push_back(std::stol(tokens[3])); // C: jump address
            }
        }
        else if (instrType == "PUSH") {
            type = InstructionType::PUSH;
            if (tokens.size() >= 3) {
                operands.push_back(std::stol(tokens[2])); // A: address
            }
        }
        else if (instrType == "POP") {
            type = InstructionType::POP;
            if (tokens.size() >= 3) {
                operands.push_back(std::stol(tokens[2])); // A: address
            }
        }
        else if (instrType == "CALL") {
            type = InstructionType::CALL;
            if (tokens.size() >= 3) {
                operands.push_back(std::stol(tokens[2])); // C: subroutine address
            }
        }
        else if (instrType == "RET") {
            type = InstructionType::RET;
        }
        else if (instrType == "HLT") {
            type = InstructionType::HLT;
        }
        else if (instrType == "USER") {
            type = InstructionType::USER;
            if (tokens.size() >= 3) {
                operands.push_back(std::stol(tokens[2])); // A: address register
            }
        }
        else if (instrType == "SYSCALL") {
            type = InstructionType::SYSCALL;
            if (tokens.size() >= 3) {
                std::string syscallTypeStr = tokens[2];
                
                if (syscallTypeStr == "PRN") {
                    syscallType = SyscallType::PRN;
                    if (tokens.size() >= 4) {
                        operands.push_back(std::stol(tokens[3])); // A: address
                    }
                }
                else if (syscallTypeStr == "HLT") {
                    syscallType = SyscallType::HLT;
                }
                else if (syscallTypeStr == "YIELD") {
                    syscallType = SyscallType::YIELD;
                }
                else {
                    std::cerr << "Unknown syscall type: " << syscallTypeStr << std::endl;
                    return;
                }
                
                Instruction instr(type, syscallType, operands);
                // Store instruction with its actual address as key
                instructionMap[instrAddr] = instr;
                instructions.push_back(instr); 
                return;
            }
        }
        else {
            std::cerr << "Unknown instruction type: " << instrType << std::endl;
            return;
        }
        
        // Create and add instruction with its actual address as key
        Instruction instr(type, operands);
        instructionMap[instrAddr] = instr;
        instructions.push_back(instr);
    }
    catch (const std::exception& e) {
        std::cerr << "Error parsing instruction line: " << line << std::endl;
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

std::string Parser::removeComment(const std::string& line) { // Remove comments from a line
    size_t commentPos = line.find('#');
    if (commentPos != std::string::npos) {
        return line.substr(0, commentPos);
    }
    return line;
}

std::vector<std::string> Parser::tokenize(const std::string& line) { // Tokenize a line into words
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;
    
    while (iss >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}

const std::vector<std::pair<size_t, long>>& Parser::getDataSegment() const { // Get the parsed data segment
    return dataSegment;
}

const std::vector<Instruction>& Parser::getInstructions() const { // Get the parsed instructions
    return instructions;
}

const std::map<size_t, Instruction>& Parser::getInstructionMap() const { // Get the instruction map
    return instructionMap;
}