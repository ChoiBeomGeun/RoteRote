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

JUMP SHOULDN'T WORK WHILE PLAYER IS ON AIR

*/
#include "PlayerController.h"
#include "Input.h"
#include "Object.h"
#include "StateManager.h"
#include "Factory.h"
#include "Graphics.h"
#include <iostream>
#define SPEED 200.f
#define MAXSPEED 300.f
#define WALLJUMP 100.f
#define WALLATTACH 10.f
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
	JumpSpeed = 500.f;
	WallJump = WALLJUMP;

	JumpEnough = false;
	JumpTriggered = false;
	WallJumpTriggered = false;
	WallSlideMax = 150.f;
	FallSpeedMax = 400.f;
	maxAltitude = 40.f;
	WallAttached = false;
	/* delay for wall jump or wall off */
	OffFromWall = false;
	WallStickTime = .25f;
	timeToWallUnStick = 0.4f;

	//pos = this->GetOwner()->GetComponent<Transform>()->GetPosition();
	myController = GAMELOGIC->ControllerList.insert(GAMELOGIC->ControllerList.end(), this);
	STATEMANAGER->IsDrawing = true;
}

void PlayerController::Update(float dt)
{
	if (!(STATEMANAGER->b_IsPauseOn) && !STATEMANAGER->b_IsRot180 && !STATEMANAGER->b_IsRot90 && (!STATEMANAGER->b_IsReplay)
		&& (!STATEMANAGER->b_IsAutoplaying) && (!CAMERA->IsCameraShaking) && !STATEMANAGER->IsDrawing) {
		JumpInit();
		MaxJump();
		Movement(dt);
		PlayerAnimation();
	}
	else if (STATEMANAGER->b_IsRot180 || STATEMANAGER->b_IsRot90)
	{
		this->GetOwner()->GetComponent<Body>()->pm_velocity = glm::vec3(0, 0, 0);
	}

	//this->GetOwner()->GetComponent<Transform>()->SetPosition(pos);
}

