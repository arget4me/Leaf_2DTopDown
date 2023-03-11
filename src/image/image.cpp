#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace LEAF
{

Image LoadImage(std::string filepath, MemoryManager& persistentMemory)
{
    static int nextId = 0;

    int width = 0;
    int height = 0;
    int channels = 0;
    uint8_t* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    int numBytes = width * height * channels;

    Image image;
    image.width = width;
    image.height = height;
    image.channels = channels;
    image.id = nextId++;
    image.buffer = persistentMemory.RequestArenaMemory(numBytes);
    if(image.buffer.byteBuffer)
    {
        if(memcpy_s(image.buffer.byteBuffer, image.buffer.numBytes, data, numBytes) != 0)
        {
            image.buffer.byteBuffer = nullptr;
            image.buffer.numBytes = 0;
            image.width = 0;
            image.height = 0;
        }
    }
    stbi_image_free(data);

    return image;
}

};