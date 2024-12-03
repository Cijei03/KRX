#pragma once
#include "BasicTypes.hpp"

class Buffer
{
public:
    char* data{};
    TGui64 size{};
public:
    Buffer() = default;
    ~Buffer() = default;
    static Buffer create(void* data, TGui64 dataSize)
    {
        Buffer cache;
        cache.data = new char[dataSize];
        std::memcpy(cache.data, data, dataSize);
        cache.size = dataSize;

        return cache;
    }
    static void free(Buffer& ptr)
    {
        delete[] ptr.data;
        ptr.data = nullptr;
    }
};