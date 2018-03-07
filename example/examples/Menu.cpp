/******************************************************************************/
/*!
\file   Menu.cpp
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

Menu state source file
There are ¡°go to level select state¡±, ¡°option state¡±, ¡°How to play state¡±
¡°Quit the game¡±
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once
#include "Menu.h"
#include "SoundManager.h"
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
#include "InGameLogic.h"
using namespace TE;

Menu::Menu()
{
}

Menu::~Menu()
{
}

void Menu::Load()
{
	MenuSound = SOUNDMANAGER->LoadSound("menu.mp3");
	MoveSound = SOUNDMANAGER->LoadSound("menumove.mp3");
	SelectSound = SOUNDMANAGER->LoadSound("menuselect.mp3");
}

 void Menu::Init()
{
	 State = StatesList::StateList::LevelSelect;
	 rotation_radius = 120.f;
	 Selection = MenuList::Menu_Start;
	 delta_angle = 90;
	 LeftRotate = false;
	 RightRotate = false;
	 IsRotating = false;

	 angle = 0;

	 selection_angle = 0;

	 LEVELMANAGER->LoadLevel("Menu.json");
	 SOUNDMANAGER->PlaySounds(MenuSound, true);
}

void Menu::Update(float dt)
{
	if (FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle == 360.f)
	{
		FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle = 0.f;
		delta_angle = 0;
	}


	if (IsRotating) {
		DeltaAngle();
		if (angle < 90)
		{
			angle += 100 * dt;
			FACTORY->ObjectIDMap[1]->GetComponent<Transform>()->position.x = cos(TUMath::DegreeToRadian(selection_angle + angle)) * rotation_radius;
			FACTORY->ObjectIDMap[1]->GetComponent<Transform>()->position.y = sin(TUMath::DegreeToRadian(selection_angle + angle)) * rotation_radius;
		}
		else
			angle = 0;
	
	}
	else
	{
		if (FACTORY->ObjectIDMap[1]->GetComponent<Transform>()->position.x > 0)
			selection_angle = 0;
		else if (FACTORY->ObjectIDMap[1]->GetComponent<Transform>()->position.y > 0)
			selection_angle = 90;
		else if (FACTORY->ObjectIDMap[1]->GetComponent<Transform>()->position.x < 0)
			selection_angle = 180;
		else if (FACTORY->ObjectIDMap[1]->GetComponent<Transform>()->position.y < 0)
			selection_angle = 270;


		delta_angle = FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle;
	}
		

	if (Input::IsTriggered(SDL_SCANCODE_SPACE))
		STATEMANAGER->MoveState(StatesList::LevelSelect);

	if(!LeftRotate)
		if (Input::IsPressed(SDL_SCANCODE_LEFT)){
			LeftRotate = true;
			IsRotating = true;
		}
	else if(!RightRotate)
		if (Input::IsPressed(SDL_SCANCODE_RIGHT)){
			RightRotate = true;
			IsRotating = true;
		}

	if (LeftRotate)
	{
		if (IsRotating){
			
			FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle += 100 * dt;
			
		}
		else
			LeftRotate = false;
	}
	else if (RightRotate)
	{
		if (IsRotating)
			FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle -= 100 * dt;
		else
			RightRotate = false;
	}
}

void Menu::Free(void)
{
}

void Menu::Unload()
{
}

void Menu::MoveToState(void)
{
	STATEMANAGER->MoveState(State);
}

void Menu::DeltaAngle(void)
{
	if (FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle  - delta_angle >= 90)
	{
		IsRotating = false;
		delta_angle = FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle;
		FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle = 90 * (delta_angle % 90);
	}
	else if (FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle - delta_angle <= -90)
	{
		IsRotating = false;
		delta_angle = FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle;
		FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle = -90 * (delta_angle % 90);
	}
}
