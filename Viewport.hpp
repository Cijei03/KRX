#pragma once
#include "BasicTypes.hpp"

class Context;

class Viewport
{
protected:
    friend class Context;
    i16vec2 dimensions{};
    i16vec2 position{};
    Viewport() = default;
    ~Viewport() = default;
public:
    static Viewport* create(i16vec2 viewPosition, i16vec2 viewDimensions)
    {
        Viewport* viewPtr = new Viewport;
        viewPtr->dimensions = viewDimensions;
        viewPtr->position = viewPosition;

        return viewPtr;
    }
    static void free(Viewport** view)
    {
        Viewport* viewPtr = *view;
        delete viewPtr;
        view = nullptr;
    }
};