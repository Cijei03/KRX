#pragma once
#include "BasicTypes.hpp"
#include <vector>

struct AttrInfo
{
    TGui32 bytesOffset{};
    TGui32 bytesStride{};
    AttrType type{};
    TGi8 ID = -1;
};

class BufferLayout
{
public:
    std::vector<AttrInfo> attrs;
public:
    void setAttrib(AttrType T, TGui32 attrOffset, TGui8 attrID, TGui32 attrStride)
    {
        // Check implementation limit is not exceeded, if yes - abort.
        if (attrID >= IMPLEMENTATION_MAX_ATTRIBS_LIMIT) return;
        // Check is current attrib is already added and just swap data.
        for (TGui8 i = 0; i < attrs.size(); i++)
        {
            if (this->attrs[i].ID == attrID)
            {
                this->attrs[i] = AttrInfo(attrOffset, attrStride, T, attrID);
                return;
            }
        }
        // If not, just add data.
        this->attrs.push_back(AttrInfo(attrOffset, attrStride, T, attrID));
    }

    BufferLayout() = default;
    ~BufferLayout() = default;
};