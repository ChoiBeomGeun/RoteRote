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
static glm::vec3 eye = { 0,0,999 };
static glm::vec3 target{ 0,0,-1 };
static glm::vec3 up{ 0,1,0 };
Camera::Camera()
{
	CenterOfCamera = { 0.f, 0.f };
	//right = { 1,0,0 };
	//cameraTarget = glm::vec3(0, 0, -1);

	cameraPos = eye;
	cameraTarget = target;
	cameraUp = up;
	_zNear = 0.f;
	_zFar = 999.f;
	angle = 45.f;
	_aspect = (float)APP->_screenWidth / (float)APP->_screenHeight;
	//CAMERA->isCentered = true;
	_distance = (+_zNear + _zFar) *.5f;
	_width = tan(angle*.5f) * 2 * _distance;
	_height = _width / _aspect;

	m_camerObject = new CameraObject();
	m_camerObject->scale = glm::vec3(1, 1, 0);
	
	DEBUG_ASSERT(CAMERA != nullptr, "Camera has more than one");
	CAMERA = this;
}
Camera::~Camera()
{
	delete m_camerObject;
	m_camerObject = NULL;

}

void Camera::Initialize()
{
	cameraPos = eye;
	cameraTarget = target;
	cameraUp = up;
	_zNear = 0.f;
	_zFar = 999.f;
	angle = 45.f;
	_aspect = (float)APP->_screenWidth / (float)APP->_screenHeight;
	_distance = (+_zNear +_zFar) *.5f;
	_width = tan(angle*.5f) * 2 * _distance;
	_height = _width / _aspect;
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
		//4:3
		if (APP->_resolution == Resolution::_1280X960 || APP->_resolution == Resolution::_800X600 || APP->_resolution == Resolution::_1024X768)
		{
			_aspect = (float)APP->_screenWidth / (float)APP->_screenHeight;
			projection = glm::perspective(glm::radians(angle), _aspect, _zNear, _zFar);
		}
		//16:9
		else if (APP->_resolution == Resolution::_1280X720 || APP->_resolution == Resolution::_1920X1080 || APP->_resolution == Resolution::_1600X900)
		{
			_aspect = (float)APP->_screenWidth / (float)APP->_screenHeight;
			projection = glm::perspective(glm::radians(angle), _aspect, _zNear, _zFar);
		}
		else if (APP->_resolution == Resolution::_1680X1050)
		{
			_aspect = (float)APP->_screenWidth / (float)APP->_screenHeight;
			projection = glm::perspective(glm::radians(angle), _aspect, _zNear, _zFar);
		}
	}
	else
	{
		//4:3
		if (APP->_resolution == Resolution::_1280X960 || APP->_resolution == Resolution::_800X600 || APP->_resolution == Resolution::_1024X768)
		{ 
			_aspect = (float)APP->_screenWidth / (float)APP->_screenHeight;
			projection = glm::perspective(glm::radians(angle), _aspect, _zNear, _zFar);
		}
		//16:9
		else if(APP->_resolution == Resolution::_1280X720 || APP->_resolution == Resolution::_1920X1080 || APP->_resolution == Resolution::_1600X900)
		{
			_aspect = (float)APP->_screenWidth / (float)APP->_screenHeight;
			projection = glm::perspective(glm::radians(angle), _aspect, _zNear, _zFar);
		}
		else if(APP->_resolution == Resolution::_1680X1050)
		{
			_aspect = (float)APP->_screenWidth / (float)APP->_screenHeight;
			projection = glm::perspective(glm::radians(angle), _aspect, _zNear, _zFar);
		}
	}
}

void TE::Camera::unproj()
{
	if (APP->_isfull)
	{
		if (APP->_resolution == Resolution::_1280X960 || APP->_resolution == Resolution::_800X600 || APP->_resolution == Resolution::_1024X768)
		{
			_aspect = 1;
			projection = glm::ortho(-APP->_screenWidth *.5f, APP->_screenWidth *.5f, -APP->_screenHeight*.5f, APP->_screenHeight*.5f, _zNear, _zFar);

		}
		else
		{
			_aspect = 1;
			projection = glm::ortho(-APP->_screenWidth *.5f, APP->_screenWidth *.5f, -APP->_screenHeight*.5f, APP->_screenHeight*.5f, _zNear, _zFar);
		}
	}
	else
	{
		if (APP->_resolution == Resolution::_1280X960 || APP->_resolution == Resolution::_800X600 || APP->_resolution == Resolution::_1024X768)
		{
			_aspect = 1;
			projection = glm::ortho(-APP->_screenWidth *.5f, APP->_screenWidth *.5f, -APP->_screenHeight*.5f, APP->_screenHeight*.5f, _zNear, _zFar);
		}
		else if (APP->_resolution == Resolution::_1280X720 || APP->_resolution == Resolution::_1920X1080 || APP->_resolution == Resolution::_1600X900)
		{
			_aspect = 1;
			projection = glm::ortho(-APP->_screenWidth *.5f, APP->_screenWidth *.5f, -APP->_screenHeight*.5f, APP->_screenHeight*.5f, _zNear, _zFar);
		}
		else if (APP->_resolution == Resolution::_1680X1050)
		{
			_aspect = 1;
			projection = glm::ortho(-APP->_screenWidth *.5f, APP->_screenWidth *.5f, -APP->_screenHeight*.5f, APP->_screenHeight*.5f, _zNear, _zFar);
		}
	}
}


void TE::Camera::followPlayer()
{
//	float PaceSpeed = 5.0f;
	glm::vec3 PaceDirection = FACTORY->GetPlayer()->GetComponent<Transform>()->position - cameraPos;
	glm::vec3 StartPosition{ 0.f };

}

float TE::Camera::near_distance() const
{
	return _zNear;
}

float TE::Camera::far_distance() const
{
	return _zFar;
}

glm::vec3 TE::Camera::viewport_geometry() const
{
	return glm::vec3(_width, _height, _distance);
}

void Camera::Roate180(void)
{
	view = glm::rotate(view, glm::radians(180.f), glm::vec3(0.0f, 0.0f, .05f));
}

void TE::Camera::scale(glm::vec3 scale)
{
	view = glm::scale(view, scale);
}