#pragma once

#include <vector>
#include <stdexcept>
#include <cstdint>
#include <iostream>

class Memory {
private:
    std::vector<long> data;
    const size_t USER_SPACE_START = 1000;

public:
    Memory(size_t size = 10000);
    long read(size_t address) const;
    void write(size_t address, long value);
    bool isAddressValid(size_t address, bool kernelMode) const;
    void loadData(const std::vector<std::pair<size_t, long>>& dataSegment);
    const std::vector<long>& getRawMemory() const;
};