void PlayerController::Movement(float dt)
{
	////std::cout << "GroundType: " << this->GetOwner()->GetComponent<Body>()->GroundType << '\n';

	WallAttached = false;

	if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Ground || WallAttached)
		WallJumpTriggered = false;	


	if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Left || this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Right)
		WallAttached = true;
	else
		WallAttached = false;

	if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Air)
	{
		OffFromWall = false;
		////std::cout << "OffFromWall = False" << '\n';
	}
	
	/* Make sure player is able to jump on ground while attached on wall */
	if (PHYSICS->GetIsPlayerGround() && WallAttached)
		this->GetOwner()->GetComponent<Body>()->GroundType = Grounded::Ground;

	
	if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Ground)
		FACTORY->GetPlayer()->GetComponent<Animation>()->isJumping = false;
	/*
	else if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Left || this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Right)
	{
		//std::cout << "Wall" << '\n';
		//std::cout << JumpEnough << '\n';
	}
	*/

	if (PHYSICS->GravityType == Gravity::y_Minus)
	{
		/* player not attached to wall */
		if (Input::IsPressed(SDL_SCANCODE_RIGHT) && !WallAttached)
		{
			FACTORY->GetPlayer()->GetComponent<Animation>()->setFlipX(false);
			FACTORY->GetPlayer()->GetComponent<Animation>()->setPressed(true);
			
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.x < MAXSPEED)
				this->GetOwner()->GetComponent<Body>()->pm_velocity += glm::vec3(SPEED, 0, 0);
		}
		else if (!FACTORY->GetPlayer()->GetComponent<Animation>()->isFlippedX())
			FACTORY->GetPlayer()->GetComponent<Animation>()->setPressed(false);
		/* player not attached to wall */
		if (Input::IsPressed(SDL_SCANCODE_LEFT) && !WallAttached)
		{
			FACTORY->GetPlayer()->GetComponent<Animation>()->setFlipX(true);
			FACTORY->GetPlayer()->GetComponent<Animation>()->setPressed(true);
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.x > -MAXSPEED)
			this->GetOwner()->GetComponent<Body>()->pm_velocity += glm::vec3(-SPEED, 0, 0);
		}
		else if (FACTORY->GetPlayer()->GetComponent<Animation>()->isFlippedX())
			FACTORY->GetPlayer()->GetComponent<Animation>()->setPressed(false);

		/*Wall Slide*/
		if ((this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Left || this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Right) && this->GetOwner()->GetComponent<Body>()->pm_velocity.y < 0)
		{
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.y < -WallSlideMax)
				this->GetOwner()->GetComponent<Body>()->pm_velocity.y = -WallSlideMax;

			//if (timeToWallUnStick > 0)
			//{
			//	this->GetOwner()->GetComponent<Body>()->pm_velocity.x = 0;

			//	if (OffFromWall)
			//	{
			//		timeToWallUnStick -= dt;
			//	}
			//	else
			//		timeToWallUnStick = WallStickTime;
			//} 
		}

		if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Left)
		{
			FACTORY->GetPlayer()->GetComponent<Animation>()->setFlipX(false);
			if (Input::IsPressed(SDL_SCANCODE_RIGHT))
			{
				if (Input::IsTriggered(SDL_SCANCODE_SPACE))
				{
					WallJumpTriggered = true;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = -1400;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = WallJump * 6;
				}
			}
			if (Input::IsPressed(SDL_SCANCODE_SPACE))
			{
				if (Input::IsPressed(SDL_SCANCODE_LEFT))
				{
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = -1000;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = WallJump * 8;
				}
			}
			else if (Input::IsTriggered(SDL_SCANCODE_LEFT))
			{
				OffFromWall = true;

				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = -SPEED * dt;
				this->GetOwner()->GetComponent<Body>()->pm_velocity.y = WallJump;
			}
		}
		if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Right)
		{
			FACTORY->GetPlayer()->GetComponent<Animation>()->setFlipX(true);
			if (Input::IsPressed(SDL_SCANCODE_LEFT))
			{
				if (Input::IsTriggered(SDL_SCANCODE_SPACE))
				{
					WallJumpTriggered = true;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = 1400;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = WallJump * 6;
				}
			}
			if (Input::IsPressed(SDL_SCANCODE_SPACE))
			{
				if (Input::IsPressed(SDL_SCANCODE_RIGHT))
				{
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = 1000;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = WallJump * 8;
				}
			}
			else if (Input::IsTriggered(SDL_SCANCODE_RIGHT))
			{
				OffFromWall = true;

				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = SPEED * dt;
				this->GetOwner()->GetComponent<Body>()->pm_velocity.y = WallJump;
			}
		}

		if (!JumpEnough)
		{
			if (Input::IsPressed(SDL_SCANCODE_SPACE) && this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Ground)
			{
				FACTORY->GetPlayer()->GetComponent<Animation>()->isJumping = true;
				this->GetOwner()->GetComponent<Body>()->pm_velocity = glm::vec3(0, JumpSpeed, 0);
				JumpTriggered = true;
			}
			if (Input::IsReleased(SDL_SCANCODE_SPACE))
			{
				FACTORY->GetPlayer()->GetComponent<Animation>()->isJumping = true;
				if (this->GetOwner()->GetComponent<Body>()->pm_velocity.y > minJumpVelocity)
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = minJumpVelocity;
			}
		}
		if (this->GetOwner()->GetComponent<Body>()->GroundType != Grounded::Ground)
		{
			
			////std::cout << "-50" << '\n';
			this->GetOwner()->GetComponent<Body>()->pm_velocity.y -= 50.f;
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.y < -FallSpeedMax)
				this->GetOwner()->GetComponent<Body>()->pm_velocity.y = -FallSpeedMax;
		}


		if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Air && !JumpTriggered && !WallJumpTriggered)
		{
			////std::cout << "-100" << '\n';
			this->GetOwner()->GetComponent<Body>()->pm_velocity.y -= 50.f;
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.y < -FallSpeedMax)
				this->GetOwner()->GetComponent<Body>()->pm_velocity.y = -FallSpeedMax;
		}
	}
	else if (PHYSICS->GravityType == Gravity::y_Plus)
	{
		if (Input::IsPressed(SDL_SCANCODE_RIGHT) && !WallAttached)
		{
			FACTORY->GetPlayer()->GetComponent<Animation>()->setFlipX(false);
			FACTORY->GetPlayer()->GetComponent<Animation>()->setPressed(true);
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.x > -MAXSPEED)
				this->GetOwner()->GetComponent<Body>()->pm_velocity += glm::vec3(-SPEED, 0, 0);
		}
		else if (!FACTORY->GetPlayer()->GetComponent<Animation>()->isFlippedX())
			FACTORY->GetPlayer()->GetComponent<Animation>()->setPressed(false);

		if (Input::IsPressed(SDL_SCANCODE_LEFT) && !WallAttached)
		{
			FACTORY->GetPlayer()->GetComponent<Animation>()->setFlipX(true);
			FACTORY->GetPlayer()->GetComponent<Animation>()->setPressed(true);
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.x < MAXSPEED)
				this->GetOwner()->GetComponent<Body>()->pm_velocity += glm::vec3(+SPEED, 0, 0);
		}
		else if (FACTORY->GetPlayer()->GetComponent<Animation>()->isFlippedX())
			FACTORY->GetPlayer()->GetComponent<Animation>()->setPressed(false);

		/*Wall Slide*/
		if ((this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Left || this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Right) && this->GetOwner()->GetComponent<Body>()->pm_velocity.y > 0)
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.y > WallSlideMax)
				this->GetOwner()->GetComponent<Body>()->pm_velocity.y = WallSlideMax;

		if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Left)
		{
			if (Input::IsPressed(SDL_SCANCODE_RIGHT))
			{
				if (Input::IsTriggered(SDL_SCANCODE_SPACE))
 				{
					WallJumpTriggered = true;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = 1400;
  					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = WallJump * 6;
				}
			}
			if (Input::IsPressed(SDL_SCANCODE_SPACE))
			{
				if (Input::IsPressed(SDL_SCANCODE_LEFT))
				{
					FACTORY->GetPlayer()->GetComponent<Animation>()->setPressed(true);
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = 1000;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = WallJump * 8;
				}
			}
			else if (Input::IsTriggered(SDL_SCANCODE_LEFT))
			{
				OffFromWall = true;
				FACTORY->GetPlayer()->GetComponent<Animation>()->setPressed(false);
				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = SPEED;
				this->GetOwner()->GetComponent<Body>()->pm_velocity.y = -WallJump;
			}
		}
		if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Right)
		{
			if (Input::IsPressed(SDL_SCANCODE_LEFT))
			{
				if (Input::IsTriggered(SDL_SCANCODE_SPACE))
				{
					WallJumpTriggered = true;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = -1400;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = WallJump * 6;
				}
			}
			if (Input::IsPressed(SDL_SCANCODE_SPACE))
			{
				if (Input::IsPressed(SDL_SCANCODE_RIGHT))
				{
					FACTORY->GetPlayer()->GetComponent<Animation>()->setPressed(true);
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = -1000;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = WallJump * 8;
				}
			}
			else if (Input::IsTriggered(SDL_SCANCODE_RIGHT))
			{
				OffFromWall = true;
				FACTORY->GetPlayer()->GetComponent<Animation>()->setPressed(false);
				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = -SPEED;
				this->GetOwner()->GetComponent<Body>()->pm_velocity.y = WallJump;
			}
		}

		if (!JumpEnough)
		{
			if (Input::IsPressed(SDL_SCANCODE_SPACE) && this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Ground)
			{
				FACTORY->GetPlayer()->GetComponent<Animation>()->isJumping = true;
 				this->GetOwner()->GetComponent<Body>()->pm_velocity = glm::vec3(0, JumpSpeed, 0);
				JumpTriggered = true;
			}
			if (Input::IsReleased(SDL_SCANCODE_SPACE))
			{
				if (this->GetOwner()->GetComponent<Body>()->pm_velocity.y < -minJumpVelocity)
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = -minJumpVelocity;
			}
		}
		if (this->GetOwner()->GetComponent<Body>()->GroundType != Grounded::Ground)
		{
			this->GetOwner()->GetComponent<Body>()->pm_velocity.y += 50.f;
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.y > FallSpeedMax)
				this->GetOwner()->GetComponent<Body>()->pm_velocity.y = FallSpeedMax;
		}


		if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Air && !JumpTriggered && !WallJumpTriggered)
		{
			this->GetOwner()->GetComponent<Body>()->pm_velocity.y += 50.f;
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.y > FallSpeedMax)
				this->GetOwner()->GetComponent<Body>()->pm_velocity.y = FallSpeedMax;
		}
	}
	else if (PHYSICS->GravityType == Gravity::x_Plus)
	{
		if (Input::IsPressed(SDL_SCANCODE_RIGHT) && !WallAttached)
		{
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.y < MAXSPEED)
				this->GetOwner()->GetComponent<Body>()->pm_velocity += glm::vec3(0, SPEED, 0);
		}

 		if (Input::IsPressed(SDL_SCANCODE_LEFT) && !WallAttached)
		{
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.y > -MAXSPEED)
				this->GetOwner()->GetComponent<Body>()->pm_velocity += glm::vec3(0, -SPEED, 0);
		}

		if ((this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Left || this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Right) && this->GetOwner()->GetComponent<Body>()->pm_velocity.y < 0)
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.x > WallSlideMax)
				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = WallSlideMax;

		if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Left)
		{
			if (Input::IsPressed(SDL_SCANCODE_RIGHT))
			{
				if (Input::IsTriggered(SDL_SCANCODE_SPACE))
				{
					WallJumpTriggered = true;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = -1400;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = WallJump * 6;
				}
			}
			if (Input::IsPressed(SDL_SCANCODE_SPACE))
			{
				if (Input::IsPressed(SDL_SCANCODE_LEFT))
				{
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = -1000;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = WallJump * 8;
				}
			}
			else if (Input::IsTriggered(SDL_SCANCODE_LEFT))
			{
				OffFromWall = true;

				this->GetOwner()->GetComponent<Body>()->pm_velocity.y = -SPEED;
				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = WallJump;
			}
		}
		if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Right)
		{
			if (Input::IsPressed(SDL_SCANCODE_LEFT))
			{
				if (Input::IsTriggered(SDL_SCANCODE_SPACE))
				{
					WallJumpTriggered = true;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = 1400;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = WallJump * 6;
				}
			}
			if (Input::IsPressed(SDL_SCANCODE_SPACE))
			{
				if (Input::IsPressed(SDL_SCANCODE_RIGHT))
				{
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = 1000;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = WallJump * 8;
				}
			}
			else if (Input::IsTriggered(SDL_SCANCODE_RIGHT))
			{
				OffFromWall = true;

				this->GetOwner()->GetComponent<Body>()->pm_velocity.y = SPEED;
				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = WallJump;
			}
		}

		if (!JumpEnough)
		{
			if (Input::IsPressed(SDL_SCANCODE_SPACE) && this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Ground)
			{
				this->GetOwner()->GetComponent<Body>()->pm_velocity = glm::vec3(JumpSpeed, 0, 0);
				JumpTriggered = true;
			}
			if (Input::IsReleased(SDL_SCANCODE_SPACE))
			{
				if (this->GetOwner()->GetComponent<Body>()->pm_velocity.x > minJumpVelocity)
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = minJumpVelocity;
			}
		}
		if (this->GetOwner()->GetComponent<Body>()->GroundType != Grounded::Ground)
		{
			this->GetOwner()->GetComponent<Body>()->pm_velocity.x += 50.f;
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.x > FallSpeedMax)
				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = FallSpeedMax;
		}


		if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Air && !JumpTriggered && !WallJumpTriggered)
		{
			this->GetOwner()->GetComponent<Body>()->pm_velocity.x += 50.f;
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.x > FallSpeedMax)
				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = FallSpeedMax;
		}
	}
	else if (PHYSICS->GravityType == Gravity::x_Minus)
	{
		if (Input::IsPressed(SDL_SCANCODE_RIGHT) && !WallAttached)
		{
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.y > -MAXSPEED)
			this->GetOwner()->GetComponent<Body>()->pm_velocity += glm::vec3(0, -SPEED, 0);
		}

		if (Input::IsPressed(SDL_SCANCODE_LEFT) && !WallAttached)
		{
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.y < MAXSPEED)
				this->GetOwner()->GetComponent<Body>()->pm_velocity += glm::vec3(0, SPEED, 0);
		}

		if ((this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Left || this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Right) && this->GetOwner()->GetComponent<Body>()->pm_velocity.y < 0)
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.x < -WallSlideMax)
				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = -WallSlideMax;

		if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Left)
		{
			if (Input::IsPressed(SDL_SCANCODE_RIGHT))
			{
				if (Input::IsTriggered(SDL_SCANCODE_SPACE))
				{
					WallJumpTriggered = true;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = 1400;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = WallJump * 6;
				}
			}
			if (Input::IsPressed(SDL_SCANCODE_SPACE))
			{
				if (Input::IsPressed(SDL_SCANCODE_LEFT))
				{
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = 1000;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = WallJump * 8;
				}
			}
			else if (Input::IsTriggered(SDL_SCANCODE_LEFT))
			{
				OffFromWall = true;

				this->GetOwner()->GetComponent<Body>()->pm_velocity.y = SPEED;
				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = WallJump;
			}
		}
		if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Right)
		{
			if (Input::IsPressed(SDL_SCANCODE_LEFT))
			{
				if (Input::IsTriggered(SDL_SCANCODE_SPACE))
				{
					WallJumpTriggered = true;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = -1400;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = WallJump * 6;
				}
			}
			if (Input::IsPressed(SDL_SCANCODE_SPACE))
			{
				if (Input::IsPressed(SDL_SCANCODE_RIGHT))
				{
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = -1000;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = WallJump * 8;
				}
			}
			else if (Input::IsTriggered(SDL_SCANCODE_RIGHT))
			{
				OffFromWall = true;

				this->GetOwner()->GetComponent<Body>()->pm_velocity.y = -SPEED;
				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = WallJump;
			}
		}

		if (!JumpEnough)
		{
			if (Input::IsPressed(SDL_SCANCODE_SPACE) && this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Ground)
			{
				this->GetOwner()->GetComponent<Body>()->pm_velocity = glm::vec3(JumpSpeed, 0, 0);
				JumpTriggered = true;
			}
			if (Input::IsReleased(SDL_SCANCODE_SPACE))
			{
				if (this->GetOwner()->GetComponent<Body>()->pm_velocity.x < -minJumpVelocity)
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = -minJumpVelocity;
			}
		}
		if (this->GetOwner()->GetComponent<Body>()->GroundType != Grounded::Ground)
		{
			this->GetOwner()->GetComponent<Body>()->pm_velocity.x -= 50.f;
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.x < -FallSpeedMax)
				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = -FallSpeedMax;
		}


		if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Air && !JumpTriggered && !WallJumpTriggered)
		{
			this->GetOwner()->GetComponent<Body>()->pm_velocity.x -= 50.f;
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.x < -FallSpeedMax)
				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = -FallSpeedMax;
		}
	}
}

