/******************************************************************************/
/*!
\file        PlayerController.cpp
\author      KyungOok Park
\par         email: qkrruddn6680@gmail.com
\par         course: GAM250
\par		 ThumbUp Engine
\date        12/12/2017

\brief
PlayerController, allow player to Jump and Move around
for every gravity change.
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "PlayerController.h"
#include "SoundManager.h"
#include "Input.h"
#include "Object.h"
#include "StateManager.h"
#include "Factory.h"
#include "Graphics.h"
#include <iostream>
#define SPEED 4000.f
#define MAXSPEED 250.f
#define WALLJUMP 500.f
#define WALLATTACH 10.f
using namespace TE;

PlayerController::PlayerController()
{
}

void PlayerController::Initialize()
{
	pre_pos = 0;
	delta_pos = 0;
	JumpSpeed = 100.f;
	WallJump = WALLJUMP;

	/* verifies whether player is on attachable wall or not */
	IsAttachable = false;

	JumpEnough = false;
	JumpTriggered = false;
	WallJumpTriggered = false;
	WallSlideMax = 150.f;
	FallSpeedMax = 500.f;
	maxAltitude = 40.f;
	WallAttached = false;
	/* delay for wall jump or wall off */
	OffFromWall = false;
	WallStickTime = .25f;
	timeToWallUnStick = 0.4f;

	myController = GAMELOGIC->ControllerList.insert(GAMELOGIC->ControllerList.end(), this);
	STATEMANAGER->IsDrawing = true;

	JumpSound = SOUNDMANAGER->LoadSound("jump.mp3");
	SlideSound = SOUNDMANAGER->LoadSound("slide.mp3"); 

	IsSlideSoundOn = false;
}

