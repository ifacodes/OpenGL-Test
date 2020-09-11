#pragma once

#include "../Components/Camera.h"
#include "../Components/Transform.h"
#include "../ECS/System.h"
#include "../ECS/Coordinator.h"

extern Coordinator gCoordinator;

// The Render System handles the rendering from the Camera

class RenderSystem : public System
{
public:

	void Init()
	{
		// We Create a Camera Entity
		mCamera = gCoordinator.CreateEntity();
		gCoordinator.AddComponent<Transform>(mCamera, Transform{ .position = vec3(0.0, 0.0, 0.0) });
		gCoordinator.AddComponent<Camera>(mCamera, Camera{ .mYaw = -90.0f, .mZoom = 45.0f });


	}

	void Update()
	{

	}

private:

	Entity mCamera;

};