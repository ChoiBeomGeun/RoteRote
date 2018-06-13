/******************************************************************************/
/*!
\file   Option.cpp
\author Kyungook.Park
\par    email: qkrruddn6680@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/04/19

Option state source file
There are ¡°go to level select state¡±, ¡°option state¡±, ¡°How to play state¡±
¡°Quit the game¡±
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once
#include "Option.h"
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
#include "Archetype.h"
using namespace TE;
Object * ooHowToPlay2;
Object * ooConfirmation2;

Object * ooCredits;


Option::Option()
{

}

Option::~Option()
{
}

void Option::Load()
{

	LEVELMANAGER->LoadLevel("Option.json");
	OptionSound = SOUNDMANAGER->LoadSound("Menu.mp3");
	MoveSound = SOUNDMANAGER->LoadSound("Menumove.mp3");
	SelectSound = SOUNDMANAGER->LoadSound("Menuselect.mp3");




	Option_SoundOnOff = FACTORY->ObjectIDMap[6]->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("Option_SoundOnOff.png");
	Option_Credits = FACTORY->ObjectIDMap[6]->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("Option_Credits.png");
	Option_Back = FACTORY->ObjectIDMap[6]->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("Option_BackToMenu.png"); 
	Option_Full = FACTORY->ObjectIDMap[6]->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("Option_FullScreen.png");
}

void Option::Init()
{
	if (IsSoundOn)
	{
		FACTORY->ObjectIDMap[1]->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("sound.png");
	}
	else
	{
		FACTORY->ObjectIDMap[1]->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("soundoff.png");
	}
	CreditsIsON = false;
	Selection = OptionList::Option_SoundOnOff;
	rotation_radius = 200;

	delta_angle = 90;
	LeftRotate = false;
	RightRotate = false;
	IsRotating = false;
	IsTextChanged = false;
	IsSelected = false;
	selection_angle = 0;
	select_index = 0;
	OptionCam.cameraSetting(CameraPosType::EN_Option);
	SOUNDMANAGER->PlaySounds(OptionSound, true);
}

void Option::Update(float dt)
{
	if (Input::IsTriggered(SDL_SCANCODE_ESCAPE))
		STATEMANAGER->MoveState(StatesList::Menu);

	IsSelected = false;
	if (FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle == 360.f || FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle == -360.f)
	{
		FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle = 0.f;
		delta_angle = 90;
		select_index = 0;
	}
	OptionCam.Update(dt);

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
		case OptionList::Option_SoundOnOff: 

			if (!SOUNDMANAGER->SoundOnOffCheck())
			{
				IsSoundOn = true;
				SOUNDMANAGER->SoundOn();
				OptionSound = SOUNDMANAGER->LoadSound("Menu.mp3");
				MoveSound = SOUNDMANAGER->LoadSound("Menumove.mp3");
				SelectSound = SOUNDMANAGER->LoadSound("Menuselect.mp3");
				//TE::FACTORY->ObjectIDMap[4]->sprite->texture_load("on.png");
				FACTORY->ObjectIDMap[1]->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("sound.png");

			}
			else
			{
				IsSoundOn = false;
				FACTORY->ObjectIDMap[1]->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("soundoff.png");
				SOUNDMANAGER->StopSound(OptionSound);
			
				SOUNDMANAGER->SoundOff();
			}

			break;
		case OptionList::Option_Credits:
		{
			static bool full = false;
		if (!APP->_isfull)
		{
			//TE::FACTORY->ObjectIDMap[7]->sprite->texture_load("on.png");
			APP->toggle_fullscreen(APP->getWindow(), true);
			full = true;
		}
		else
		{
			//	TE::FACTORY->ObjectIDMap[7]->sprite->texture_load("OFF.png");
			APP->toggle_fullscreen(APP->getWindow(), false);
			full = false;
		}
		break;
		}
		case OptionList::Option_Back:
		{
			STATEMANAGER->MoveState(1);
		break;
		}
		case OptionList::Option_Full:
		{
			if (CreditsIsON)
			{
				CreditsIsON = false;
				FACTORY->Destroy(ooCredits);
				return;
			}
			
			ooCredits = FACTORY->CreateArchetype(ReadingArchetype("Button.json"));
			ooCredits->GetComponent<Transform>()->SetPosition(glm::vec3(0,0,0));
			ooCredits->GetComponent<Transform>()->SetScale(glm::vec3(1280.f, 720.f, 0));
			ooCredits->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("CreditsText.png");
			CreditsIsON = true;
			return;
		break; 
		
		}
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




}




void Option::Free(void)
{

}

void Option::Unload()
{
}

void Option::DeltaAngle(void)
{
	if (abs(FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle - delta_angle) >= 90)
	{
		IsRotating = false;
		delta_angle = FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle;
		if (delta_angle > 0)
			FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle = 90 * float(select_index);

		if (delta_angle < 0)
			FACTORY->ObjectIDMap[5]->GetComponent<Transform>()->angle = -90 * float(abs(select_index));
	}
}

void Option::Selection_plus(void)
{
	switch (Selection)
	{
	case OptionList::Option_SoundOnOff:  Selection = OptionList::Option_Full; //FACTORY->ObjectIDMap[7]->GetComponent<Sprite>()->texture_load("Option_Credits.png");
		break;
	case OptionList::Option_Full: Selection = OptionList::Option_Back;	//FACTORY->ObjectIDMap[7]->GetComponent<Sprite>()->texture_load("Option_Back.png");
		break;
	case OptionList::Option_Back: Selection = OptionList::Option_Credits; //FACTORY->ObjectIDMap[7]->GetComponent<Sprite>()->texture_load("Option_Full.png");
		break;
	case OptionList::Option_Credits: Selection = OptionList::Option_SoundOnOff; //FACTORY->ObjectIDMap[7]->GetComponent<Sprite>()->texture_load("Option_SoundOnOff.png");
		break;
	default:
		break;
	}
}

void Option::Selection_minus(void)
{
	switch (Selection)
	{
	case OptionList::Option_Full:  Selection = OptionList::Option_SoundOnOff; //FACTORY->ObjectIDMap[7]->GetComponent<Sprite>()->texture_load("Option_SoundOnOff.png");
		break;
	case OptionList::Option_Back: Selection = OptionList::Option_Full; //FACTORY->ObjectIDMap[7]->GetComponent<Sprite>()->texture_load("Option_Credits.png");
		break;
	case OptionList::Option_Credits: Selection = OptionList::Option_Back; //FACTORY->ObjectIDMap[7]->GetComponent<Sprite>()->texture_load("Option_Back.png");
		break;
	case OptionList::Option_SoundOnOff: Selection = OptionList::Option_Credits; //FACTORY->ObjectIDMap[7]->GetComponent<Sprite>()->texture_load("Option_Option.png");
		break;
	default:
		break;
	}
}

void Option::Selection_Text(void)
{
	switch (Selection)
	{
	case OptionList::Option_Full: FACTORY->ObjectIDMap[6]->GetComponent<Sprite>()->m_TextureID = Option_Credits;
		break;
	case OptionList::Option_Back: FACTORY->ObjectIDMap[6]->GetComponent<Sprite>()->m_TextureID = Option_Back;
		break;
	case OptionList::Option_Credits: FACTORY->ObjectIDMap[6]->GetComponent<Sprite>()->m_TextureID =Option_Full;
		break;
	case OptionList::Option_SoundOnOff: FACTORY->ObjectIDMap[6]->GetComponent<Sprite>()->m_TextureID = Option_SoundOnOff;
		break;
	default:
		break;
	}
}
