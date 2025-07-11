#include "memory.h"

Memory::Memory(size_t size) : data(size, 0) {
    // Initialize memory with zeros
}

long Memory::read(size_t address) const { // Read a value from memory at the specified address
    if (address >= data.size()) {
        throw std::out_of_range("Memory access out of bounds");
    }
    return data[address];
}

void Memory::write(size_t address, long value) { // Write a value to memory at the specified address
    if (address >= data.size()) {
        throw std::out_of_range("Memory access out of bounds");
    }
    data[address] = value;
}

bool Memory::isAddressValid(size_t address, bool kernelMode) const { // Check if the address is valid based on the current mode (kernel or user)
    if (kernelMode) {
        return address < data.size();
    }
    
    return ((address <= 20) || (address >= USER_SPACE_START)) && (address < data.size());
}

void Memory::loadData(const std::vector<std::pair<size_t, long>>& dataSegment) { // Load data into memory from a vector of pairs (address, value)
    for (const auto& entry : dataSegment) {
        write(entry.first, entry.second);
    }
}

const std::vector<long>& Memory::getRawMemory() const { // Get the raw memory data
    return data;
}