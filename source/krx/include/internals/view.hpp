#pragma once
#include "texture2d.hpp"
#include "buffer.hpp"
#include <variant>

enum class krxTextureTarget
{
	COLOR_RENDER_TARGET_OUTPUT,
	DEPTH_RENDER_TARGET_OUTPUT,
};

enum class krxBufferTarget
{
	ATTRIBUTE_TARGET,
	SHADER_UNIFORM_TARGET
};

using krxViewTarget = std::variant<krxTextureTarget, krxBufferTarget>;

struct krxViewCreationInfo
{
	krxResource* Resource;
	krxViewTarget Target;
};

class krxPipelineLayout;
class krxContext;

class krxResourceView
{
private:
	friend class krxPipelineLayout;
	friend class krxContext;
	krxViewCreationInfo Info;
public:
	krxResourceView(const krxViewCreationInfo& CreationInfo);
};