#pragma once

#include "instruction.h"
#include <vector>
#include <string>
#include <utility>
#include <fstream>
#include <map>
#include <iostream>
#include <sstream>

class Parser {
private:
    bool parsingDataSection;
    bool parsingInstructionSection;
    std::vector<std::pair<size_t, long>> dataSegment;
    std::vector<Instruction> instructions; 
    std::map<size_t, Instruction> instructionMap; 
    
    void processDataLine(const std::string& line);
    void processInstructionLine(const std::string& line);
    std::string removeComment(const std::string& line);
    std::vector<std::string> tokenize(const std::string& line);

public:
    Parser();
    bool parseFile(const std::string& filePath);
    const std::vector<std::pair<size_t, long>>& getDataSegment() const;
    const std::vector<Instruction>& getInstructions() const;
    const std::map<size_t, Instruction>& getInstructionMap() const;
};