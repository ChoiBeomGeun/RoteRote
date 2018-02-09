/**
\file        PlayerController.h
\author      KyungOok Park
\par         email: qkrruddn6680@gmail.com
\par         course: GAM200
\par	ThumbUp Engine
\date        12/12/2017
\brief
PlayerController state class.
*/
#pragma once
#include <iostream>
#include "Controller.h"
#include "Transform.h"

#define JUMP_SPEED 100.f

namespace TE

{
	class Body;
	class PlayerController :public Controller {
	public:
		PlayerController();
		~PlayerController();
		void Initialize() override;
		void Update(float dt) override;
		void Movement(float dt);
		void JumpInit();
		void MaxJump();

	private:
		glm::vec3 pos;
		Transform * transform;
		Body * body;

		float pre_pos;
		float maxAltitude;
		float JumpSpeed;

		float delta_pos;

		bool JumpEnough;

		bool GravityOn;
	};
}