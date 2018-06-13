/******************************************************************************/
/*!
\file	cAutoMoving.h
\author HyunJun Yoo
\par    email: hyunjun306@gmail.com
\par    course: CS225
\date   12/16/2017
\brief

AutoMoving header
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "Component.h"
#include "Transform.h"
#include  <string>
#include "glew.h"
#include "SDL.h"

namespace TE {
	struct Paths
	{
		glm::vec3 pathway;
		bool checkingPath = true;

	};
	class AutoMoving : public Component
	{
	public:
		AutoMoving();
		~AutoMoving();
		virtual void Initialize() override;
		glm::vec3 mOriginPosition;
		unsigned int NumberOfTowardPath = 0;
		std::vector <Paths> mPaths;
	};
}