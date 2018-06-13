/******************************************************************************/
/*!
\file        Camera.h
\author      HyunJun Yoo
\par         email: hyunjun306@gmail.com
\par         course: GAM250
\par		 ThumbUp Engine
\date        06/13/2018

this is camera class
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once



#ifndef CAMERA_H
#define CAMERA_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace TE {
	struct CameraObject {
		glm::vec3 pos;
		glm::vec3 scale;
	};
	class Camera
	{
	public:
		Camera();
		~Camera();

		void Initialize();
		void Roate180(void);
		void scale(glm::vec3 scale);
		void update();
		void rotate(float angle);
		void proj();
		void unproj();
		void followPlayer();

		glm::mat4 view;
		glm::vec3 cameraPos;
		glm::vec3 cameraTarget;
		glm::vec3 cameraUp;
		glm::vec2 CenterOfCamera;
		glm::mat4 projection;
		bool IsCameraShaking{ false };
		bool IsCameraAttached{ false };
		float angle;
		float near_distance() const;
		float far_distance() const;
		glm::vec3 viewport_geometry() const;
	private:
		
		float _zNear;
		float _zFar;
		float _aspect;
		float _distance;
		float _width, _height;
		glm::vec3 right;
		CameraObject * m_camerObject;
	};
	extern Camera * CAMERA;
}

#endif CAMERA_H// CAMERA_H