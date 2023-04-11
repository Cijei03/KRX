#pragma once
#include "BasicTypes.hpp"
#include <memory>
#include "Buffer.hpp"
#include "BufferLayout.hpp"

class BufferIterator
{
public:
    struct InternalInfo
    {
        char* bufPtr{};
        TGui32 stride{};
    };
    using BuffersList = std::unique_ptr<Buffer*[]>;
    using LayoutsList = std::unique_ptr<BufferLayout*[]>;

    std::vector<InternalInfo> infos;
    void incrementIterator(TGui32 vCount)
    {
        for (TGui8 i = 0; i < this->infos.size(); i++)
        {
            this->infos[i].bufPtr += (this->infos[i].stride * vCount);
        }
    }
    void makeIteratable(const BuffersList& buffers, const LayoutsList& layouts, TGui32 vOffset)
    {
        this->infos.clear();
        for (TGui8 i = 0; i < IMPLEMENTATION_MAX_ATTRIBS_LIMIT; i++)
        {
            if (buffers[i] == nullptr) continue;
            char* basicBufAddress = buffers[i]->data;
            for (TGui8 attrIterator = 0; attrIterator < layouts[i]->attrs.size(); attrIterator++)
            {
                auto attrID = layouts[i]->attrs[attrIterator].ID;
                infos.push_back(InternalInfo(basicBufAddress + layouts[i]->attrs[attrIterator].bytesOffset, layouts[i]->attrs[attrIterator].bytesStride));
            }
        }
        this->incrementIterator(vOffset);
    }

    BufferIterator() = default;
    ~BufferIterator() = default;
};