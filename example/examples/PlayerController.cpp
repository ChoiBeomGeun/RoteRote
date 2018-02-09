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
#define SPEED 300.f
#define WALLATTACH 50.f
using namespace TE;

PlayerController::PlayerController()
{
}

void PlayerController::Initialize()
{
	
	//   pos = this->GetOwner()->GetComponent<Transform>()->GetPosition();
//	this->GetOwner()->GetComponent<Transform>() = GetOwner()->GetComponent<Transform>();
	//this->GetOwner()->GetComponent<Body>() =GetOwner()->GetComponent<Body>();

	pre_pos = 0;
	delta_pos = 0;
	JumpSpeed = 300.f;

	JumpEnough = false;

	maxAltitude = 50.f;
	//pos = this->GetOwner()->GetComponent<Transform>()->GetPosition();
	myController = GAMELOGIC->ControllerList.insert(GAMELOGIC->ControllerList.end(), this);
	STATEMANAGER->IsDrawing = true;
}

void PlayerController::Update(float dt)
{
	if (!(STATEMANAGER->b_IsPauseOn) && !STATEMANAGER->b_IsRot180 && !STATEMANAGER->b_IsRot90 && (!STATEMANAGER->b_IsReplay)
		&& (!STATEMANAGER->b_IsAutoplaying)&&(!CAMERA->IsCameraShaking) && !STATEMANAGER->IsDrawing) {
		JumpInit();
		MaxJump();
		Movement(dt);
	}
	else if (STATEMANAGER->b_IsRot180 || STATEMANAGER->b_IsRot90)
	{
		this->GetOwner()->GetComponent<Body>()->pm_velocity = glm::vec3(0, 0, 0);
	}

	//this->GetOwner()->GetComponent<Transform>()->SetPosition(pos);
}

void PlayerController::Movement(float /*dt*/)
{
	if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Left || this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Right)
		printf("Is Wall\n");
	if (Input::IsPressed(SDL_SCANCODE_RIGHT))
	{
		this->GetOwner()->GetComponent<Body>()->pm_velocity += glm::vec3(SPEED, 0, 0);
	}

	if (Input::IsPressed(SDL_SCANCODE_LEFT))
	{
		this->GetOwner()->GetComponent<Body>()->pm_velocity += glm::vec3(-SPEED, 0, 0);
	}
	
	if (!JumpEnough)
	{
		if (Input::IsPressed(SDL_SCANCODE_UP))
		{
			this->GetOwner()->GetComponent<Body>()->pm_velocity += glm::vec3(0, JumpSpeed, 0);
		}
	}
	else if(this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Air)
		this->GetOwner()->GetComponent<Body>()->pm_velocity.y -= 50.f;

	if (this->GetOwner()->GetComponent<Body>()->GroundType != Grounded::Ground && delta_pos == 0)
		this->GetOwner()->GetComponent<Body>()->pm_velocity.y -= 200.f;

	if (Input::IsPressed(SDL_SCANCODE_DOWN))
	{

	}
}

void PlayerController::JumpInit()
{
	if (PHYSICS->gravity.y < 0)
	{
		if (!(this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Air))
		{
			//printf("Not Air\n");
			pre_pos = this->GetOwner()->GetComponent<Body>()->m_pTransform->position.y;
		}
		delta_pos = this->GetOwner()->GetComponent<Body>()->m_pTransform->position.y - pre_pos;
		JumpSpeed = JUMP_SPEED;
	}
	else if (PHYSICS->gravity.x < 0)
	{
		if (!(this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Air))
		{
			pre_pos = this->GetOwner()->GetComponent<Body>()->m_pTransform->position.x;
		}
		delta_pos = this->GetOwner()->GetComponent<Body>()->m_pTransform->position.x - pre_pos;
		JumpSpeed = JUMP_SPEED;
	}
	else if (PHYSICS->gravity.y > 0)
	{
		if (!(this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Air))
		{
			pre_pos = this->GetOwner()->GetComponent<Body>()->m_pTransform->position.y;
		}
		delta_pos = pre_pos - this->GetOwner()->GetComponent<Body>()->m_pTransform->position.y;
		JumpSpeed = -JUMP_SPEED;
	}
	else if (PHYSICS->gravity.x > 0)
	{
		if (!(this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Air))
		{
			pre_pos = this->GetOwner()->GetComponent<Body>()->m_pTransform->position.x;
		}
		delta_pos = pre_pos - this->GetOwner()->GetComponent<Body>()->m_pTransform->position.x;
		JumpSpeed = -JUMP_SPEED;
	}
}

void PlayerController::MaxJump()
{
	if (delta_pos >= maxAltitude)
		JumpEnough = true;
	else if(this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Ground)
		JumpEnough = false;


	//if(JumpEnough && this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Air && 
}

PlayerController::~PlayerController()
{
}