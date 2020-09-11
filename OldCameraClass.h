#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Game_Command.h"

const float YAW			= -90.0f;
const float PITCH		=  0.0f;
const float SPEED		=  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM		=  45.0f;

class Camera 
{
public:
	// constructors

	// vector
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : mFront(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM), deltaTime(0.0f)
	{
		mPosition = position;
		mWorldUp = up;
		mYaw = yaw;
		mPitch = pitch;
		// update the vectors
		updateCameraVectors();
	}
	// scalar
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : mFront(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM), deltaTime(0.0f)
	{
		mPosition = glm::vec3(posX, posY, posZ);
		mWorldUp = glm::vec3(upX, upY, upZ);
		mYaw = yaw;
		mPitch = pitch;
		// update the vectors
		updateCameraVectors();
	}

	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(mPosition, mPosition + mFront, mUp);
	}

	void getDeltaTime(float dT)
	{
		deltaTime = dT;
	}

	void ProcessKeyboardInput(Game_Command direction) 
	{		
		float velocity = movementSpeed * deltaTime;
		//std::cout << deltaTime << " " << velocity << "\n";
		if (direction == Game_Command::MOVE_FORWARD)
		{
			mPosition += mFront * velocity;
			//std::cout << mPosition.x << mPosition.y << mPosition.z << " " << mFront.x << mFront.y << mFront.z << " " << velocity << "\n";
		}
		if (direction == Game_Command::MOVE_BACKWARD)
		{ 		
			mPosition -= mFront * velocity;
		}			
		if (direction == Game_Command::MOVE_LEFT)
		{
			//std::cout << mPosition.x << mPosition.y << mPosition.z << " " << mRight.x << mRight.y << mRight.z << " " << velocity << "\n";
			mPosition -= mRight * velocity;
		}
		if (direction == Game_Command::MOVE_RIGHT)
		{
			//std::cout << mPosition.x << mPosition.y << mPosition.z << " " << mRight.x << mRight.y << mRight.z << " " << velocity << "\n";
			mPosition += mRight * velocity;
		}
	}

	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
	{
		//printf("mouse wow\n");
		xoffset *= mouseSensitivity;
		yoffset *= mouseSensitivity;

		mYaw	+= xoffset;
		mPitch	+= yoffset;

		if (constrainPitch)
		{
			if (mPitch > 89.0f)
				mPitch = 89.0f;
			if (mPitch < -89.0f)
				mPitch = -89.0f;
		}

		updateCameraVectors();
	}

	void ProcessMouseScroll(float yoffset) 
	{
		zoom -= (float)yoffset;
		if (zoom < 1.0f)
			zoom = 1.0f;
		if (zoom > 45.0f)
			zoom = 45.0f;
		
	}

	float Zoom()
	{
		return zoom;
	}

private:

	// private methods

	void updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
		front.y = sin(glm::radians(mPitch));
		front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
		mFront = glm::normalize(front);

		mRight = glm::normalize(glm::cross(mFront, mWorldUp));
		mUp	   = glm::normalize(glm::cross(mRight, mFront));
	}

	// private variables

	glm::vec3 mPosition;
	glm::vec3 mFront;
	glm::vec3 mUp;
	glm::vec3 mRight;
	glm::vec3 mWorldUp;

	float mYaw;
	float mPitch;

	float movementSpeed;
	float mouseSensitivity;
	float zoom;
	float deltaTime;
};