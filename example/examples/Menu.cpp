/******************************************************************************/
/*!
\file   Menu.cpp
\author Kyungook.Park
\par    email: qkrruddn6680@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/04/19

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
#include "Factory.h"
#include "Input.h"
#include "StateManager.h"
#include "Engine.h"
#include "Graphics.h"
#include  "LevelManager.h"
#include  "Timer.h"
#include "Application.h"
#include "InGameLogic.h"
#include "ParticleManager.h"
using namespace TE;


Menu::Menu()
{
}

Menu::~Menu() 
{
}

void Menu::Load()
{
	LEVELMANAGER->LoadLevel("Menu.json");


	MenuSound = SOUNDMANAGER->LoadSound("menu.mp3");
	MoveSound = SOUNDMANAGER->LoadSound("menumove.mp3");
	SelectSound = SOUNDMANAGER->LoadSound("menuselect.mp3");

	Menu_Start = FACTORY->ObjectIDMap[6]->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("Menu_Start.png");
	Menu_HowToPlay = FACTORY->ObjectIDMap[6]->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("Menu_HowToPlay.png");
	Menu_Quit = FACTORY->ObjectIDMap[6]->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("Menu_Quit.png");
	Menu_Option = FACTORY->ObjectIDMap[6]->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("Menu_Option.png");





	select_particle = FACTORY->CreateHUD(glm::vec3(0, 0,0),glm::vec3(0.1f));
	select_particle->GetComponent<Sprite>()->isPerspective = true;
	PARTICLEMANAGER->LoadEmitter(select_particle, "MenuParticle.json");
	for (auto p : FACTORY->ObjectIDMap)
	{
		p.second->GetComponent<Sprite>()->isPerspective = true;
	}
	/*FACTORY->ObjectIDMap[1]->GetComponent<Transform>()->position = glm::vec3(0, rotation_radius, 0);
	FACTORY->ObjectIDMap[1]->GetComponent<Transform>()->scale = glm::vec3(APP->_screenWidth *.05f, APP->_screenWidth *.05f, 0);

	FACTORY->ObjectIDMap[2]->GetComponent<Transform>()->position = glm::vec3(-rotation_radius, 0, 0);
	FACTORY->ObjectIDMap[2]->GetComponent<Transform>()->scale = glm::vec3(APP->_screenWidth *.05f, APP->_screenWidth *.05f, 0);

	FACTORY->ObjectIDMap[3]->GetComponent<Transform>()->position = glm::vec3(0, -rotation_radius, 0);
	FACTORY->ObjectIDMap[3]->GetComponent<Transform>()->scale = glm::vec3(APP->_screenWidth *.05f, APP->_screenWidth *.05f, 0);

	FACTORY->ObjectIDMap[4]->GetComponent<Transform>()->position = glm::vec3(rotation_radius, 0, 0);
	FACTORY->ObjectIDMap[4]->GetComponent<Transform>()->scale = glm::vec3(APP->_screenWidth *.05f, APP->_screenWidth *.05f, 0);

	FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->scale = glm::vec3(APP->_screenWidth *.45f, APP->_screenWidth *.45f, 0);
	FACTORY->ObjectIDMap[6]->GetComponent<Transform>()->scale = glm::vec3(APP->_screenWidth *.25f, APP->_screenWidth *.25f, 0);
*/

	
	select_particle->GetComponent<Transform>()->position.y = static_cast<float>(rotation_radius);
	//select_particle->GetComponent<Emitter>()->pos.y = static_cast<float>(rotation_radius);
}

 void Menu::Init()
{
	 HowToPlayIsOn = false;
	 Selection = MenuList::Menu_Start;
	 rotation_radius = 200;

	 delta_angle = 90;
	 LeftRotate = false;
	 RightRotate = false;
	 IsRotating = false;
	 IsTextChanged = false;
	 IsSelected = false;
	 selection_angle = 0;
	 select_index = 0;
	 MenuCam.cameraSetting(CameraPosType::EN_Menu);
	 SOUNDMANAGER->PlaySounds(MenuSound, true);
	 /*APP->ResizeAllObjects();	
	 FACTORY->ObjectIDMap[7]->GetComponent<Transform>()->position = glm::vec3(0, APP->_screenHeight*.45f, 0);

	 FACTORY->ObjectIDMap[7]->GetComponent<Transform>()->scale = glm::vec3(APP->_screenWidth*.5f, APP->_screenHeight*.2f, 0);*/

}

