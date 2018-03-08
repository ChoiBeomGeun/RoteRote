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
	 rotation_radius = 100.f;
	 Selection = MenuList::Menu_Start;
	 delta_angle = 90;
	 LeftRotate = false;
	 RightRotate = false;
	 IsRotating = false;
	 IsSelected = false;
	 selection_angle = 0;
	 select_index = 0;
	 LEVELMANAGER->LoadLevel("Menu.json");
	 SOUNDMANAGER->PlaySounds(MenuSound, true);
}

void Menu::Update(float dt)
{
	if (FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle == 360.f || FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle == -360.f)
	{
		FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle = 0.f;
		delta_angle = 0;
		select_index = 0;
	}


	if (IsRotating) {
		DeltaAngle();

		for (int i = 1; i < 5; ++i) {

			FACTORY->ObjectIDMap[i]->GetComponent<Transform>()->position.x = cos(TUMath::DegreeToRadian(FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle + (i) * 90)) * rotation_radius;
			FACTORY->ObjectIDMap[i]->GetComponent<Transform>()->position.y = sin(TUMath::DegreeToRadian(FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle + (i) * 90)) * rotation_radius;
		}	
	}
	else
	{
		delta_angle = FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle;
	}
		
	if (!IsSelected && !IsRotating)
		if (Input::IsTriggered(SDL_SCANCODE_SPACE))
			IsSelected = true;

	if (IsSelected)
	{
		switch (Selection)
		{
		case MenuList::Menu_Start: STATEMANAGER->MoveState(StatesList::LevelSelect);
			break;
		case MenuList::Menu_HowToPlay:
			break;
		case MenuList::Menu_Quit: STATEMANAGER->Quit();
			break;
		case MenuList::Menu_Option: STATEMANAGER->MoveState(StatesList::Option);
			break;
		}
	}


	if(!LeftRotate && !IsRotating)
		if (Input::IsPressed(SDL_SCANCODE_LEFT)){
			LeftRotate = true;
			IsRotating = true;
			++select_index;
			Selection_plus();
		}
	else if(!RightRotate && !IsRotating)
		if (Input::IsPressed(SDL_SCANCODE_RIGHT)){
			RightRotate = true;
			IsRotating = true;
			--select_index;
			Selection_minus();
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

void Menu::DeltaAngle(void)
{
	if (abs(FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle  - delta_angle) >= 90)
	{
		IsRotating = false;
		delta_angle = FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle;
		if (delta_angle = FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle > 0)
		FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle = 90 * select_index;

		if(FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle < 0)
		FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle = -90 * abs(select_index);
	}
}

void Menu::Selection_plus(void)
{
	switch (Selection)
	{
	case MenuList::Menu_Start:  Selection = MenuList::Menu_HowToPlay; FACTORY->ObjectIDMap[7]->GetComponent<Sprite>()->texture_load("Menu_HowToPlay.png");
		break;
	case MenuList::Menu_HowToPlay: Selection = MenuList::Menu_Quit;	FACTORY->ObjectIDMap[7]->GetComponent<Sprite>()->texture_load("Menu_Quit.png");
		break;
	case MenuList::Menu_Quit: Selection = MenuList::Menu_Option; FACTORY->ObjectIDMap[7]->GetComponent<Sprite>()->texture_load("Menu_Option.png");
		break;
	case MenuList::Menu_Option: Selection = MenuList::Menu_Start; FACTORY->ObjectIDMap[7]->GetComponent<Sprite>()->texture_load("Menu_Start.png");
		break;
	default:
		break;
	}
}

void Menu::Selection_minus(void)
{
	switch (Selection)
	{
	case MenuList::Menu_HowToPlay:  Selection = MenuList::Menu_Start; FACTORY->ObjectIDMap[7]->GetComponent<Sprite>()->texture_load("Menu_Start.png");
		break;
	case MenuList::Menu_Quit: Selection = MenuList::Menu_HowToPlay; FACTORY->ObjectIDMap[7]->GetComponent<Sprite>()->texture_load("Menu_HowToPlay.png");
		break;
	case MenuList::Menu_Option: Selection = MenuList::Menu_Quit; FACTORY->ObjectIDMap[7]->GetComponent<Sprite>()->texture_load("Menu_Quit.png");
		break;
	case MenuList::Menu_Start: Selection = MenuList::Menu_Option; FACTORY->ObjectIDMap[7]->GetComponent<Sprite>()->texture_load("Menu_Option.png");
		break;
	default:
		break;
	}
}
