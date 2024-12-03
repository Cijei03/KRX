#pragma once
#include "../include/internals/shaders.hpp"

using krxVertexShader = void(*)(const intern::ShaderInterface* Input, intern::ShaderInterface* Output, glm::vec4* SV_POSITION);
using krxFragmentShader = void(*)(const intern::ShaderInterface* Input, intern::ShaderInterface* Output);