void PlayerController::Update(float dt)
{
	if (APP->IsKeyBoardAvailable)
	{
		if (!APP->b_Win && !(STATEMANAGER->b_IsPauseOn) && !STATEMANAGER->b_IsRot180 && !STATEMANAGER->b_IsRot90 && (!STATEMANAGER->b_IsReplay)
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
	}
}

void PlayerController::Movement(float dt)
{
	WallAttached = false;

	if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Ground || WallAttached)
		WallJumpTriggered = false;


	if ((this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Left || this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Right) && IsAttachable)
	{
		WallAttached = true;
	}
	else
	{
		WallAttached = false;
	}

	if (WallAttached)
	{
		if (!IsSlideSoundOn)
		{
			IsSlideSoundOn = true;
			SOUNDMANAGER->PlaySounds(SlideSound, true);
		}
	}
	else
	{
		SOUNDMANAGER->StopSound(SlideSound);
		IsSlideSoundOn = false;
	}

	if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Air)
	{
		OffFromWall = false;
	}

	/* Make sure player is able to jump on ground while attached on wall */
	if (PHYSICS->GetIsPlayerGround() && WallAttached)
		this->GetOwner()->GetComponent<Body>()->GroundType = Grounded::Ground;

	if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Ground)
		FACTORY->GetPlayer()->GetComponent<Animation>()->isJumping = false;


	if (PHYSICS->GravityType == Gravity::y_Minus)
	{
		/* player not attached to wall */
		if (Input::IsPressed(SDL_SCANCODE_RIGHT) && !WallAttached)
		{
			FACTORY->GetPlayer()->GetComponent<Animation>()->setFlipX(false);
			FACTORY->GetPlayer()->GetComponent<Animation>()->setPressed(true);

			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.x < MAXSPEED)
				this->GetOwner()->GetComponent<Body>()->pm_velocity += glm::vec3(SPEED, 0, 0) * dt;
			else
				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = MAXSPEED;
		}
		else if (!FACTORY->GetPlayer()->GetComponent<Animation>()->isFlippedX())
			FACTORY->GetPlayer()->GetComponent<Animation>()->setPressed(false);
		/* player not attached to wall */
		if (Input::IsPressed(SDL_SCANCODE_LEFT) && !WallAttached)
		{
			FACTORY->GetPlayer()->GetComponent<Animation>()->setFlipX(true);
			FACTORY->GetPlayer()->GetComponent<Animation>()->setPressed(true);
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.x > -MAXSPEED)
				this->GetOwner()->GetComponent<Body>()->pm_velocity += glm::vec3(-SPEED, 0, 0) * dt;
		}
		else if (FACTORY->GetPlayer()->GetComponent<Animation>()->isFlippedX())
			FACTORY->GetPlayer()->GetComponent<Animation>()->setPressed(false);

		/*Wall Slide*/
		if ((this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Left || this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Right) && this->GetOwner()->GetComponent<Body>()->pm_velocity.y < 0)
		{
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.y < -WallSlideMax)
				this->GetOwner()->GetComponent<Body>()->pm_velocity.y = -WallSlideMax;
		}

		if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Left)
		{
			FACTORY->GetPlayer()->GetComponent<Animation>()->setFlipX(false);
			if (Input::IsPressed(SDL_SCANCODE_RIGHT))
			{
				if (Input::IsTriggered(SDL_SCANCODE_SPACE) && IsAttachable)
				{
					WallJumpTriggered = true;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = -WallJump;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = 600;
				}
			}
			if (Input::IsPressed(SDL_SCANCODE_SPACE) && IsAttachable)
			{
				if (Input::IsPressed(SDL_SCANCODE_LEFT))
				{
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = -700;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = WallJump;
				}
			}
			else if (Input::IsTriggered(SDL_SCANCODE_LEFT))
			{
				OffFromWall = true;

				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = -WallJump * 0.5f;
				this->GetOwner()->GetComponent<Body>()->pm_velocity.y = WallJump * 0.5f;
			}
		}
		if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Right)
		{
			FACTORY->GetPlayer()->GetComponent<Animation>()->setFlipX(true);
			if (Input::IsPressed(SDL_SCANCODE_LEFT))
			{
				if (Input::IsTriggered(SDL_SCANCODE_SPACE) && IsAttachable)
				{
					WallJumpTriggered = true;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = WallJump;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = 600;
				}
			}
			if (Input::IsPressed(SDL_SCANCODE_SPACE) && IsAttachable)
			{
				if (Input::IsPressed(SDL_SCANCODE_RIGHT))
				{
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = 700;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = WallJump;
				}
			}
			else if (Input::IsTriggered(SDL_SCANCODE_RIGHT))
			{
				OffFromWall = true;

				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = WallJump * 0.5f;
				this->GetOwner()->GetComponent<Body>()->pm_velocity.y = WallJump * 0.5f;
			}
		}

		if (!JumpEnough)
		{
			if (Input::IsPressed(SDL_SCANCODE_SPACE) && this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Ground)
			{
				FACTORY->GetPlayer()->GetComponent<Animation>()->isJumping = true;
				this->GetOwner()->GetComponent<Body>()->pm_velocity = glm::vec3(0, JumpSpeed, 0);
				JumpTriggered = true;
				SOUNDMANAGER->PlaySounds(JumpSound, false);
			}
		}
		if (this->GetOwner()->GetComponent<Body>()->GroundType != Grounded::Ground)
		{
			this->GetOwner()->GetComponent<Body>()->pm_velocity.y -= 2500.f * dt;
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.y < -FallSpeedMax)
				this->GetOwner()->GetComponent<Body>()->pm_velocity.y = -FallSpeedMax;
		}


		if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Air && !JumpTriggered && !WallJumpTriggered)
		{
			this->GetOwner()->GetComponent<Body>()->pm_velocity.y -= 2500.f * dt;
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
				this->GetOwner()->GetComponent<Body>()->pm_velocity += glm::vec3(-SPEED, 0, 0) * dt;
		}
		else if (!FACTORY->GetPlayer()->GetComponent<Animation>()->isFlippedX())
			FACTORY->GetPlayer()->GetComponent<Animation>()->setPressed(false);

		if (Input::IsPressed(SDL_SCANCODE_LEFT) && !WallAttached)
		{
			FACTORY->GetPlayer()->GetComponent<Animation>()->setFlipX(true);
			FACTORY->GetPlayer()->GetComponent<Animation>()->setPressed(true);
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.x < MAXSPEED)
				this->GetOwner()->GetComponent<Body>()->pm_velocity += glm::vec3(+SPEED, 0, 0) * dt;
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
				if (Input::IsTriggered(SDL_SCANCODE_SPACE) && IsAttachable)
				{
					WallJumpTriggered = true;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = -WallJump;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = -600;
				}
			}
			if (Input::IsPressed(SDL_SCANCODE_SPACE) && IsAttachable)
			{
				if (Input::IsPressed(SDL_SCANCODE_LEFT))
				{
					FACTORY->GetPlayer()->GetComponent<Animation>()->setPressed(true);
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = 700;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = WallJump;
				}
			}
			else if (Input::IsTriggered(SDL_SCANCODE_LEFT))
			{
				OffFromWall = true;
				FACTORY->GetPlayer()->GetComponent<Animation>()->setPressed(false);
				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = -WallJump * 0.5f;
				this->GetOwner()->GetComponent<Body>()->pm_velocity.y = WallJump * 0.5f;
			}
		}
		if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Right)
		{
			if (Input::IsPressed(SDL_SCANCODE_LEFT))
			{
				if (Input::IsTriggered(SDL_SCANCODE_SPACE) && IsAttachable)
				{
					WallJumpTriggered = true;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = WallJump;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = -600;
				}
			}
			if (Input::IsPressed(SDL_SCANCODE_SPACE) && IsAttachable)
			{
				if (Input::IsPressed(SDL_SCANCODE_RIGHT))
				{
					FACTORY->GetPlayer()->GetComponent<Animation>()->setPressed(true);
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = -700;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = WallJump;
				}
			}
			else if (Input::IsTriggered(SDL_SCANCODE_RIGHT))
			{
				OffFromWall = true;
				FACTORY->GetPlayer()->GetComponent<Animation>()->setPressed(false); 
				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = WallJump * 0.5f;
				this->GetOwner()->GetComponent<Body>()->pm_velocity.y = WallJump * 0.5f;
			}
		}

		if (!JumpEnough)
		{
			if (Input::IsPressed(SDL_SCANCODE_SPACE) && this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Ground)
			{
				FACTORY->GetPlayer()->GetComponent<Animation>()->isJumping = true;
				this->GetOwner()->GetComponent<Body>()->pm_velocity = glm::vec3(0, JumpSpeed, 0);
				JumpTriggered = true;
				SOUNDMANAGER->PlaySounds(JumpSound, false);
			}
		}
		if (this->GetOwner()->GetComponent<Body>()->GroundType != Grounded::Ground)
		{
			this->GetOwner()->GetComponent<Body>()->pm_velocity.y += 2500.f * dt;
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.y > FallSpeedMax)
				this->GetOwner()->GetComponent<Body>()->pm_velocity.y = FallSpeedMax;
		}


		if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Air && !JumpTriggered && !WallJumpTriggered)
		{
			this->GetOwner()->GetComponent<Body>()->pm_velocity.y += 2500.f * dt;
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.y > FallSpeedMax)
				this->GetOwner()->GetComponent<Body>()->pm_velocity.y = FallSpeedMax;
		}
	}
	else if (PHYSICS->GravityType == Gravity::x_Plus)
	{
		if (Input::IsPressed(SDL_SCANCODE_RIGHT) && !WallAttached)
		{
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.y < MAXSPEED)
				this->GetOwner()->GetComponent<Body>()->pm_velocity += glm::vec3(0, SPEED, 0) * dt;
		}

		if (Input::IsPressed(SDL_SCANCODE_LEFT) && !WallAttached)
		{
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.y > -MAXSPEED)
				this->GetOwner()->GetComponent<Body>()->pm_velocity += glm::vec3(0, -SPEED, 0) * dt;
		}

		if ((this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Left || this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Right) && this->GetOwner()->GetComponent<Body>()->pm_velocity.x > 0)
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.x > WallSlideMax)
				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = WallSlideMax;

		if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Left)
		{
			if (Input::IsPressed(SDL_SCANCODE_RIGHT))
			{
				if (Input::IsTriggered(SDL_SCANCODE_SPACE) && IsAttachable)
				{
					WallJumpTriggered = true;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = WallJump;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = -600;
				}
			}
			if (Input::IsPressed(SDL_SCANCODE_SPACE) && IsAttachable)
			{
				if (Input::IsPressed(SDL_SCANCODE_LEFT))
				{
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = -700;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = WallJump;
				}
			}
			else if (Input::IsTriggered(SDL_SCANCODE_LEFT))
			{
				OffFromWall = true;

				this->GetOwner()->GetComponent<Body>()->pm_velocity.y = WallJump * 0.5f;
				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = WallJump * 0.5f;
			}
		}
		if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Right)
		{
			if (Input::IsPressed(SDL_SCANCODE_LEFT))
			{
				if (Input::IsTriggered(SDL_SCANCODE_SPACE) && IsAttachable)
				{
					WallJumpTriggered = true;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = -WallJump;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = -600;
				}
			}
			if (Input::IsPressed(SDL_SCANCODE_SPACE) && IsAttachable)
			{
				if (Input::IsPressed(SDL_SCANCODE_RIGHT))
				{
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = 700;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = WallJump;
				}
			}
			else if (Input::IsTriggered(SDL_SCANCODE_RIGHT))
			{
				OffFromWall = true;

				this->GetOwner()->GetComponent<Body>()->pm_velocity.y = -WallJump * 0.5f;
				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = WallJump * 0.5f;
			}
		}

		if (!JumpEnough)
		{
			if (Input::IsPressed(SDL_SCANCODE_SPACE) && this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Ground)
			{
				this->GetOwner()->GetComponent<Body>()->pm_velocity = glm::vec3(JumpSpeed, 0, 0);
				JumpTriggered = true;
				SOUNDMANAGER->PlaySounds(JumpSound, false);
			}
		}
		if (this->GetOwner()->GetComponent<Body>()->GroundType != Grounded::Ground)
		{
			this->GetOwner()->GetComponent<Body>()->pm_velocity.x += 2500.f * dt;
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.x > FallSpeedMax)
				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = FallSpeedMax;
		}


		if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Air && !JumpTriggered && !WallJumpTriggered)
		{
			this->GetOwner()->GetComponent<Body>()->pm_velocity.x += 2500.f * dt;
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.x > FallSpeedMax)
				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = FallSpeedMax;
		}
	}
	else if (PHYSICS->GravityType == Gravity::x_Minus)
	{
		if (Input::IsPressed(SDL_SCANCODE_RIGHT) && !WallAttached)
		{
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.y > -MAXSPEED)
				this->GetOwner()->GetComponent<Body>()->pm_velocity += glm::vec3(0, -SPEED, 0) * dt;
		}

		if (Input::IsPressed(SDL_SCANCODE_LEFT) && !WallAttached)
		{
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.y < MAXSPEED)
				this->GetOwner()->GetComponent<Body>()->pm_velocity += glm::vec3(0, SPEED, 0) * dt;
		}

		if ((this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Left || this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Right) && this->GetOwner()->GetComponent<Body>()->pm_velocity.x < 0)
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.x < -WallSlideMax)
				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = -WallSlideMax;

		if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Left)
		{
			if (Input::IsPressed(SDL_SCANCODE_RIGHT))
			{
				if (Input::IsTriggered(SDL_SCANCODE_SPACE) && IsAttachable)
				{
					WallJumpTriggered = true;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = WallJump;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = 600;
				}
			}
			if (Input::IsPressed(SDL_SCANCODE_SPACE) && IsAttachable)
			{
				if (Input::IsPressed(SDL_SCANCODE_LEFT))
				{
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = 700;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = WallJump;
				}
			}
			else if (Input::IsTriggered(SDL_SCANCODE_LEFT))
			{
				OffFromWall = true;

				this->GetOwner()->GetComponent<Body>()->pm_velocity.y = WallJump * 0.5f;
				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = WallJump * 0.5f;
			}
		}
		if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Right)
		{
			if (Input::IsPressed(SDL_SCANCODE_LEFT))
			{
				if (Input::IsTriggered(SDL_SCANCODE_SPACE) && IsAttachable)
				{
					WallJumpTriggered = true;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = -WallJump;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = 600;
				}
			}
			if (Input::IsPressed(SDL_SCANCODE_SPACE) && IsAttachable)
			{
				if (Input::IsPressed(SDL_SCANCODE_RIGHT))
				{
					this->GetOwner()->GetComponent<Body>()->pm_velocity.y = -700;
					this->GetOwner()->GetComponent<Body>()->pm_velocity.x = WallJump;
				}
			}
			else if (Input::IsTriggered(SDL_SCANCODE_RIGHT))
			{
				OffFromWall = true;

				this->GetOwner()->GetComponent<Body>()->pm_velocity.y = -WallJump * 0.5f;
				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = WallJump * 0.5f;
			}
		}

		if (!JumpEnough)
		{
			if (Input::IsPressed(SDL_SCANCODE_SPACE) && this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Ground)
			{
				this->GetOwner()->GetComponent<Body>()->pm_velocity = glm::vec3(JumpSpeed, 0, 0);
				JumpTriggered = true;
				SOUNDMANAGER->PlaySounds(JumpSound, false);
			}
		}
		if (this->GetOwner()->GetComponent<Body>()->GroundType != Grounded::Ground)
		{
			this->GetOwner()->GetComponent<Body>()->pm_velocity.x -= 2500.f * dt;
			if (this->GetOwner()->GetComponent<Body>()->pm_velocity.x < -FallSpeedMax)
				this->GetOwner()->GetComponent<Body>()->pm_velocity.x = -FallSpeedMax;
		}


		if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Air && !JumpTriggered && !WallJumpTriggered)
		{
			this->GetOwner()->GetComponent<Body>()->pm_velocity.x -= 2500.f * dt;
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
		JumpEnough = true;
	}
	else if (this->GetOwner()->GetComponent<Body>()->GroundType == Grounded::Ground)
	{
		JumpEnough = false;
		JumpTriggered = false;
	}
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