void PlayerController::JumpInit()
{
	if (PHYSICS->gravity.y < 0)
	{
		if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Ground)
		{
			//printf("Not Air\n");
			pre_pos = this->GetOwner()->GetComponent<Body>()->m_pTransform->position.y;
		}
		delta_pos = this->GetOwner()->GetComponent<Body>()->m_pTransform->position.y - pre_pos;
		JumpSpeed = JUMP_SPEED;
		WallJump = WALLJUMP;
	}
	else if (PHYSICS->gravity.x < 0)
	{
		if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Ground)
		{
			pre_pos = this->GetOwner()->GetComponent<Body>()->m_pTransform->position.x;
		}
		delta_pos = this->GetOwner()->GetComponent<Body>()->m_pTransform->position.x - pre_pos;
		JumpSpeed = JUMP_SPEED;
		WallJump = WALLJUMP;
	}
	else if (PHYSICS->gravity.y > 0)
	{
		if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Ground)
		{
			pre_pos = this->GetOwner()->GetComponent<Body>()->m_pTransform->position.y;
		}
		delta_pos = pre_pos - this->GetOwner()->GetComponent<Body>()->m_pTransform->position.y;
		JumpSpeed = -JUMP_SPEED;
		WallJump = -WALLJUMP;
	}
	else if (PHYSICS->gravity.x > 0)
	{
		if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Ground)
		{
			pre_pos = this->GetOwner()->GetComponent<Body>()->m_pTransform->position.x;
		}
		delta_pos = pre_pos - this->GetOwner()->GetComponent<Body>()->m_pTransform->position.x;
		JumpSpeed = -JUMP_SPEED;
		WallJump = -WALLJUMP;
	}
}

