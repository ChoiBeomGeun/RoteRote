
/******************************************************************************/
/*!
\file   Trigger.cpp
\author HyunJun Yoo
\par    email: hyunjun306@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/12/16

Trigger game logic source file
There are interacting between player and triggers.
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/


#include "Trigger.h"
#include "StateManager.h"
#include "SoundManager.h"
#include "Timer.h"

using namespace TE;
Object * oPlayerInTrigger;
std::vector<Object*> TriggerList;

typedef std::vector<std::pair<Object*, int>> ButtonObjectList;
ButtonObjectList buttonsobj;
unsigned int RotationSound;

namespace TE {


	TriggerLogic * TRIGGERLOGIC = nullptr;


}

void RotateButtons(float angles) {


	if (buttonsobj.size() > 0) {

		for (auto it : buttonsobj) {

			it.first->transform->angle += angles;



		}



	}

}


TriggerLogic::TriggerLogic()
{

	TRIGGERLOGIC = this;
	RotationSound = SOUNDMANAGER->LoadSound("rotation.mp3");

}
void TriggerLogic::Initialize(void)
{

	TriggerList.clear();
	buttonsobj.clear();

	for (auto Objects : FACTORY->ObjectIDMap)
	{
		
		if (FACTORY->ObjectIDMap[Objects.first]->GetComponent(ComponentType::CT_CONTROLLER) != nullptr)
			oPlayerInTrigger = Objects.second;

		if (FACTORY->ObjectIDMap[Objects.first]->objectstyle == Objectstyle::Trigger90)
		{
			//Objects.second->trigger->i_innertimer = 100;
			Objects.second->trigger->i_innertimer = 0;
			Objects.second->trigger->Trigger_useable = true;
			TriggerList.push_back(Objects.second);

		}
		if (FACTORY->ObjectIDMap[Objects.first]->objectstyle == Objectstyle::Trigger180)
		{
			//Objects.second->trigger->i_innertimer = 100;
			Objects.second->trigger->i_innertimer = 0;
			Objects.second->trigger->Trigger_useable = true;
			TriggerList.push_back(Objects.second);

		}


	}
	for (int i = 0; i < 4; ++i)
		isDegree[i] = false;
	for (int i = 0; i < 4; ++i)
		isDegree180[i] = false;
	for (int i = 0; i < 4; ++i)
		isDegree180double[i] = false;

	int i = 0;

	for (auto it : FACTORY->ObjectIDMap)
	{
 		if (it.second->objectstyle == Objectstyle::Button)
		{

			buttonsobj.push_back(ButtonObjectList::value_type(it.second, i));
			i++;
		}


	}
}

void TriggerLogic::Update(float dt)
{

	dt = dt;
	for (auto TriggerObjects : TriggerList)
	{
		if (TriggerObjects->objectstyle != Objectstyle::Trigger90 &&
			(TriggerObjects->objectstyle != Objectstyle::Trigger180))
			continue;
		if (TriggerObjects->objectstyle == Objectstyle::Trigger90)
		{
			if (PHYSICS->RectvsRectCollisionCheck(oPlayerInTrigger->transform, TriggerObjects->transform) && TriggerObjects->trigger->Trigger_useable)
			{
				STATEMANAGER->b_IsGravityChanged = true;
				STATEMANAGER->b_IsRot90 = true;
				STATEMANAGER->b_IsRot180 = false;
				//NumberOfTriggersActivation++;
				if (TriggerObjects->trigger->Trigger_useable)
				{
					if (STATEMANAGER->b_IsRot90 && !STATEMANAGER->b_IsRot180)
						_90anglebutton(TriggerObjects);
				}

			}

		}
		if (TriggerObjects->objectstyle == Objectstyle::Trigger180)
		{
			if (PHYSICS->RectvsRectCollisionCheck(oPlayerInTrigger->transform, TriggerObjects->transform) && TriggerObjects->trigger->Trigger_useable)
			{
				
				STATEMANAGER->b_IsGravityChanged = true;
				STATEMANAGER->b_IsRot90 = false;
				STATEMANAGER->b_IsRot180 = true;
				if (TriggerObjects->trigger->Trigger_useable)
				{
					if (STATEMANAGER->b_IsRot180 && !STATEMANAGER->b_IsRot90)
					{
						_180anglebutton(TriggerObjects);
						NumberOfTriggersActivation++;
					}
				} 
				//_180anglebutton(TriggerObjects->trigger);
			}
		}
		if (!TriggerObjects->trigger->Trigger_useable)
			TriggerObjects->trigger->i_innertimer += 0.25f;


		if (TriggerObjects->trigger->i_innertimer > 30)
		{
			
			TriggerObjects->trigger->Trigger_useable = true;
			TriggerObjects->trigger->i_innertimer = 0;
		}

		if (!TriggerObjects->trigger->Trigger_useable)
		{
			if (TriggerObjects->objectstyle == Objectstyle::Trigger180)
			TriggerObjects->sprite->texture_load("180buttonPushed.png");
			if (TriggerObjects->objectstyle == Objectstyle::Trigger90)
			TriggerObjects->sprite->texture_load("90buttonPushed.png");
		}

		if (TriggerObjects->trigger->Trigger_useable)
		{
			if (TriggerObjects->objectstyle == Objectstyle::Trigger180)
			TriggerObjects->sprite->texture_load("180button.png");
			if (TriggerObjects->objectstyle == Objectstyle::Trigger90)
				TriggerObjects->sprite->texture_load("90button.png");
		}
	}





}

void TE::TriggerLogic::Free(void)
{
	STATEMANAGER->b_IsRotating = false;
	CAMERA->cameraUp.x = 0;
	CAMERA->cameraUp.y = 1;
	PHYSICS->gravityScale = -20.f;
	PHYSICS->GravityType = Gravity::y_Minus;
	PHYSICS->gravity = glm::vec3(0, PHYSICS->gravityScale, 0);
	for (int i = 0; i < 4; ++i)
		isDegree[i] = false;
	for (int i = 0; i < 4; ++i)
		isDegree180[i] = false;
	for (int i = 0; i < 4; ++i)
		isDegree180double[i] = false;

	for (auto Objects : FACTORY->ObjectIDMap)
	{
		if (FACTORY->ObjectIDMap[Objects.first]->objectstyle == Objectstyle::Trigger90)
		{
			Objects.second->trigger->i_innertimer = 0;
			Objects.second->trigger->Trigger_useable = false;
		}
		if (FACTORY->ObjectIDMap[Objects.first]->objectstyle == Objectstyle::Trigger180)
		{
			Objects.second->trigger->i_innertimer = 0;
			Objects.second->trigger->Trigger_useable = false;
		}
	}

	TriggerList.clear();

}


TriggerLogic::~TriggerLogic()
{
	
}

void TE::TriggerLogic::_90anglebutton(Object* Trigger)
{
	Trigger->trigger->i_innertimer += .025f;

	// from 0
	if (CAMERA->cameraUp.x == 0 && CAMERA->cameraUp.y >= 1)
	{
		SOUNDMANAGER->PlaySounds(RotationSound, false);
		isDegree[0] = true;
		PHYSICS->Gravity90();
	}
	if (isDegree[0])
	{
		if (CAMERA->cameraUp.x <= 1.f && CAMERA->cameraUp.y >= 0.f)
		{
			RotateButtons(-2.25);
			FACTORY->GetPlayer()->transform->angle -= 2.25;
			CAMERA->cameraUp.x += .025f;
			CAMERA->cameraUp.y -= .025f;
			STATEMANAGER->b_IsRotating = true;

		}
		else
		{
			RotateButtons(2.25);
			FACTORY->GetPlayer()->transform->angle += 2.25;
			CAMERA->cameraUp.x = 1.f;
			CAMERA->cameraUp.y = 0.f;
			STATEMANAGER->b_IsRot90 = false;
			STATEMANAGER->b_IsGravityChanged = false;
			STATEMANAGER->b_IsRotating = false;
			Trigger->trigger->Trigger_useable = false;
			isDegree[0] = false;
	
		}
	}
	// from 90
	else if (CAMERA->cameraUp.x == 1 && CAMERA->cameraUp.y >= 0)
	{
		SOUNDMANAGER->PlaySounds(RotationSound, false);
		isDegree[1] = true;
		PHYSICS->Gravity90();
		// to 180
	}
	if (isDegree[1])
	{
		if (CAMERA->cameraUp.x >= 0 && CAMERA->cameraUp.y >= -1)
		{
			FACTORY->GetPlayer()->transform->angle -= 2.25;
			RotateButtons(-2.25);
			CAMERA->cameraUp.x -= .025f;
			CAMERA->cameraUp.y -= .025f;
			STATEMANAGER->b_IsRotating = true;
		}
		else
		{
			RotateButtons(2.25);
			FACTORY->GetPlayer()->transform->angle += 2.25;
			//FACTORY->GetPlayer()->transform->angle = ;
			CAMERA->cameraUp.x = 0.f;
			CAMERA->cameraUp.y = -1.f;
			STATEMANAGER->b_IsRot90 = false;
			STATEMANAGER->b_IsGravityChanged = false;
			STATEMANAGER->b_IsRotating = false;
			Trigger->trigger->Trigger_useable = false;
			isDegree[1] = false;

		}
	}
	// from 180
	else if (CAMERA->cameraUp.x == 0 && CAMERA->cameraUp.y == -1)
	{
		SOUNDMANAGER->PlaySounds(RotationSound, false);
		isDegree[2] = true;
		PHYSICS->Gravity90();
		// to 270
	}
	if (isDegree[2])
	{
		if (CAMERA->cameraUp.x >= -1 && CAMERA->cameraUp.y <= 0)
		{
			RotateButtons(-2.25);
			FACTORY->GetPlayer()->transform->angle -= 2.25;
			CAMERA->cameraUp.x -= .025f;
			CAMERA->cameraUp.y += .025f;
			STATEMANAGER->b_IsRotating = true;
			
		}
		else
		{
			RotateButtons(2.25);
			FACTORY->GetPlayer()->transform->angle += 2.25;
			
			CAMERA->cameraUp.x = -1.f;
			CAMERA->cameraUp.y = 0.f;
			STATEMANAGER->b_IsRot90 = false;
			STATEMANAGER->b_IsGravityChanged = false;
			STATEMANAGER->b_IsRotating = false;
			Trigger->trigger->Trigger_useable = false;
			isDegree[2] = false;
		}
	}
	// from 270
	else if (CAMERA->cameraUp.x == -1 && CAMERA->cameraUp.y == 0)
	{
		SOUNDMANAGER->PlaySounds(RotationSound, false);
		isDegree[3] = true;
		PHYSICS->Gravity90();
		// to 0
	}
	if (isDegree[3])
	{
		if (CAMERA->cameraUp.x <= 0 && CAMERA->cameraUp.y <= 1)
		{
			RotateButtons(-2.25);
			FACTORY->GetPlayer()->transform->angle -= 2.25;
			CAMERA->cameraUp.x += .025f;
			CAMERA->cameraUp.y += .025f;
			STATEMANAGER->b_IsRotating = true;
			
		}
		else
		{
			RotateButtons(2.25);
			FACTORY->GetPlayer()->transform->angle += 2.25;
			CAMERA->cameraUp.x = 0.f;
			CAMERA->cameraUp.y = 1.f;
			STATEMANAGER->b_IsRot90 = false;
			STATEMANAGER->b_IsGravityChanged = false;
			STATEMANAGER->b_IsRotating = false;
			Trigger->trigger->Trigger_useable = false;
			isDegree[3] = false;

		}
	}
}

void TE::TriggerLogic::_180anglebutton(Object* Trigger)
{
	Trigger->trigger->i_innertimer += .025f;

	// from 0
	if (CAMERA->cameraUp.x == 0 && CAMERA->cameraUp.y == 1)
	{
		SOUNDMANAGER->PlaySounds(RotationSound, false);
		isDegree180[0] = true;
		isDegree180double[0] = false;
		PHYSICS->Gravity180();
	}
	// from 90
	else if (CAMERA->cameraUp.x == 1 && CAMERA->cameraUp.y == 0)
	{
		SOUNDMANAGER->PlaySounds(RotationSound, false);
		isDegree180[1] = true;
		isDegree180double[1] = false;
		PHYSICS->Gravity180();
	}

	// from 180
	else if (CAMERA->cameraUp.x == 0 && CAMERA->cameraUp.y == -1)
	{
		SOUNDMANAGER->PlaySounds(RotationSound, false);
		isDegree180[2] = true;
		isDegree180double[2] = false;
		PHYSICS->Gravity180();
	}
	// from 270
	else if (CAMERA->cameraUp.x == -1 && CAMERA->cameraUp.y == 0)
	{
		SOUNDMANAGER->PlaySounds(RotationSound, false);
		isDegree180[3] = true;
		isDegree180double[3] = false;
		PHYSICS->Gravity180();
	}

	// from 0
	//to 90
	if (isDegree180[0])
	{
		if (CAMERA->cameraUp.x <= 1 && CAMERA->cameraUp.y >= 0.f)
		{
			RotateButtons(-4.5);
			FACTORY->GetPlayer()->transform->angle -= 4.5;
			CAMERA->cameraUp.x += .025f;
			CAMERA->cameraUp.y -= .025f;
			STATEMANAGER->b_IsRotating = true;
		}
		else
		{
			

			isDegree180double[0] = true;
			//isDegree180[0] = false;
		}

		if (isDegree180double[0])
		{
			if (CAMERA->cameraUp.x >= 0 && CAMERA->cameraUp.y >= -1.f)
			{
				CAMERA->cameraUp.x -= .025f;
				CAMERA->cameraUp.y -= .025f;
			
			}
			else
			{
				RotateButtons(4.5);
				FACTORY->GetPlayer()->transform->angle += 4.5;
				CAMERA->cameraUp.x = 0.f;
				CAMERA->cameraUp.y = -1.f;
				STATEMANAGER->b_IsGravityChanged = false;
				STATEMANAGER->b_IsRot180 = false;
				STATEMANAGER->b_IsRotating = false;
				Trigger->trigger->Trigger_useable = false;
				isDegree180double[0] = false;
				isDegree180[0] = false;
		
			}
		}
	}


	// from 90

	//to 180
	if (isDegree180[1])
	{
		if (CAMERA->cameraUp.x >= 0 && CAMERA->cameraUp.y >= -1.f)
		{
			RotateButtons(-4.5);
			FACTORY->GetPlayer()->transform->angle -= 4.5;
			CAMERA->cameraUp.x -= .025f;
			CAMERA->cameraUp.y -= .025f;
			STATEMANAGER->b_IsRotating = true;
		}
		else
		{
			
		
			isDegree180double[1] = true;
			//isDegree180[1] = false;
		}
		if (isDegree180double[1])
		{
			if (CAMERA->cameraUp.x >= -1 && CAMERA->cameraUp.y <= 0.f)
			{
				CAMERA->cameraUp.x -= .025f;
				CAMERA->cameraUp.y += .025f;
			}
			else
			{
				RotateButtons(4.5);
				FACTORY->GetPlayer()->transform->angle += 4.5;
				CAMERA->cameraUp.x = -1.f;
				CAMERA->cameraUp.y = 0.f;
				STATEMANAGER->b_IsGravityChanged = false;
				STATEMANAGER->b_IsRot180 = false;
				STATEMANAGER->b_IsRotating = false;
				Trigger->trigger->Trigger_useable = false;
				isDegree180double[1] = false;
				isDegree180[1] = false;
			
			}
		}
	}

	// from 180
	//to 270
	if (isDegree180[2])
	{
		
		if (CAMERA->cameraUp.x >= -1 && CAMERA->cameraUp.y <= 0.f)
		{
			RotateButtons(-4.5);
			FACTORY->GetPlayer()->transform->angle -= 4.5;
			CAMERA->cameraUp.x -= .025f;
			CAMERA->cameraUp.y += .025f;
			STATEMANAGER->b_IsRotating = true;
		}
		else
		{
			
		
			isDegree180double[2] = true;
			//isDegree180[2] = false;
		}
		if (isDegree180double[2])
		{
			if (CAMERA->cameraUp.x <= 0 && CAMERA->cameraUp.y <= 1.f)
			{
				CAMERA->cameraUp.x += .025f;
				CAMERA->cameraUp.y += .025f;
			}
			else
			{
				RotateButtons(4.5);
				FACTORY->GetPlayer()->transform->angle += 4.5;
				CAMERA->cameraUp.x = 0.f;
				CAMERA->cameraUp.y = 1.f;
				STATEMANAGER->b_IsGravityChanged = false;
				STATEMANAGER->b_IsRot180 = false;
				STATEMANAGER->b_IsRotating = false;
				Trigger->trigger->Trigger_useable = false;
				isDegree180double[2] = false;
			
				isDegree180[2] = false;
			}
		}
	}

	// from 270
	//to 0
	// cameraUp = vec(-1, 0,0)
	if (isDegree180[3])
	{

		if (CAMERA->cameraUp.x <= 0 && CAMERA->cameraUp.y <= 1.f)
		{
			RotateButtons(-4.5);
			FACTORY->GetPlayer()->transform->angle -= 4.5;
			CAMERA->cameraUp.x += .025f;
			CAMERA->cameraUp.y += .025f;
			STATEMANAGER->b_IsRotating = true;
		}
		else
		{
			
	
			isDegree180double[3] = true;
			//isDegree180[3] = false;
		}
		// to 90
		if (isDegree180double[3])
		{
			if (CAMERA->cameraUp.x <= 1 && CAMERA->cameraUp.y >= 0.f)
			{
				CAMERA->cameraUp.x += .025f;
				CAMERA->cameraUp.y -= .025f;
			}
			else
			{
				RotateButtons(4.5);
				FACTORY->GetPlayer()->transform->angle += 4.5;
				CAMERA->cameraUp.x = 1.f;
				CAMERA->cameraUp.y = 0.f;
				STATEMANAGER->b_IsGravityChanged = false;
				STATEMANAGER->b_IsRot180 = false;
				STATEMANAGER->b_IsRotating = false;
				Trigger->trigger->Trigger_useable = false;
				isDegree180double[3] = false;
				isDegree180[3] = false;
	
			}
		}
	}
}

void TE::TriggerLogic::InitDegree(void)
{

	for (int i = 0; i < 4; ++i)
		isDegree[i] = false;
	for (int i = 0; i < 4; ++i)
		isDegree180[i] = false;
	for (int i = 0; i < 4; ++i)
		isDegree180double[i] = false;
}