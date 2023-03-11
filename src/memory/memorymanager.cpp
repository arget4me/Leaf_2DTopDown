#include "memorymanager.h"

namespace LEAF
{
ArenaMemory CreateArenaMemory(uint32_t blockSizeBytes, uint32_t numBlocks)
{
    ArenaMemory arena;
    arena.blockByteSize = blockSizeBytes;
    arena.numBlocks = numBlocks;
    arena.byteBuffer = new uint8_t[blockSizeBytes * numBlocks];
    arena.nextBlock = 0;

    return arena;
}

MemoryManager::MemoryManager()
{
    arena.blockByteSize = 0;
    arena.numBlocks = 0;
    arena.nextBlock = 0;
    arena.byteBuffer = nullptr;
    loopAround = false;
}

MemoryManager& MemoryManager::operator=(MemoryManager&& other)
{
    this->arena.blockByteSize = other.arena.blockByteSize;
    this->arena.numBlocks = other.arena.numBlocks;
    this->arena.nextBlock = other.arena.nextBlock;
    this->arena.byteBuffer = other.arena.byteBuffer;
    this->loopAround = other.loopAround;

    other.arena.byteBuffer = nullptr;
    other.arena.numBlocks = 0;
    other.arena.nextBlock = 0;
    other.arena.blockByteSize = 0;

    return *this;
}

MemoryManager::MemoryManager(uint32_t blockSizeBytes, uint32_t numBlocks, bool loopAround)
{
    arena = CreateArenaMemory(blockSizeBytes, numBlocks);
    this->loopAround = loopAround;
}

MemoryManager::~MemoryManager()
{
    if(arena.byteBuffer)
    {
        delete arena.byteBuffer;
    }
}

MemoryBlock MemoryManager::RequestArenaMemory(uint32_t numBytes)
{
    uint32_t requiredBlocks = numBytes / arena.blockByteSize + ((numBytes % arena.blockByteSize) > 0);
    uint32_t remaningBlocks = arena.numBlocks - arena.nextBlock;
    if (arena.nextBlock >= arena.numBlocks)
    {
        return {0, nullptr};
    }

    if (requiredBlocks >= arena.numBlocks)
    {
        if(loopAround)
        {
            // Reset the loop around arena since it ran out of memory
            arena.nextBlock = 0;
        }
        return {0, nullptr};
    }
    
    if (requiredBlocks > remaningBlocks)
    {
        return {0, nullptr};
    }

    uint32_t updateNextBlock = arena.nextBlock + requiredBlocks;
    if(loopAround)
    {
        updateNextBlock %= arena.numBlocks;
    }

    uint8_t* rawMemory = (arena.byteBuffer + arena.blockByteSize * arena.nextBlock);
    MemoryBlock memoryBlock;
    memoryBlock.numBytes = arena.blockByteSize * requiredBlocks;
    memoryBlock.byteBuffer = rawMemory;

    arena.nextBlock = updateNextBlock;

    return memoryBlock;
}

}