
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
#include "PlayerController.h"

using namespace TE;
//Object * FACTORY->GamePlayer;
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

			it.first->GetComponent<Transform>()->angle += angles;



		}



	}

}


TriggerLogic::TriggerLogic()
{

	TRIGGERLOGIC = this;
	

}

void TriggerLogic::Initialize(void)
{
	Limited180_3[0] = Sprite::find_texture_id("limited180_1.png");
	Limited180_3[1] = Sprite::find_texture_id("limited180_2.png");
	Limited180_3[2] = Sprite::find_texture_id("limited180_3.png");

	Limited90_3[0] = Sprite::find_texture_id("limited90_1.png");
	Limited90_3[1] = Sprite::find_texture_id("limited90_2.png");
	Limited90_3[2] = Sprite::find_texture_id("limited90_3.png");

	unusable_Trigger = Sprite::find_texture_id("trigger_unusable");

	RotationSound = SOUNDMANAGER->LoadSound("rotation.mp3");
	TriggerList.clear();
	buttonsobj.clear();

	for (auto Objects : FACTORY->ObjectIDMap)
	{
		
		if (FACTORY->ObjectIDMap[Objects.first]->HasComponent<PlayerController>() )
			FACTORY->GamePlayer = Objects.second;

		if (FACTORY->ObjectIDMap[Objects.first]->objectstyle == Objectstyle::Trigger90)
		{
			//Objects.second->GetComponent<Trigger>()->i_innertimer = 100;
			Objects.second->GetComponent<Trigger>()->i_innertimer = 0;
			Objects.second->GetComponent<Trigger>()->Trigger_useable = true;
			TriggerList.push_back(Objects.second);

		}
		else if (FACTORY->ObjectIDMap[Objects.first]->objectstyle == Objectstyle::Trigger180)
		{
			//Objects.second->GetComponent<Trigger>()->i_innertimer = 100;
			Objects.second->GetComponent<Trigger>()->i_innertimer = 0;
			Objects.second->GetComponent<Trigger>()->Trigger_useable = true;
			TriggerList.push_back(Objects.second);

		}
		else if (FACTORY->ObjectIDMap[Objects.first]->objectstyle == Objectstyle::Trigger90Right)
		{
			//Objects.second->GetComponent<Trigger>()->i_innertimer = 100;
			Objects.second->GetComponent<Trigger>()->i_innertimer = 0;
			Objects.second->GetComponent<Trigger>()->Trigger_useable = true;
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
	//FACTORY->GamePlayer = FACTORY->GamePlayer;

	dt = dt;
	for (auto TriggerObjects : TriggerList)
	{
		if((APP->b_Lose)  )
			continue;

		if (TriggerObjects->GetComponent<Trigger>()->LifeTime == 0)
			TriggerObjects->GetComponent<Trigger>()->Trigger_useable = false;

		if (!TriggerObjects->GetComponent<Trigger>()->Trigger_useable)
			TriggerObjects->GetComponent<Sprite>()->m_TextureID = unusable_Trigger;

		if (TriggerObjects->objectstyle != Objectstyle::Trigger90 &&
			(TriggerObjects->objectstyle != Objectstyle::Trigger180) && (TriggerObjects->objectstyle != Objectstyle::Trigger90Right))
			continue;
		if (TriggerObjects->objectstyle == Objectstyle::Trigger90)
		{
			if ((PHYSICS->RectvsRectCollisionCheck(FACTORY->GamePlayer->GetComponent<Transform>(), 
				TriggerObjects->GetComponent<Transform>()) &&
				TriggerObjects->GetComponent<Trigger>()->Trigger_useable))
			{
				STATEMANAGER->b_IsGravityChanged = true;
				STATEMANAGER->b_IsRot90 = true;
				STATEMANAGER->b_IsRot180 = false;
				//NumberOfTriggersActivation++;
				if (TriggerObjects->GetComponent<Trigger>()->Trigger_useable)
				{
					if (STATEMANAGER->b_IsRot90 && !STATEMANAGER->b_IsRot180) {
						_90angle_toleft_button(TriggerObjects);
					}
				}
			}

		}
		if (TriggerObjects->objectstyle == Objectstyle::Trigger90Right)
		{
			if ((PHYSICS->RectvsRectCollisionCheck(FACTORY->GamePlayer->GetComponent<Transform>(),
				TriggerObjects->GetComponent<Transform>()) &&
				TriggerObjects->GetComponent<Trigger>()->Trigger_useable))
			{
				STATEMANAGER->b_IsGravityChanged = true;
				STATEMANAGER->b_IsRot90 = true;
				STATEMANAGER->b_IsRot180 = false;
				//NumberOfTriggersActivation++;
				if (TriggerObjects->GetComponent<Trigger>()->Trigger_useable)
				{
					if (STATEMANAGER->b_IsRot90 && !STATEMANAGER->b_IsRot180) {
						_90angle_toright_button(TriggerObjects);
					}
				}

			}

		}
		if (TriggerObjects->objectstyle == Objectstyle::Trigger180)
		{
			if (PHYSICS->RectvsRectCollisionCheck(FACTORY->GamePlayer->GetComponent<Transform>(), TriggerObjects->GetComponent<Transform>()) && TriggerObjects->GetComponent<Trigger>()->Trigger_useable)
			{
				
				STATEMANAGER->b_IsGravityChanged = true;
				STATEMANAGER->b_IsRot90 = false;
				STATEMANAGER->b_IsRot180 = true;
				if (TriggerObjects->GetComponent<Trigger>()->Trigger_useable)
				{
					if (STATEMANAGER->b_IsRot180 && !STATEMANAGER->b_IsRot90)
					{
						_180anglebutton(TriggerObjects);
						//NumberOfTriggersActivation++;
					}
				} 
				//_180anglebutton(TriggerObjects->GetComponent<Trigger>());
			}
		}
		if (!TriggerObjects->GetComponent<Trigger>()->Trigger_useable)
			TriggerObjects->GetComponent<Trigger>()->i_innertimer += 0.25f;


		if (TriggerObjects->GetComponent<Trigger>()->i_innertimer > 30)
		{
			NumberOfTriggersActivation++;
			TriggerObjects->GetComponent<Trigger>()->Trigger_useable = true;
			TriggerObjects->GetComponent<Trigger>()->i_innertimer = 0;
		}
		// To do: esd
		// 90 degrees right way needs to be implemented
		if (!TriggerObjects->GetComponent<Trigger>()->Trigger_useable)
		{
			if (TriggerObjects->objectstyle == Objectstyle::Trigger180)
			TriggerObjects->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("180buttonPushed.png");
			if (TriggerObjects->objectstyle == Objectstyle::Trigger90)
			TriggerObjects->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("90buttonPushed.png");
			if (TriggerObjects->objectstyle == Objectstyle::Trigger90Right)
				TriggerObjects->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("90buttonPushed.png");
		}

		if (TriggerObjects->GetComponent<Trigger>()->Trigger_useable)
		{
			if (TriggerObjects->objectstyle == Objectstyle::Trigger180)
			TriggerObjects->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("180button.png");
			if (TriggerObjects->objectstyle == Objectstyle::Trigger90)
				TriggerObjects->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("90button.png");
			if (TriggerObjects->objectstyle == Objectstyle::Trigger90Right)
				TriggerObjects->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("90TriggerRight.png");
		}

		LimitedTexture(TriggerObjects);
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
			Objects.second->GetComponent<Trigger>()->i_innertimer = 0;
			Objects.second->GetComponent<Trigger>()->Trigger_useable = false;
			Objects.second->GetComponent<Trigger>()->LifeTime = Objects.second->GetComponent<Trigger>()->MaxLife;
		}
		if (FACTORY->ObjectIDMap[Objects.first]->objectstyle == Objectstyle::Trigger180)
		{
			Objects.second->GetComponent<Trigger>()->i_innertimer = 0;
			Objects.second->GetComponent<Trigger>()->Trigger_useable = false;
			Objects.second->GetComponent<Trigger>()->LifeTime = Objects.second->GetComponent<Trigger>()->MaxLife;
		}
	}

	TriggerList.clear();

}


TriggerLogic::~TriggerLogic()
{
	
}

void TE::TriggerLogic::_90angle_toleft_button(Object* pTrigger)
{
	pTrigger->GetComponent<Trigger>()->i_innertimer += .025f;

	// from 0
	if (CAMERA->cameraUp.x == 0 && CAMERA->cameraUp.y == 1)
	{
		SOUNDMANAGER->PlaySounds(RotationSound, false);
		isDegree[0] = true;
		PHYSICS->Gravity90_to_left();
	}
	if (isDegree[0])
	{
		if (CAMERA->cameraUp.x <= 1.f && CAMERA->cameraUp.y >= 0.f)
		{
			RotateButtons(-2.25);
			FACTORY->GetPlayer()->GetComponent<Transform>()->angle -= 2.25;
			CAMERA->cameraUp.x += .025f;
			CAMERA->cameraUp.y -= .025f;
			STATEMANAGER->b_IsRotating = true;

		}
		else
		{
			RotateButtons(2.25);
			FACTORY->GetPlayer()->GetComponent<Transform>()->angle += 2.25;
			CAMERA->cameraUp.x = 1.f;
			CAMERA->cameraUp.y = 0.f;
			STATEMANAGER->b_IsRot90 = false;
			STATEMANAGER->b_IsGravityChanged = true;
			STATEMANAGER->b_IsRotating = false;
			pTrigger->GetComponent<Trigger>()->Trigger_useable = false;
			isDegree[0] = false;
			if (pTrigger->GetComponent<Trigger>()->LifeTime > 0) {
				--pTrigger->GetComponent<Trigger>()->LifeTime;
				LimitedTexture(pTrigger);
			}
		}
	}
	// from 90
	else if (CAMERA->cameraUp.x == 1 && CAMERA->cameraUp.y == 0)
	{
		SOUNDMANAGER->PlaySounds(RotationSound, false);
		isDegree[1] = true;
		PHYSICS->Gravity90_to_left();
		// to 180
	}
	if (isDegree[1])
	{
		if (CAMERA->cameraUp.x >= 0 && CAMERA->cameraUp.y >= -1)
		{
			FACTORY->GetPlayer()->GetComponent<Transform>()->angle -= 2.25;
			RotateButtons(-2.25);
			CAMERA->cameraUp.x -= .025f;
			CAMERA->cameraUp.y -= .025f;
			STATEMANAGER->b_IsRotating = true;
		}
		else
		{
			RotateButtons(2.25);
			FACTORY->GetPlayer()->GetComponent<Transform>()->angle += 2.25;
			//FACTORY->GetPlayer()->GetComponent<Transform>()->angle = ;
			CAMERA->cameraUp.x = 0.f;
			CAMERA->cameraUp.y = -1.f;
			STATEMANAGER->b_IsRot90 = false;
			STATEMANAGER->b_IsGravityChanged = true;
			STATEMANAGER->b_IsRotating = false;
			pTrigger->GetComponent<Trigger>()->Trigger_useable = false;
			isDegree[1] = false;
			if (pTrigger->GetComponent<Trigger>()->LifeTime > 0) {
				--pTrigger->GetComponent<Trigger>()->LifeTime;
				LimitedTexture(pTrigger);
			}

		}
	}
	// from 180
	else if (CAMERA->cameraUp.x == 0 && CAMERA->cameraUp.y == -1)
	{
		SOUNDMANAGER->PlaySounds(RotationSound, false);
		isDegree[2] = true;
		PHYSICS->Gravity90_to_left();
		// to 270
	}
	if (isDegree[2])
	{
		if (CAMERA->cameraUp.x >= -1 && CAMERA->cameraUp.y <= 0)
		{
			RotateButtons(-2.25);
			FACTORY->GetPlayer()->GetComponent<Transform>()->angle -= 2.25;
			CAMERA->cameraUp.x -= .025f;
			CAMERA->cameraUp.y += .025f;
			STATEMANAGER->b_IsRotating = true;
			
		}
		else
		{
			RotateButtons(2.25);
			FACTORY->GetPlayer()->GetComponent<Transform>()->angle += 2.25;
			
			CAMERA->cameraUp.x = -1.f;
			CAMERA->cameraUp.y = 0.f;
			STATEMANAGER->b_IsRot90 = false;
			STATEMANAGER->b_IsGravityChanged = true;
			STATEMANAGER->b_IsRotating = false;
			pTrigger->GetComponent<Trigger>()->Trigger_useable = false;
			isDegree[2] = false;
			if (pTrigger->GetComponent<Trigger>()->LifeTime > 0) {
				--pTrigger->GetComponent<Trigger>()->LifeTime;
				LimitedTexture(pTrigger);
			}
		}
	}
	// from 270
	else if (CAMERA->cameraUp.x == -1 && CAMERA->cameraUp.y == 0)
	{
		SOUNDMANAGER->PlaySounds(RotationSound, false);
		isDegree[3] = true;
		PHYSICS->Gravity90_to_left();
		// to 0
	}
	if (isDegree[3])
	{
		if (CAMERA->cameraUp.x <= 0 && CAMERA->cameraUp.y <= 1)
		{
			RotateButtons(-2.25);
			FACTORY->GetPlayer()->GetComponent<Transform>()->angle -= 2.25;
			CAMERA->cameraUp.x += .025f;
			CAMERA->cameraUp.y += .025f;
			STATEMANAGER->b_IsRotating = true;
			
		}
		else
		{
			RotateButtons(2.25);
			FACTORY->GetPlayer()->GetComponent<Transform>()->angle += 2.25;
			CAMERA->cameraUp.x = 0.f;
			CAMERA->cameraUp.y = 1.f;
			STATEMANAGER->b_IsRot90 = false;
			STATEMANAGER->b_IsGravityChanged = true;
			STATEMANAGER->b_IsRotating = false;
			pTrigger->GetComponent<Trigger>()->Trigger_useable = false;
			isDegree[3] = false;
			if (pTrigger->GetComponent<Trigger>()->LifeTime > 0) {
				--pTrigger->GetComponent<Trigger>()->LifeTime;
				LimitedTexture(pTrigger);
			}
		}
	}
}

void TriggerLogic::_90angle_toright_button(Object* pTrigger)
{
	pTrigger->GetComponent<Trigger>()->i_innertimer += .025f;

	// from 270
	if (CAMERA->cameraUp.x == -1 && CAMERA->cameraUp.y == 0)
	{
		SOUNDMANAGER->PlaySounds(RotationSound, false);
		isDegree[0] = true;
		PHYSICS->Gravity90_to_right();
		// to 0
	}
	if (isDegree[0])
	{
		// to 180
		if (CAMERA->cameraUp.x <= 0 && CAMERA->cameraUp.y >= -1)
		{
			RotateButtons(2.25);
			FACTORY->GetPlayer()->GetComponent<Transform>()->angle += 2.25;
			CAMERA->cameraUp.x += .025f;
			CAMERA->cameraUp.y -= .025f;
			STATEMANAGER->b_IsRotating = true;

		}
		else
		{
			RotateButtons(-2.25);
			FACTORY->GetPlayer()->GetComponent<Transform>()->angle -= 2.25;
			CAMERA->cameraUp.x = 0.f;
			CAMERA->cameraUp.y = -1.f;
			STATEMANAGER->b_IsRot90 = false;
			STATEMANAGER->b_IsGravityChanged = true;
			STATEMANAGER->b_IsRotating = false;
			pTrigger->GetComponent<Trigger>()->Trigger_useable = false;
			isDegree[0] = false;
			if (pTrigger->GetComponent<Trigger>()->LifeTime > 0) {
				--pTrigger->GetComponent<Trigger>()->LifeTime;
				LimitedTexture(pTrigger);
			}
		}
	}
	// from 180
	else if (CAMERA->cameraUp.x == 0 && CAMERA->cameraUp.y == -1)
	{
		SOUNDMANAGER->PlaySounds(RotationSound, false);
		isDegree[1] = true;
		PHYSICS->Gravity90_to_right();
		// to 270
	}
	if (isDegree[1])
	{
		// to 270
		if (CAMERA->cameraUp.x <= 1 && CAMERA->cameraUp.y <= 0)
		{
			RotateButtons(2.25);
			FACTORY->GetPlayer()->GetComponent<Transform>()->angle += 2.25;
			CAMERA->cameraUp.x += .025f;
			CAMERA->cameraUp.y += .025f;
			STATEMANAGER->b_IsRotating = true;

		}
		else
		{
			RotateButtons(-2.25);
			FACTORY->GetPlayer()->GetComponent<Transform>()->angle -= 2.25;

			CAMERA->cameraUp.x = 1.f;
			CAMERA->cameraUp.y = 0.f;
			STATEMANAGER->b_IsRot90 = false;
			STATEMANAGER->b_IsGravityChanged = true;
			STATEMANAGER->b_IsRotating = false;
			pTrigger->GetComponent<Trigger>()->Trigger_useable = false;
			isDegree[1] = false;
			if (pTrigger->GetComponent<Trigger>()->LifeTime > 0) {
				--pTrigger->GetComponent<Trigger>()->LifeTime;
				LimitedTexture(pTrigger);
			}
		}
	}
	// from 90
	else if (CAMERA->cameraUp.x == 1 && CAMERA->cameraUp.y == 0)
	{
		SOUNDMANAGER->PlaySounds(RotationSound, false);
		isDegree[2] = true;
		PHYSICS->Gravity90_to_right();
		// to 180
	}
	if (isDegree[2])
	{
		// to 180
		if (CAMERA->cameraUp.x >= 0 && CAMERA->cameraUp.y <= 1)
		{
			FACTORY->GetPlayer()->GetComponent<Transform>()->angle += 2.25;
			RotateButtons(+2.25);
			CAMERA->cameraUp.x -= .025f;
			CAMERA->cameraUp.y += .025f;
			STATEMANAGER->b_IsRotating = true;
		}
		else
		{
			RotateButtons(-2.25);
			FACTORY->GetPlayer()->GetComponent<Transform>()->angle -= 2.25;
			//FACTORY->GetPlayer()->GetComponent<Transform>()->angle = ;
			CAMERA->cameraUp.x = 0.f;
			CAMERA->cameraUp.y = 1.f;
			STATEMANAGER->b_IsRot90 = false;
			STATEMANAGER->b_IsGravityChanged = true;
			STATEMANAGER->b_IsRotating = false;
			pTrigger->GetComponent<Trigger>()->Trigger_useable = false;
			isDegree[2] = false;
			if (pTrigger->GetComponent<Trigger>()->LifeTime > 0) {
				--pTrigger->GetComponent<Trigger>()->LifeTime;
				LimitedTexture(pTrigger);
			}

		}
	}
	// from 0
	else if (CAMERA->cameraUp.x == 0 && CAMERA->cameraUp.y == 1)
	{
		SOUNDMANAGER->PlaySounds(RotationSound, false);
		isDegree[3] = true;
		PHYSICS->Gravity90_to_right();
		//90
	}
	if (isDegree[3])
	{
		// to 90
		if (CAMERA->cameraUp.x >= -1.f && CAMERA->cameraUp.y >= 0.f)
		{
			RotateButtons(2.25);
			FACTORY->GetPlayer()->GetComponent<Transform>()->angle += 2.25;
			CAMERA->cameraUp.x -= .025f;
			CAMERA->cameraUp.y -= .025f;
			STATEMANAGER->b_IsRotating = true;

		}
		else
		{
			RotateButtons(-2.25);
			FACTORY->GetPlayer()->GetComponent<Transform>()->angle -= 2.25;
			CAMERA->cameraUp.x = -1.f;
			CAMERA->cameraUp.y = 0.f;
			STATEMANAGER->b_IsRot90 = false;
			STATEMANAGER->b_IsGravityChanged = true;
			STATEMANAGER->b_IsRotating = false;
			pTrigger->GetComponent<Trigger>()->Trigger_useable = false;
			isDegree[3] = false;
			if (pTrigger->GetComponent<Trigger>()->LifeTime > 0) {
				--pTrigger->GetComponent<Trigger>()->LifeTime;
				LimitedTexture(pTrigger);
			}
		}
	}
	
	
}

void TE::TriggerLogic::_180anglebutton(Object* pTrigger)
{
	pTrigger->GetComponent<Trigger>()->i_innertimer += .025f;

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
			FACTORY->GetPlayer()->GetComponent<Transform>()->angle -= 4.5;
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
				FACTORY->GetPlayer()->GetComponent<Transform>()->angle += 4.5;
				CAMERA->cameraUp.x = 0.f;
				CAMERA->cameraUp.y = -1.f;
				STATEMANAGER->b_IsGravityChanged = true;
				STATEMANAGER->b_IsRot180 = false;
				STATEMANAGER->b_IsRotating = false;
				pTrigger->GetComponent<Trigger>()->Trigger_useable = false;
				isDegree180double[0] = false;
				isDegree180[0] = false;
				if (pTrigger->GetComponent<Trigger>()->LifeTime > 0) {
					--pTrigger->GetComponent<Trigger>()->LifeTime;
					LimitedTexture(pTrigger);
				}
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
			FACTORY->GetPlayer()->GetComponent<Transform>()->angle -= 4.5;
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
				FACTORY->GetPlayer()->GetComponent<Transform>()->angle += 4.5;
				CAMERA->cameraUp.x = -1.f;
				CAMERA->cameraUp.y = 0.f;
				STATEMANAGER->b_IsGravityChanged = true;
				STATEMANAGER->b_IsRot180 = false;
				STATEMANAGER->b_IsRotating = false;
				pTrigger->GetComponent<Trigger>()->Trigger_useable = false;
				isDegree180double[1] = false;
				isDegree180[1] = false;
				if (pTrigger->GetComponent<Trigger>()->LifeTime > 0) {
					--pTrigger->GetComponent<Trigger>()->LifeTime;
					LimitedTexture(pTrigger);
				}
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
			FACTORY->GetPlayer()->GetComponent<Transform>()->angle -= 4.5;
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
				FACTORY->GetPlayer()->GetComponent<Transform>()->angle += 4.5;
				CAMERA->cameraUp.x = 0.f;
				CAMERA->cameraUp.y = 1.f;
				STATEMANAGER->b_IsGravityChanged = true;
				STATEMANAGER->b_IsRot180 = false;
				STATEMANAGER->b_IsRotating = false;
				pTrigger->GetComponent<Trigger>()->Trigger_useable = false;
				isDegree180double[2] = false;
				isDegree180[2] = false;
				if (pTrigger->GetComponent<Trigger>()->LifeTime > 0) {
					--pTrigger->GetComponent<Trigger>()->LifeTime;
					LimitedTexture(pTrigger);
				}
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
			FACTORY->GetPlayer()->GetComponent<Transform>()->angle -= 4.5;
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
				FACTORY->GetPlayer()->GetComponent<Transform>()->angle += 4.5;
				CAMERA->cameraUp.x = 1.f;
				CAMERA->cameraUp.y = 0.f;
				STATEMANAGER->b_IsGravityChanged = true;
				STATEMANAGER->b_IsRot180 = false;
				STATEMANAGER->b_IsRotating = false;
				pTrigger->GetComponent<Trigger>()->Trigger_useable = false;
				isDegree180double[3] = false;
				isDegree180[3] = false;
				if (pTrigger->GetComponent<Trigger>()->LifeTime > 0) {
					--pTrigger->GetComponent<Trigger>()->LifeTime;
					LimitedTexture(pTrigger);
				}
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

void TriggerLogic::LimitedTexture(Object* trigger)
{
	if (trigger->GetComponent<Trigger>()->MaxLife == 3)
	{
		if (trigger->objectstyle == Objectstyle::Trigger90Right)
		{
			switch (trigger->GetComponent<Trigger>()->LifeTime)
			{
			case 3:
				trigger->GetComponent<Sprite>()->m_TextureID = Limited90_3[trigger->GetComponent<Trigger>()->MaxLife - 1];
				break;
			case 2:
				trigger->GetComponent<Sprite>()->m_TextureID = Limited90_3[trigger->GetComponent<Trigger>()->MaxLife - 2];
				break;
			case 1:
				trigger->GetComponent<Sprite>()->m_TextureID = Limited90_3[trigger->GetComponent<Trigger>()->MaxLife - 3];
				break;
			}
		}
		if (trigger->objectstyle == Objectstyle::Trigger90)
		{
			switch (trigger->GetComponent<Trigger>()->LifeTime)
			{
			case 3:
				trigger->GetComponent<Sprite>()->m_TextureID = Limited90_3[trigger->GetComponent<Trigger>()->MaxLife - 1];
				break;
			case 2:
				trigger->GetComponent<Sprite>()->m_TextureID = Limited90_3[trigger->GetComponent<Trigger>()->MaxLife - 2];
				break;
			case 1:
				trigger->GetComponent<Sprite>()->m_TextureID = Limited90_3[trigger->GetComponent<Trigger>()->MaxLife - 3];
				break;
			}
		}
		else if (trigger->objectstyle == Objectstyle::Trigger180)
		{
			switch (trigger->GetComponent<Trigger>()->LifeTime)
			{
			case 3:
				trigger->GetComponent<Sprite>()->m_TextureID = Limited180_3[trigger->GetComponent<Trigger>()->MaxLife - 1];
				break;
			case 2:
				trigger->GetComponent<Sprite>()->m_TextureID = Limited180_3[trigger->GetComponent<Trigger>()->MaxLife - 2];
				break;
			case 1:
				trigger->GetComponent<Sprite>()->m_TextureID = Limited180_3[trigger->GetComponent<Trigger>()->MaxLife - 3];
				break;
			}
		}
	}
}
