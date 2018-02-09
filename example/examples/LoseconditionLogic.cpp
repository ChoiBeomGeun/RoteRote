/******************************************************************************/
/*!
\file   LoseconditionLogic.cpp
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

Lose condition game logic source file
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "LoseconditionLogic.h"
#include "StateManager.h"
#include "logging.h"
#include "Input.h"
#include "SoundManager.h"
using namespace TE;
Object * Loseplayer;
Object *obj;
SOUNDID LoseSound;
bool Losesoundonce ;
namespace TE {


	LoseconditionLogic * LOSECONDITIONLOGIC = nullptr;


}
TE::LoseconditionLogic::LoseconditionLogic()
{
	LoseSound = SOUNDMANAGER->LoadSound("win.mp3");
	LOSECONDITIONLOGIC = this;
}

void TE::LoseconditionLogic::Initialize()
{
	Losesoundonce = true;
	Loseplayer = FACTORY->GetPlayer();

}

void TE::LoseconditionLogic::Update(float dt)
{
	dt = dt;
	if (STATEMANAGER->b_IsReplay)
		return;
	for (auto Objects : FACTORY->ObjectIDMap)
	{


		if (Objects.second->objectstyle != Objectstyle::Player && Objects.second->objectstyle != Objectstyle::Clearzone&& Objects.second->objectstyle != Objectstyle::Button && Objects.second->objectstyle != Objectstyle::Trigger180 && Objects.second->objectstyle != Objectstyle::Trigger90)
			obj = Objects.second;

		


		if (Loseplayer != nullptr && obj != nullptr)
		{
			if (Loseplayer->transform->position.x > 2000 || Loseplayer->transform->position.x < -2000||
				Loseplayer->transform->position.y > 2000 || Loseplayer->transform->position.y < -2000)
			{

				if (Losesoundonce)
					//	SOUNDMANAGER->PlaySounds(LoseSound, false);

					LoseSound = false;
				FACTORY->GetPlayer()->sprite->ChangeColor(255, 0, 0, 255);
				APP->b_Lose = true;
				//STATEMANAGER->ReplayPosition.clear();
				//STATEMANAGER->ReplayPosition.clear();
				CAMERA->IsCameraShaking = true;
				STATEMANAGER->Restart();

			}
			float xnormal = (float)(Loseplayer->transform->position.x - obj->transform->position.x);
			float ynormal = (float)(Loseplayer->transform->position.y - obj->transform->position.y);
			int xdis = (int)(Loseplayer->transform->scale.x * 0.5f + obj->transform->scale.x * 0.5f);
			int ydis = (int)(Loseplayer->transform->scale.y * 0.5f + obj->transform->scale.y * 0.5f);

			int int_xnormal = TUMath::Round(xnormal);
			int int_ynormal = TUMath::Round(ynormal);

			if (Physics::RectvsRectCollisionCheck(Loseplayer->transform, obj->transform))
			{
				if (std::abs(int_xnormal) + 10.f < std::abs(xdis) && std::abs(int_ynormal) + 10.f < std::abs(ydis))
				{
					
					if(Losesoundonce)
				//	SOUNDMANAGER->PlaySounds(LoseSound, false);

 					LoseSound = false;
					FACTORY->GetPlayer()->sprite->ChangeColor(255, 0, 0, 255);
					APP->b_Lose = true;
					STATEMANAGER->ReplayPosition.clear();
					//STATEMANAGER->ReplayPosition.clear();
					CAMERA->IsCameraShaking = true;
					STATEMANAGER->Restart();

				}

			}
		}

	}
}

void TE::LoseconditionLogic::Free(void)
{
//	delete LOSECONDITIONLOGIC;
	CAMERA->IsCameraShaking = false;
}

TE::LoseconditionLogic::~LoseconditionLogic()
{
	
}