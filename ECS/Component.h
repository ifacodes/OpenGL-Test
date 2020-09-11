#pragma once
#include <glm/glm.hpp>
#include <bitset>;

// component structures are declared in this header file

using namespace glm;

using Component_Type = std::uint8_t;
const Component_Type MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;

struct Transform
{
	vec3 position;
	vec3 scale;
};