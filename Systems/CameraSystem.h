#pragma once

#include "System.h"
#include "Coordinator.h"
#include "../Components/Transform.h"
#include "../Components/Camera.h"

extern Coordinator gCoordinator;

// Camera System handles the input handling and updating the Camera Vectors

class CameraSystem : public System
{
public:
	void Init()
	{
		// arbitrary value for now
		speed = 2.5;
		worldUp = vec3(0.0f, 1.0f, 0.0f);
	}

	void Update(float dt)
	{
		for (auto& e : mEntities)
		{
			// for any Camera Entities, Get the Transform Component
			auto& transform = gCoordinator.GetComponent<Transform>(e);
			auto& camera = gCoordinator.GetComponent<Camera>(e);

			float velocity = speed * dt;

			// do the movement inputs


			
		}
	}
private:
	// some sort of message or event listener

	void UpdateCameraVectors()
	{
		for (auto& e : mEntities)
		{
			auto& camera = gCoordinator.GetComponent<Camera>(e);

			vec3 f;
			f.x = cos(glm::radians(camera.mYaw)) * cos(glm::radians(camera.mPitch));
			f.y = sin(glm::radians(camera.mPitch));
			f.z = sin(glm::radians(camera.mYaw)) * cos(glm::radians(camera.mPitch));
			camera.mFront = glm::normalize(f);

			camera.mRight = glm::normalize(glm::cross(camera.mFront, worldUp));
			camera.mUp = glm::normalize(glm::cross(camera.mRight, camera.mFront));

		}
	}

	float speed = 0;
	vec3 worldUp;
};