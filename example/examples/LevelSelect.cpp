/******************************************************************************/
/*!
\file   LevelSelect.cpp
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

Level selection stage source file
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "StateLists.h"
#include "LevelSelect.h"
#include <stdio.h>
#include "Object.h"
#include "Factory.h"
#include "Input.h"
#include "StateManager.h"
#include "Engine.h"
#include "Graphics.h"
#include  "LevelManager.h"
#include  "Timer.h"
#include "Application.h"
#include "SoundManager.h"
#include <vector>
#include "InGameLogic.h"
using namespace TE;

LevelSelect::LevelSelect()
{

}

LevelSelect::~LevelSelect()
{

}

/*
Load texture & Initialize a value of texture to Levelpng[i]
*/
void LevelSelect::Load()
{
	LEVELMANAGER->LoadLevel("selectlevel.json");

	for (int i = 0; i <= LevelList::quit; ++i)
		Levelpng[i] = FACTORY->ObjectIDMap[3]->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id(toString(i));


}


void LevelSelect::Init()
{
	FACTORY->ObjectIDMap[3]->GetComponent<Sprite>()->m_TextureID = Levelpng[0];
	LevelList = LevelList::level1;
	IsRotating = false;
	IsLeftPressed = false;
	IsRightPreesed = false;
	LvlSelectCam.cameraSetting(glm::vec3(0, 0, 500.f));
}

void LevelSelect::Update(float dt)
{
	//LvlSelectCam.cameraSetting(glm::vec3(0,0,0));
	if (std::abs(FACTORY->ObjectIDMap[2]->GetComponent<Transform>()->angle == 360.f))
		FACTORY->ObjectIDMap[2]->GetComponent<Transform>()->angle = 0.f;

	LvlSelectCam.Update(dt);
	if (!IsRotating)	{
		if (Input::IsPressed(SDL_SCANCODE_RIGHT)) {
			if (LevelList == LevelList::quit)
				LevelList = LevelList::level1;
			else
				++LevelList;

			selectAngle = FACTORY->ObjectIDMap[2]->GetComponent<Transform>()->angle;

			IsRotating = true;
			IsLeftPressed = true;
			IsRightPreesed = false;
		}
		if (Input::IsPressed(SDL_SCANCODE_LEFT)) {
			if (LevelList == LevelList::level1)
				LevelList = LevelList::quit;
			else
				--LevelList;

			selectAngle = FACTORY->ObjectIDMap[2]->GetComponent<Transform>()->angle;

			IsRotating = true;
			IsRightPreesed = true;
			IsLeftPressed = false;
		}
		

		if (Input::IsTriggered(SDL_SCANCODE_SPACE)) {
			STATEMANAGER->Loadtolevelname = "level1.json";
			STATEMANAGER->MoveState(StatesList::Level1);
			STATEMANAGER->i_LevelSelect = LevelList;
		}
	}

	if (IsRotating)
	{
		if (IsLeftPressed)
			FACTORY->ObjectIDMap[2]->GetComponent<Transform>()->angle -= 100 * dt;
		else
			FACTORY->ObjectIDMap[2]->GetComponent<Transform>()->angle += 100 * dt;

		Rotation();
	}

}

void LevelSelect::Free(void)
{

}


void LevelSelect::Unload()
{
}

void LevelSelect::Rotation(void)
{
	if (IsRotating)		{
		if (FACTORY->ObjectIDMap[2]->GetComponent<Transform>()->angle - selectAngle > 0) {
			if (std::abs(FACTORY->ObjectIDMap[2]->GetComponent<Transform>()->angle - selectAngle) > 90) {
				FACTORY->ObjectIDMap[2]->GetComponent<Transform>()->angle = LevelList * 90.f;
				IsRotating = false;
				FACTORY->ObjectIDMap[3]->GetComponent<Sprite>()->m_TextureID = Levelpng[LevelList];
			}

		}
		else if (FACTORY->ObjectIDMap[2]->GetComponent<Transform>()->angle - selectAngle < 0) {
			if (std::abs(FACTORY->ObjectIDMap[2]->GetComponent<Transform>()->angle - selectAngle) > 90) {
				FACTORY->ObjectIDMap[2]->GetComponent<Transform>()->angle = -LevelList * 90.f;
				IsRotating = false;
				FACTORY->ObjectIDMap[3]->GetComponent<Sprite>()->m_TextureID = Levelpng[LevelList];
			}

		}

	}

}
