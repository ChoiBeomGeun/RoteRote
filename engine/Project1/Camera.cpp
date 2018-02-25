/**
\file        Camera.cpp
\author      HyunJun Yoo
\par         email: hyunjun306@gmail.com
\par         course: CS225
\date        12/11/2017
\brief

Camera class implementation.
It can set location of camera, where to look and zoom in and out.
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/

#include "Camera.h"
#include "Object.h"
#include "Factory.h"
#include <glm/vector_relational.hpp>

namespace TE
{
	Camera * CAMERA = nullptr;
}

using namespace TE;

Camera::Camera()
{
	CenterOfCamera = { 0.f, 0.f };
	cameraPos = glm::vec3(0, 0, 3);
	cameraTarget = glm::vec3(0, 0, -1);

	m_camerObject = new CameraObject();
	m_camerObject->scale = glm::vec3(1, 1, 0);
	DEBUG_ASSERT(CAMERA != nullptr, "Camera has more than one");
	CAMERA = this;
}
Camera::~Camera()
{

}

void Camera::Initialize()
{
	_zNear = 1.f;
	_zFar = 1000.f;
	angle = 45.f;
	_aspect = (float)APP->_screenWidth / (float)APP->_screenHeight;
	CAMERA->isCentered = true;
}
void TE::Camera::update()
{
	view = glm::lookAt(cameraPos, cameraTarget + cameraPos, cameraUp);

}

void TE::Camera::rotate(float rotAngle)
{
	//view = glm::rotate(view, angle, glm::vec3(0, 0, -1));
	//view = glm::rotate(glm::radians(angle), glm::vec3(1.f, 1.f, 0.f));

	if (rotAngle == 90)
	{
		// from 0
		if (cameraUp.x == 0 && cameraUp.y >= 1)
		{
			// to 90
			while (cameraUp.x <= 1.f && cameraUp.y >= 0.f)
			{
				cameraUp.x += .001f;
				cameraUp.y -= .001f;
			}
			cameraUp.x = 1.f;
			cameraUp.y = 0.f;
		}
		// from 90
		else if (cameraUp.x == 1 && cameraUp.y >= 0)
		{
			// to 180
			while (cameraUp.x >= 0 && cameraUp.y >= -1)
			{
				cameraUp.x -= .001f;
				cameraUp.y -= .001f;
			}
			cameraUp.x = 0.f;
			cameraUp.y = -1.f;
		}
		// from 180
		else if (cameraUp.x == 0 && cameraUp.y <= -1)
		{
			// to 270
			while (cameraUp.x <= -1 && cameraUp.y >= 0)
			{
				cameraUp.x -= .001f;
				cameraUp.y += .001f;
			}
			cameraUp.x = -1.f;
			cameraUp.y = 0.f;
		}
		// from 270
		else if (cameraUp.x <= -1 && cameraUp.y == 0)
		{
			// to 0
			while (cameraUp.x >= 0 && cameraUp.y <= 1)
			{
				cameraUp.x += 0.01f;
				cameraUp.y += 0.01f;
			}
			cameraUp.x = 0.0f;
			cameraUp.y = 1.0f;
		}
	}
}

void TE::Camera::proj()
{
	if (APP->_isfull)
	{
		if (/*APP->_resolution == Resolution::_960x720 ||*/ APP->_resolution == Resolution::_1280X960)
		{
			_aspect = (float)4 / 3;
			projection = glm::perspective(glm::radians(angle), _aspect, _zNear, _zFar);

		}
		else
		{
			_aspect = (float)APP->_screenWidth / (float)APP->_screenHeight;
			projection = glm::perspective(glm::radians(angle), _aspect, _zNear, _zFar);
		}
	}
	else
	{
		if (/*APP->_resolution == Resolution::_960x720 ||*/ APP->_resolution == Resolution::_1280X960)
		{
			_aspect = (float)4 / 3;
			projection = glm::perspective(glm::radians(angle), _aspect, _zNear, _zFar);
		}
		else
		{
			_aspect = (float)APP->_screenWidth / (float)APP->_screenHeight;
			projection = glm::perspective(glm::radians(angle), _aspect, _zNear, _zFar);
		}
	}
}

void TE::Camera::onPlayer(bool isOnPlayer)
{
	if (isOnPlayer)
	{
		CAMERA->cameraPos = FACTORY->GetPlayer()->GetComponent<Transform>()->position;
		CAMERA->cameraPos.z = 999.f;
		view = glm::lookAt(CAMERA->cameraPos, CAMERA->cameraTarget, CAMERA->cameraUp);
	}
}

void TE::Camera::followPlayer()
{
//	float PaceSpeed = 5.0f;
	glm::vec3 PaceDirection = FACTORY->GetPlayer()->GetComponent<Transform>()->position - cameraPos;
	glm::vec3 StartPosition{ 0.f };

}



void Camera::lookat(const glm::vec3& eye, const glm::vec3& target, const glm::vec3 & up)
{
	cameraPos = eye;
	cameraTarget = target;
	cameraUp = up;
	view = glm::lookAt(cameraPos, cameraTarget + cameraPos, cameraUp);
}

void Camera::Roate180(void)
{
	view = glm::rotate(view, glm::radians(180.f), glm::vec3(0.0f, 0.0f, .05f));
}

void TE::Camera::scale(glm::vec3 scale)
{
	view = glm::scale(view, scale);
}