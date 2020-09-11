#pragma once

#include <glm/glm.hpp>
using namespace glm;

struct Camera
{
	vec3 mFront;
	vec3 mRight;
	vec3 mUp;
	float mPitch;
	float mYaw;
	float mZoom;
};