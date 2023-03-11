#pragma once
#include <string>
#include "memory/memorymanager.h"

namespace LEAF
{

struct ImageSource
{
    std::string filepath;
};

struct Image
{
    uint64_t id;
    uint32_t width;
    uint32_t height;
    uint32_t channels;
    MemoryBlock buffer;
};

Image LoadImage(std::string filepath, MemoryManager& persistentMemory);

};