/**
\file        PlayerController.cpp
\author      KyungOok Park
\par         email: qkrruddn6680@gmail.com
\par         course: GAM200
\par		ThumbUp Engine
\date        12/12/2017
\brief
PlayerController, allow player to Jump and Move around
for every gravity change.
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include "PlayerController.h"
#include "Input.h"
#include "Object.h"
#include "StateManager.h"
#include "Factory.h"
#include "Graphics.h"
#include <iostream>
#define SPEED 40.f
using namespace TE;

PlayerController::PlayerController()
{
}

void PlayerController::Initialize()
{
	
	//   pos = transform->GetPosition();
	transform = dynamic_cast<Transform*>(GetOwner()->GetComponent(CT_TRANSFORM));
	body = dynamic_cast<Body*>(GetOwner()->GetComponent(CT_BODY));

	pre_pos = 0;
	delta_pos = 0;
	JumpSpeed = 100.f;

	maxAltitude = 80.f;
	//pos = transform->GetPosition();
	myController = GAMELOGIC->ControllerList.insert(GAMELOGIC->ControllerList.end(), this);
	STATEMANAGER->IsDrawing = true;
}

void PlayerController::Update(float dt)
{
	if (!(STATEMANAGER->b_IsPauseOn) && !STATEMANAGER->b_IsRot180 && !STATEMANAGER->b_IsRot90 && (!STATEMANAGER->b_IsReplay)
		&& (!STATEMANAGER->b_IsAutoplaying)&&(!CAMERA->IsCameraShaking) && !STATEMANAGER->IsDrawing) {
		JumpInit();
		Movement(dt);
	}
	else if (STATEMANAGER->b_IsRot180 || STATEMANAGER->b_IsRot90)
	{
		body->pm_velocity = glm::vec3(0, 0, 0);
	}

	//transform->SetPosition(pos);
}

void PlayerController::Movement(float /*dt*/)
{
	//std::cout << body->GroundType << std::endl;
	if (PHYSICS->gravity.y < 0)
	{
		if (body->GroundType == Grounded::Ground)
		{
			if (Input::IsPressed(SDL_SCANCODE_SPACE))
			{
				FACTORY->GetPlayer()->animation->isJumping = true;
				body->pm_velocity.y = JumpSpeed;
			}
			else
				FACTORY->GetPlayer()->animation->isJumping = false;

		}
		if (body->GroundType == Grounded::Left)
		{
			FACTORY->GetPlayer()->animation->setFlipX(false);
			if (Input::IsPressed(SDL_SCANCODE_RIGHT))
			{
				if (Input::IsPressed(SDL_SCANCODE_SPACE))
				{
					body->pm_velocity = glm::vec3(-250.f, JumpSpeed, 0);
				}
				else if (Input::IsPressed(SDL_SCANCODE_LEFT))
				{
					body->pm_velocity = glm::vec3(-300.f, JumpSpeed, 0);
				}
				else
				{
					body->pm_velocity = glm::vec3(SPEED, 0, 0);

				}
			}

		}
		if (body->GroundType == Grounded::Right)
		{
			FACTORY->GetPlayer()->animation->setFlipX(true);
			if (Input::IsPressed(SDL_SCANCODE_LEFT))
			{
				if (Input::IsPressed(SDL_SCANCODE_SPACE))
				{
					//FACTORY->GetPlayer()->animation->isJumping = true;
					body->pm_velocity = glm::vec3(250.f, JumpSpeed, 0);
					//body->pm_velocity += glm::vec3(0, JumpSpeed * 2, 0);
				}
				else if (Input::IsPressed(SDL_SCANCODE_RIGHT))
				{
					body->pm_velocity = glm::vec3(300.f, JumpSpeed, 0);
				}
				else
				{
					body->pm_velocity = glm::vec3(-SPEED, 0, 0);

				}
			}

		}
		if (delta_pos >= maxAltitude)
		{
			body->pm_velocity.y -= 20.f;
		}
		if (Input::IsTriggered(SDL_SCANCODE_S))
		{
			body->pm_velocity += glm::vec3(0.f, -SPEED, 0.f);
		}
		if (Input::IsPressed(SDL_SCANCODE_LEFT))
		{
			FACTORY->GetPlayer()->animation->setFlipX(true);
			FACTORY->GetPlayer()->animation->setPressed(true);
			if (body->pm_velocity.x >= -40)
				body->pm_velocity += glm::vec3(-SPEED, 0.f, 0.f);
		}
		else if (FACTORY->GetPlayer()->animation->isFlippedX())
			FACTORY->GetPlayer()->animation->setPressed(false);
		if (Input::IsPressed(SDL_SCANCODE_RIGHT))
		{
			FACTORY->GetPlayer()->animation->setFlipX(false);
			FACTORY->GetPlayer()->animation->setPressed(true);

			if (body->pm_velocity.x <= 40)
				body->pm_velocity += glm::vec3(SPEED, 0.f, 0.f);
		}
		else if (!FACTORY->GetPlayer()->animation->isFlippedX())
			FACTORY->GetPlayer()->animation->setPressed(false);
	}

	if (PHYSICS->gravity.x < 0)
	{
		if (body->GroundType == Grounded::Ground)
		{
			if (Input::IsPressed(SDL_SCANCODE_SPACE))
			{
				FACTORY->GetPlayer()->animation->isJumping = true;
				body->pm_velocity.x = JumpSpeed;
			}
			else
				FACTORY->GetPlayer()->animation->isJumping = false;
			//body->Jump = false;
		}
		if (body->GroundType == Grounded::Left)
		{
			//FACTORY->GetPlayer()->sprite->isFlipX = false;
			FACTORY->GetPlayer()->animation->setFlipX(false);
			if (Input::IsPressed(SDL_SCANCODE_RIGHT))
			{
				if (Input::IsPressed(SDL_SCANCODE_SPACE))
				{
					body->pm_velocity = glm::vec3(JumpSpeed, 250.f, 0);
					//body->pm_velocity += glm::vec3(JumpSpeed * 2, 0, 0);
				}
				else if (Input::IsPressed(SDL_SCANCODE_LEFT))
				{
					body->pm_velocity = glm::vec3(JumpSpeed, 300, 0);
				}
				else
				{
					body->pm_velocity = glm::vec3(0, -SPEED, 0);
				}
			}
		}
		if (body->GroundType == Grounded::Right)
		{
			if (Input::IsPressed(SDL_SCANCODE_LEFT))
			{
				if (Input::IsPressed(SDL_SCANCODE_SPACE))
				{
					body->pm_velocity = glm::vec3(JumpSpeed, -250.f, 0);
					//body->pm_velocity += glm::vec3(JumpSpeed * 2, 0, 0);
				}
				else if (Input::IsPressed(SDL_SCANCODE_RIGHT))
				{
					body->pm_velocity = glm::vec3(JumpSpeed, -300, 0);
				}
				else
				{
					body->pm_velocity = glm::vec3(0, SPEED, 0);
				}
			}
		}
		if (delta_pos >= maxAltitude)
		{
			body->pm_velocity.x -= 20.f;
		}
		if (Input::IsTriggered(SDL_SCANCODE_S))
		{
			body->pm_velocity.x += -SPEED;
		}
		if (Input::IsPressed(SDL_SCANCODE_LEFT))
		{
			//FACTORY->GetPlayer()->sprite->isFlipX = true;
			FACTORY->GetPlayer()->animation->setFlipX(true);
			FACTORY->GetPlayer()->animation->setPressed(true);
			if (body->pm_velocity.y <= 40)
				body->pm_velocity.y += SPEED;
		}
		else if (FACTORY->GetPlayer()->animation->isFlippedX())
			FACTORY->GetPlayer()->animation->setPressed(false);
		if (Input::IsPressed(SDL_SCANCODE_RIGHT))
		{
			//FACTORY->GetPlayer()->sprite->isFlipX = false;
			FACTORY->GetPlayer()->animation->setFlipX(false);
			FACTORY->GetPlayer()->animation->setPressed(true);
			if (body->pm_velocity.y >= -40)
				body->pm_velocity.y += -SPEED;
		}
		else if (!FACTORY->GetPlayer()->animation->isFlippedX())
			FACTORY->GetPlayer()->animation->setPressed(false);
	}

	if (PHYSICS->gravity.y > 0)
	{
		if (body->GroundType == Grounded::Ground)
		{
			if (Input::IsPressed(SDL_SCANCODE_SPACE))
			{
				FACTORY->GetPlayer()->animation->isJumping = true;
				body->pm_velocity.y = JumpSpeed;
			}
			else
				FACTORY->GetPlayer()->animation->isJumping = false;
			//body->Jump = false;
		}
		if (body->GroundType == Grounded::Left)
		{
			if (Input::IsPressed(SDL_SCANCODE_RIGHT))
			{
				if (Input::IsPressed(SDL_SCANCODE_SPACE))
				{
					body->pm_velocity = glm::vec3(250.f, JumpSpeed, 0);
					//body->pm_velocity += glm::vec3(0, JumpSpeed * 2, 0);
				}
				else if (Input::IsPressed(SDL_SCANCODE_LEFT))
				{
					body->pm_velocity = glm::vec3(300, JumpSpeed, 0);

					FACTORY->GetPlayer()->animation->setPressed(true);
				}
				else
				{
					body->pm_velocity = glm::vec3(-SPEED, 0, 0);
					FACTORY->GetPlayer()->animation->setPressed(false);
				}
			}
		}
		if (body->GroundType == Grounded::Right)
		{
			if (Input::IsPressed(SDL_SCANCODE_LEFT))
			{

				if (Input::IsPressed(SDL_SCANCODE_SPACE))
				{
					body->pm_velocity = glm::vec3(-250.f, JumpSpeed, 0);
					//body->pm_velocity += glm::vec3(0, JumpSpeed * 2, 0);
				}
				else if (Input::IsPressed(SDL_SCANCODE_RIGHT))
				{

					FACTORY->GetPlayer()->animation->setPressed(true);
					body->pm_velocity = glm::vec3(-300, JumpSpeed, 0);
				}
				else
				{
					body->pm_velocity = glm::vec3(SPEED, 0, 0);
					FACTORY->GetPlayer()->animation->setPressed(false);
				}
			}
		}
		if (delta_pos >= maxAltitude)
		{
			body->pm_velocity.y += 20.f;
		}
		if (Input::IsPressed(SDL_SCANCODE_S))
		{
			body->pm_velocity.y += SPEED;
		}
		if (Input::IsPressed(SDL_SCANCODE_LEFT))
		{
			//FACTORY->GetPlayer()->sprite->isFlipX = true;
			FACTORY->GetPlayer()->animation->setFlipX(true);
			FACTORY->GetPlayer()->animation->setPressed(true);
			if (body->pm_velocity.x <= 40)
				body->pm_velocity.x += SPEED;
		}
		else if (FACTORY->GetPlayer()->animation->isFlippedX())
			FACTORY->GetPlayer()->animation->setPressed(false);

		if (Input::IsPressed(SDL_SCANCODE_RIGHT))
		{
			//FACTORY->GetPlayer()->sprite->isFlipX = false;
			FACTORY->GetPlayer()->animation->setFlipX(false);
			FACTORY->GetPlayer()->animation->setPressed(true);
			if (body->pm_velocity.x >= -40)
				body->pm_velocity.x += -SPEED;
		}
		else if (!FACTORY->GetPlayer()->animation->isFlippedX())
			FACTORY->GetPlayer()->animation->setPressed(false);

	}

	if (PHYSICS->gravity.x > 0)
	{

		if (body->GroundType == Grounded::Ground)
		{
			if (Input::IsPressed(SDL_SCANCODE_SPACE))
			{
				FACTORY->GetPlayer()->animation->isJumping = true;
				body->pm_velocity.x = JumpSpeed;
			}
			else
				FACTORY->GetPlayer()->animation->isJumping = false;
			//body->Jump = false;
		}
		if (body->GroundType == Grounded::Left)
		{
			if (Input::IsPressed(SDL_SCANCODE_RIGHT))
			{
				if (Input::IsPressed(SDL_SCANCODE_SPACE))
				{
					body->pm_velocity = glm::vec3(JumpSpeed, -250.f, 0);
					//body->pm_velocity += glm::vec3(JumpSpeed * 2, 0, 0);
				}
				else if (Input::IsPressed(SDL_SCANCODE_LEFT))
				{
					body->pm_velocity = glm::vec3(JumpSpeed, -300, 0);
				}
				else
				{
					body->pm_velocity = glm::vec3(0, SPEED, 0);

				}
			}
		}
		if (body->GroundType == Grounded::Right)
		{

			if (Input::IsPressed(SDL_SCANCODE_LEFT))
			{
				if (Input::IsPressed(SDL_SCANCODE_SPACE))
				{
					body->pm_velocity = glm::vec3(JumpSpeed, 250.f, 0);
					//body->pm_velocity += glm::vec3(JumpSpeed * 2, 0, 0);
				}
				else if (Input::IsPressed(SDL_SCANCODE_RIGHT))
				{
					body->pm_velocity = glm::vec3(JumpSpeed, 300, 0);
				}
				else
				{
					body->pm_velocity = glm::vec3(0, -SPEED, 0);

				}
			}
		}
		if (delta_pos >= maxAltitude)
		{
			body->pm_velocity.x += 20.f;
		}
		if (Input::IsPressed(SDL_SCANCODE_S))
		{
			body->pm_velocity.x += SPEED;
		}
		if (Input::IsPressed(SDL_SCANCODE_LEFT))
		{
			//FACTORY->GetPlayer()->sprite->isFlipX = true;
			FACTORY->GetPlayer()->animation->setFlipX(true);
			FACTORY->GetPlayer()->animation->setPressed(true);
			if (body->pm_velocity.y >= -40)
				body->pm_velocity.y += -SPEED;
		}
		else if (FACTORY->GetPlayer()->animation->isFlippedX())
			FACTORY->GetPlayer()->animation->setPressed(false);
		if (Input::IsPressed(SDL_SCANCODE_RIGHT))
		{
			//FACTORY->GetPlayer()->sprite->isFlipX = false;
			FACTORY->GetPlayer()->animation->setFlipX(false);
			FACTORY->GetPlayer()->animation->setPressed(true);
			if (body->pm_velocity.y <= 40)
				body->pm_velocity.y += SPEED;
		}
		else if (!FACTORY->GetPlayer()->animation->isFlippedX())
			FACTORY->GetPlayer()->animation->setPressed(false);


	}
	if (body->GroundType == Grounded::Air)
	{
		FACTORY->GetPlayer()->animation->_isOnWall = false;
		FACTORY->GetPlayer()->animation->setPressed(false);
	}
	else if (body->GroundType == Grounded::Ground && !Input::IsAnyPressed())
	{
		body->pm_velocity = glm::vec3(0, 0, 0);
	}
	else if (body->GroundType == Grounded::Left)
	{
		FACTORY->GetPlayer()->animation->_isOnWall = true;
	}
	else if (body->GroundType == Grounded::Right)
	{
		FACTORY->GetPlayer()->animation->_isOnWall = true;
	}
	else if (body->GroundType == Grounded::Ground)
	{
		FACTORY->GetPlayer()->animation->_isOnWall = false;
	}
}

