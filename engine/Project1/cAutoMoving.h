#pragma once
/**
\file        Sprite.h
\author      HyunJun Yoo
\par         email: hyunjun306@gmail.com
\par         course: CS225
\date        12/16/2017
\brief

Sprite component,
where sprite is defined.


All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once

#include "Component.h"

//#include "vector3.h"
#include "Transform.h"
#include  <string>

#include "glew.h"
#include "SDL.h"



namespace TE {
	struct Paths
	{
		glm::vec3 pathway;
		bool checkingPath;

	};
	class AutoMoving : public Component
	{
	public:
		AutoMoving();

		~AutoMoving();

		virtual void Initialize() override;

		std::vector <Paths> mPaths;


	};
}