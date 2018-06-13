/******************************************************************************/
/*!
\file   LoseconditionLogic.cpp
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM250
\par    ThumbUp Engine
\date   2017/11/29

Lose condition game logic source file
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "LoseconditionLogic.h"
#include "StateManager.h"
#include "logging.h"
#include "Input.h"
#include "SoundManager.h"
#include "Camera.h"
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
	isBoundaryLose = false;
}

void TE::LoseconditionLogic::Update(float dt)
{
	dt = dt;
	if (STATEMANAGER->b_IsReplay)
		return;
	for (auto Objects : FACTORY->ObjectIDMap)
	{
		if (Objects.second->objectstyle == Objectstyle::Hazard)
		{
			if (Physics::RectvsRectCollisionCheck(Loseplayer->GetComponent<Transform>(), Objects.second->GetComponent<Transform>()))
			{
				SOUNDMANAGER->PlayOnceSounds(LoseSound, false, IsLoseSoundPlayed);
			
				APP->b_Lose = true;
				//STATEMANAGER->ReplayPosition.clear();
				//STATEMANAGER->ReplayPosition.clear();
 				CAMERA->IsCameraShaking = true;
				STATEMANAGER->Restart();
			}
		}
		if (Objects.second->objectstyle ==Objectstyle::Box || Objects.second->objectstyle == Objectstyle::AttachBox
			|| Objects.second->objectstyle == Objectstyle::AttachWall || Objects.second->objectstyle == Objectstyle::Wall
			)
		{
				obj = Objects.second;
		}
		else
		{
			continue;
		}
		


		if (Loseplayer != nullptr && obj != nullptr)
		{
			_fourPoints[DyingPlace::EN_LEFT_X] = CAMERA->cameraPos.x - APP->_screenWidth *.45f; // , CAMERA->CenterOfCamera.y + APP->_screenHeight*.5f);
			_fourPoints[DyingPlace::EN_RIGHT_X] = CAMERA->cameraPos.x + APP->_screenWidth *.45f;// , CAMERA->CenterOfCamera.y + APP->_screenHeight*.5f);
			_fourPoints[DyingPlace::EN_UP_Y] = CAMERA->cameraPos.y + APP->_screenHeight*.45f;
			_fourPoints[DyingPlace::EN_DOWN_Y] = CAMERA->cameraPos.y - APP->_screenHeight*.45f;



			if ((Loseplayer->GetComponent<Transform>()->position.x > 1000/*_fourPoints[DyingPlace::EN_RIGHT_X]*/ || Loseplayer->GetComponent<Transform>()->position.x < -1000/*_fourPoints[DyingPlace::EN_LEFT_X]*/ ||
				Loseplayer->GetComponent<Transform>()->position.y > 1000/*_fourPoints[DyingPlace::EN_UP_Y]*/ || Loseplayer->GetComponent<Transform>()->position.y < -1000/*_fourPoints[DyingPlace::EN_DOWN_Y]*/))
			{

		
						SOUNDMANAGER->PlayOnceSounds(LoseSound, false,IsLoseSoundPlayed);

				
				FACTORY->GetPlayer()->GetComponent<Sprite>()->ChangeColor(255, 0, 0, 255);
				FACTORY->GetPlayer()->GetComponent<Body>()->pm_velocity = glm::vec3(0);
				APP->b_Lose = true;
				//STATEMANAGER->ReplayPosition.clear();
				//STATEMANAGER->ReplayPosition.clear();
				isBoundaryLose = true;
				CAMERA->IsCameraShaking = false;
				STATEMANAGER->Restart();

			}
			float xnormal = (float)(Loseplayer->GetComponent<Transform>()->position.x - obj->GetComponent<Transform>()->position.x);
			float ynormal = (float)(Loseplayer->GetComponent<Transform>()->position.y - obj->GetComponent<Transform>()->position.y);
			int xdis = (int)(Loseplayer->GetComponent<Transform>()->scale.x * 0.5f + obj->GetComponent<Transform>()->scale.x * 0.5f);
			int ydis = (int)(Loseplayer->GetComponent<Transform>()->scale.y * 0.5f + obj->GetComponent<Transform>()->scale.y * 0.5f);

			int int_xnormal = TUMath::Round(xnormal);
			int int_ynormal = TUMath::Round(ynormal);

			if (Physics::RectvsRectCollisionCheck(Loseplayer->GetComponent<Transform>(), obj->GetComponent<Transform>()))
			{
				if (std::abs(int_xnormal) + 10.f < std::abs(xdis) && std::abs(int_ynormal) + 10.f < std::abs(ydis))
				{
					SOUNDMANAGER->PlayOnceSounds(LoseSound, false, IsLoseSoundPlayed);


 					
					FACTORY->GetPlayer()->GetComponent<Sprite>()->ChangeColor(255, 0, 0, 255);
					FACTORY->GetPlayer()->GetComponent<Body>()->pm_velocity = glm::vec3(0);
					APP->b_Lose = true;
			
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