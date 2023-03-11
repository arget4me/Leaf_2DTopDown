#pragma once

#include <stdint.h>
#define KILOBYTE (1024)
#define MEGABYTE (1024 * KILOBYTE)
#define GIGABYTE (1024 * MEGABYTE)

namespace LEAF
{

// When requested gives the next block if reaching the end it will loop around and overwrite old.
struct ArenaMemory
{
    uint32_t blockByteSize;
    uint32_t numBlocks;
    uint32_t nextBlock;
    uint8_t* byteBuffer;
};

// Meant to borrow only. The MemoryManager manages this memory.
struct MemoryBlock
{
    uint32_t numBytes;
    uint8_t* byteBuffer;
};

class MemoryManager
{
public:
    MemoryManager();
    MemoryManager(uint32_t blockSizeBytes, uint32_t numBlocks, bool loopAround = false);
    ~MemoryManager();

    MemoryManager& operator=(MemoryManager&& other);

    MemoryBlock RequestArenaMemory(uint32_t numBytes);
private:
    bool loopAround;
    ArenaMemory arena;
};



}