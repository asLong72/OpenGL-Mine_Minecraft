#pragma once
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "../include/glad/glad.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"

//#include <vector>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	SPACE,
	SHIFT,
};

const float YAW = 0.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVTY = 0.1f;
const float ZOOM = 45.0f;


class Camera {
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;

	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	//初始化
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM) {
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	//初始化
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM) {
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	//获得观察矩阵
	inline glm::mat4 GetViewMatrix() {
		return glm::lookAt(Position, Position + Front, Up);
	}
	//按键处理
	void ProcessKeyboard(Camera_Movement, float);
	//鼠标输入处理
	void ProcessMouseMovement(float, float, GLboolean);
	void ProcessMouseMovement(float, float);

	//变焦处理
	void ProcessMouseScroll(float);

private:
	//更新摄像机向量
	void updateCameraVectors();
};


#endif // !__CAMERA_H__