void PlayerController::MaxJump()
{
	if (delta_pos >= maxAltitude)
	{
		////std::cout << "JumpEnough True" << '\n';
		JumpEnough = true;
	}
	else if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Ground)
	{
		////std::cout << "JumpEnough False" << '\n';
		////std::cout << "JumpTriggered False" << '\n';
		JumpEnough = false;
		JumpTriggered = false;
	}


	//if(JumpEnough && this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Air && 
}

void PlayerController::PlayerAnimation()
{
	/* player not attached to wall */
	if (Input::IsPressed(SDL_SCANCODE_RIGHT) && !WallAttached)
	{
		FACTORY->GetPlayer()->GetComponent<Animation>()->setFlipX(false);
		FACTORY->GetPlayer()->GetComponent<Animation>()->setPressed(true);
	}
	else if (!FACTORY->GetPlayer()->GetComponent<Animation>()->isFlippedX())
		FACTORY->GetPlayer()->GetComponent<Animation>()->setPressed(false);
	/* player not attached to wall */
	if (Input::IsPressed(SDL_SCANCODE_LEFT) && !WallAttached)
	{
		FACTORY->GetPlayer()->GetComponent<Animation>()->setFlipX(true);
		FACTORY->GetPlayer()->GetComponent<Animation>()->setPressed(true);
	}
	else if (FACTORY->GetPlayer()->GetComponent<Animation>()->isFlippedX())
		FACTORY->GetPlayer()->GetComponent<Animation>()->setPressed(false);

	if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Left)
	{
		FACTORY->GetPlayer()->GetComponent<Animation>()->setFlipX(false);
	}
	if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Right)
	{
		FACTORY->GetPlayer()->GetComponent<Animation>()->setFlipX(true);
	}

	if (!JumpEnough)
	{
		if (Input::IsPressed(SDL_SCANCODE_SPACE) && this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Ground)
			FACTORY->GetPlayer()->GetComponent<Animation>()->isJumping = true;
		if (Input::IsReleased(SDL_SCANCODE_SPACE))
			FACTORY->GetPlayer()->GetComponent<Animation>()->isJumping = true;
	}


	if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Air)
	{
		FACTORY->GetPlayer()->GetComponent<Animation>()->_isOnWall = false;
		FACTORY->GetPlayer()->GetComponent<Animation>()->setPressed(false);
	}
	else if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Left)
	{
		FACTORY->GetPlayer()->GetComponent<Animation>()->_isOnWall = true;
	}
	else if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Right)
	{
		FACTORY->GetPlayer()->GetComponent<Animation>()->_isOnWall = true;
	}
	else if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Ground)
	{
		FACTORY->GetPlayer()->GetComponent<Animation>()->_isOnWall = false;
	}
}

PlayerController::~PlayerController()
{
}