#pragma once
#include <vector>
#include <cstdint>

class krxResource
{
protected:
	std::vector<uint8_t> Data;
public:
	krxResource() = default;
	virtual ~krxResource() = default;
};