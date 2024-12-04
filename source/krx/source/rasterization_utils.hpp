#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "../include/internals/view.hpp"
#include "shaders.hpp"

static glm::i16vec2 ndc_to_pixel_coord(const glm::f32vec4& SV_POSITION, const glm::i16vec2 ScreenSize)
{
	return glm::i16vec2((SV_POSITION.x * 0.5f + 0.5f) * ScreenSize.x, ScreenSize.y - ((SV_POSITION.y * 0.5f + 0.5f) * ScreenSize.y));
}
template<typename T>
static T min(T a, T b, T c)
{
    T cache = (a < b ? a : b);
    return (cache < c ? cache : c);
}
template<typename T>
static T max(T a, T b, T c)
{
    T cache = (a > b ? a : b);
    return (cache > c ? cache : c);
}

static int32_t edge_function(glm::i16vec2 p, glm::i16vec2 v0, glm::i16vec2 v1)
{
    return ((p.x - v0.x) * (v1.y - v0.y) - (p.y - v0.y) * (v1.x - v0.x));
}

static bool check_pixel_cw(int32_t* signs)
{
	return (signs[0] > 0 && signs[1] > 0 && signs[2] > 0);
}
static bool check_pixel_ccw(int32_t* signs)
{
	return (signs[0] < 0 && signs[1] < 0 && signs[2] < 0);
}