void PlayerController::JumpInit()
{
	if (PHYSICS->gravity.y < 0)
	{
		if (body->GroundType == Grounded::Ground || body->GroundType == Grounded::Left || body->GroundType == Grounded::Right)
		{
			pre_pos = body->m_pTransform->position.y;
		}
		delta_pos = body->m_pTransform->position.y - pre_pos;
		JumpSpeed = JUMP_SPEED;
	}
	else if (PHYSICS->gravity.x < 0)
	{
		if (body->GroundType == Grounded::Ground || body->GroundType == Grounded::Left || body->GroundType == Grounded::Right)
		{
			pre_pos = body->m_pTransform->position.x;
		}
		delta_pos = body->m_pTransform->position.x - pre_pos;
		JumpSpeed = JUMP_SPEED;
	}
	else if (PHYSICS->gravity.y > 0)
	{
		if (body->GroundType == Grounded::Ground || body->GroundType == Grounded::Left || body->GroundType == Grounded::Right)
		{
			pre_pos = body->m_pTransform->position.y;
		}
		delta_pos = pre_pos - body->m_pTransform->position.y;
		JumpSpeed = -JUMP_SPEED;
	}
	else if (PHYSICS->gravity.x > 0)
	{
		if (body->GroundType == Grounded::Ground || body->GroundType == Grounded::Left || body->GroundType == Grounded::Right)
		{
			pre_pos = body->m_pTransform->position.x;
		}
		delta_pos = pre_pos - body->m_pTransform->position.x;
		JumpSpeed = -JUMP_SPEED;
	}
}

PlayerController::~PlayerController()
{
}