void Menu::Update(float dt)
{

	if (Input::IsTriggered(SDL_SCANCODE_F1))
		STATEMANAGER->MoveState(StatesList::Intro-1);

	if (ConfirmationIsOn) {
		if (Input::IsTriggered(SDL_SCANCODE_Y))
			ENGINE->Quit();
		if (Input::IsTriggered(SDL_SCANCODE_N))
		{
			ConfirmationIsOn = false;
			FACTORY->Destroy(obj_confirmation);
			return;
		}
	}
	if (ConfirmationIsOn)
		return;

		IsSelected = false;
		if (FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle == 360.f || FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle == -360.f)
		{
			FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle = 0.f;
			delta_angle = 0;
			select_index = 0;
		}
		MenuCam.Update(dt);

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
			IsTextChanged = false;
			Selection_Text();
		}

		if (!IsSelected && !IsRotating)
			if (Input::IsTriggered(SDL_SCANCODE_SPACE) || Input::IsTriggered(SDL_SCANCODE_RETURN))
			{
				SOUNDMANAGER->PlaySounds(SelectSound, false);
				IsSelected = true;
			}
		if (IsSelected)
		{
			switch (Selection)
			{
			case MenuList::Menu_Start: STATEMANAGER->MoveState(StatesList::LevelSelect);
				break;
			case MenuList::Menu_HowToPlay:
				if (HowToPlayIsOn)
				{
					HowToPlayIsOn = false;
					FACTORY->Destroy(obj_howToPlay);
					return;
				}

				obj_howToPlay = FACTORY->CreateHUD(glm::vec3(0.2, -0.7, 0), glm::vec3(.7, 0.5, 0));
				obj_howToPlay->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("howTOPlay2.png");
				HowToPlayIsOn = true;
				return;
				break;
			case MenuList::Menu_Quit: 
				obj_confirmation = FACTORY->CreateHUD(glm::vec3(0, 0, 0), glm::vec3(.5, 0.5, 0));
				obj_confirmation->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("Sure.png");
				ConfirmationIsOn = true;
				return;
				break;
			case MenuList::Menu_Option: STATEMANAGER->MoveState(StatesList::Option);
				break;
			}
		}


		if (!LeftRotate && !IsRotating && !IsTextChanged)
			if (Input::IsPressed(SDL_SCANCODE_LEFT)) {
				LeftRotate = true;
				IsRotating = true;
				++select_index;
				Selection_plus();
				IsTextChanged = true;
		
				SOUNDMANAGER->PlaySounds(MoveSound, false);
			}
			else if (!RightRotate && !IsRotating && !IsTextChanged)
				if (Input::IsPressed(SDL_SCANCODE_RIGHT)) {
					RightRotate = true;
					IsRotating = true;
					--select_index;
					Selection_minus();
					IsTextChanged = true;
			
					SOUNDMANAGER->PlaySounds(MoveSound, false);
				}

		if (LeftRotate)
		{
			if (IsRotating) {

				FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle += 250 * dt;

			}
			else
				LeftRotate = false;
		}
		else if (RightRotate)
		{
			if (IsRotating)
				FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle -= 250 * dt;
			else
				RightRotate = false;
		}

		for (auto p : PARTICLEMANAGER->m_EmitterList)
		{
			p->isOn = true;
			if (p->type == ET_SELECTION)
				p->pos = glm::vec3(0,rotation_radius,0);
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
		if (delta_angle > 0)
		FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle = 90 * float(select_index);

		if (delta_angle < 0)
		FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle = -90 * float(abs(select_index));
	}
}

void Menu::Selection_plus(void)
{
	switch (Selection)
	{
	case MenuList::Menu_Start:  Selection = MenuList::Menu_HowToPlay; //FACTORY->ObjectIDMap[7]->GetComponent<Sprite>()->texture_load("Menu_HowToPlay.png");
		break;
	case MenuList::Menu_HowToPlay: Selection = MenuList::Menu_Quit;	//FACTORY->ObjectIDMap[7]->GetComponent<Sprite>()->texture_load("Menu_Quit.png");
		break;
	case MenuList::Menu_Quit: Selection = MenuList::Menu_Option; //FACTORY->ObjectIDMap[7]->GetComponent<Sprite>()->texture_load("Menu_Option.png");
		break;
	case MenuList::Menu_Option: Selection = MenuList::Menu_Start; //FACTORY->ObjectIDMap[7]->GetComponent<Sprite>()->texture_load("Menu_Start.png");
		break;
	default:
		break;
	}
}

void Menu::Selection_minus(void)
{
	switch (Selection)
	{
	case MenuList::Menu_HowToPlay:  Selection = MenuList::Menu_Start; //FACTORY->ObjectIDMap[7]->GetComponent<Sprite>()->texture_load("Menu_Start.png");
		break;
	case MenuList::Menu_Quit: Selection = MenuList::Menu_HowToPlay; //FACTORY->ObjectIDMap[7]->GetComponent<Sprite>()->texture_load("Menu_HowToPlay.png");
		break;
	case MenuList::Menu_Option: Selection = MenuList::Menu_Quit; //FACTORY->ObjectIDMap[7]->GetComponent<Sprite>()->texture_load("Menu_Quit.png");
		break;
	case MenuList::Menu_Start: Selection = MenuList::Menu_Option; //FACTORY->ObjectIDMap[7]->GetComponent<Sprite>()->texture_load("Menu_Option.png");
		break;
	default:
		break;
	}
}

void Menu::Selection_Text(void)
{
	switch (Selection)
	{
	case MenuList::Menu_HowToPlay: FACTORY->ObjectIDMap[6]->GetComponent<Sprite>()->m_TextureID = Menu_HowToPlay;
		break;
	case MenuList::Menu_Quit: FACTORY->ObjectIDMap[6]->GetComponent<Sprite>()->m_TextureID = Menu_Quit;
		break;
	case MenuList::Menu_Option: FACTORY->ObjectIDMap[6]->GetComponent<Sprite>()->m_TextureID = Menu_Option;
		break;
	case MenuList::Menu_Start: FACTORY->ObjectIDMap[6]->GetComponent<Sprite>()->m_TextureID = Menu_Start;
		break;
	default:
		break;
	}
}
