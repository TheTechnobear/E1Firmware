/*
* Electra One MIDI Controller Firmware
* See COPYRIGHT file at the top of the source tree.
*
* This product includes software developed by the
* Electra One Project (http://electra.one/).
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.
*/

/**
 * @file MemoryBlock.h
 *
 * @brief A representation of a chunk of memory in E1 style memory pool.
 */

#pragma once

#include "Graphics.h"
#include "helpers.h"

class MemoryPool;

enum class MemoryBlockOutputType { Decimals, Chars };

class MemoryBlock
{
public:
    MemoryBlock();
    MemoryBlock(MemoryPool *memoryPool, uint32_t address);
    virtual ~MemoryBlock();

    int read(void);
    size_t readBytes(uint8_t *buffer, size_t length);
    size_t writeBytes(const uint8_t *buffer, size_t length);
    void seek(uint32_t address);
    uint32_t tell(void) const;
    void close();

    bool isAllocatedInPool(void) const;
    bool isEmpty(void) const;
    size_t getLength(void) const;
    MemoryBlock getRange(uint32_t rangeStart, uint32_t rangeLength) const;
    int peek(size_t position) const;

    // The size of the header must be divisible by 4
    static const size_t headerMaxSize = 56;

protected:
    MemoryPool *memoryPool;
    uint32_t startAddress;
    uint32_t currentAddress;
    uint32_t endAddress;
    size_t length;

    uint8_t header[headerMaxSize];
    size_t headerLength;
    bool isFullMessageInHeader;

private:
    size_t readBytesInternal(uint8_t *buffer, size_t length);
};
