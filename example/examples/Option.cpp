/******************************************************************************/
/*!
\file   Option.cpp
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

Option state source file
There are changing resolution , mute sounds, toggle fullscreen
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "Option.h"
#include <stdio.h>
#include "Object.h"
#include "Factory.h"
#include "Input.h"
#include "StateManager.h"
#include "Engine.h"
#include "LevelManager.h"
#include  "SoundManager.h"
#include "Application.h"
#include "InGameLogic.h"
using namespace TE;
void Back(void);
void Soundsetting(void);
void resolution(void);
void full(void);
unsigned int menu1;
unsigned int move1;
unsigned int selectt1;
void(*ofunction[4])(void) = { Soundsetting,full,resolution,Back };
int index1 = 0;
static unsigned char  i;

void Soundsetting(void)
{

 	if (!SOUNDMANAGER->SoundOnOffCheck())
	{
		SOUNDMANAGER->SoundOn();
		menu1 = TE::SOUNDMANAGER->LoadSound("menu.mp3");
		move1 = TE::SOUNDMANAGER->LoadSound("menumove.mp3");
		selectt1 = TE::SOUNDMANAGER->LoadSound("menuselect.mp3");
		//TE::FACTORY->ObjectIDMap[4]->sprite->texture_load("on.png");
		
	}
	else
	{
		//TE::FACTORY->ObjectIDMap[4]->sprite->texture_load("off.png");
	//	SOUNDMANAGER->DeleteSounds();
		SOUNDMANAGER->SoundOff();
		
	}

}
void resolution(void)
{


	i++;

	if (i == 4)
	{
		i = 1;
	}
	APP->ChangeScreenSize(APP->getWindow(), (TE::Resolution)i);




}
void full(void)
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
}
void Back(void)
{
	STATEMANAGER->MoveState(1);

}
Option::Option()
{

}

Option::~Option()
{
}

void Option::Load()
{
	menu1 = TE::SOUNDMANAGER->LoadSound("menu.mp3");
	move1 = TE::SOUNDMANAGER->LoadSound("menumove.mp3");
	selectt1 = TE::SOUNDMANAGER->LoadSound("menuselect.mp3");
}

void Option::Init()
{
	i = (unsigned char)APP->ResolutionNumber;
	LEVELMANAGER->LoadLevel("Option.json");
	printf("OptionInit\n");
}

void Option::Update(float dt)
{
	if(APP->_isfull)
		TE::FACTORY->ObjectIDMap[7]->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("On.png");
	else
		TE::FACTORY->ObjectIDMap[7]->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("OFF.png");

	
		if (SOUNDMANAGER->SoundOnOffCheck())
			TE::FACTORY->ObjectIDMap[4]->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("on.png");
		else
			TE::FACTORY->ObjectIDMap[4]->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("off.png");
	dt = dt;
	if (Input::IsTriggered(SDL_SCANCODE_UP))
		if (index1 >= 0)
		{
			SOUNDMANAGER->PlaySounds(move1, false);
			index1--;
			if (index1 == -1)
				index1 = 3;
		}

	if (Input::IsTriggered(SDL_SCANCODE_DOWN))
		if (index1 <= 3)
		{
			SOUNDMANAGER->PlaySounds(move1, false);
			index1++;
			if (index1 == 4)
				index1 = 0;
		}

	switch (i)
	{

	case 1:
		TE::FACTORY->ObjectIDMap[8]->GetComponent<Transform>()->position.y = 0;
		APP->ResolutionNumber = 1;
		break;
	case 2:
		TE::FACTORY->ObjectIDMap[8]->GetComponent<Transform>()->position.y = -96;
		APP->ResolutionNumber = 2;
		break;
	case 3:
		TE::FACTORY->ObjectIDMap[8]->GetComponent<Transform>()->position.y = -203;
		APP->ResolutionNumber = 3;
		break;

	}

	switch (index1)
	{
	case 0:
		TE::FACTORY->ObjectIDMap[1]->GetComponent<Sprite>()->ChangeColor(255, 255, 0, 255);
		TE::FACTORY->ObjectIDMap[2]->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		TE::FACTORY->ObjectIDMap[3]->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		TE::FACTORY->ObjectIDMap[6]->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		break;
	case 2:
		TE::FACTORY->ObjectIDMap[2]->GetComponent<Sprite>()->ChangeColor(255, 255, 0, 255);
		TE::FACTORY->ObjectIDMap[1]->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		TE::FACTORY->ObjectIDMap[3]->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		TE::FACTORY->ObjectIDMap[6]->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		break;
	case 3:
		TE::FACTORY->ObjectIDMap[3]->GetComponent<Sprite>()->ChangeColor(255, 255, 0, 255);
		TE::FACTORY->ObjectIDMap[2]->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		TE::FACTORY->ObjectIDMap[1]->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		TE::FACTORY->ObjectIDMap[6]->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		break;
	case 1:
		TE::FACTORY->ObjectIDMap[6]->GetComponent<Sprite>()->ChangeColor(255, 255, 0, 255);
		TE::FACTORY->ObjectIDMap[2]->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		TE::FACTORY->ObjectIDMap[1]->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		TE::FACTORY->ObjectIDMap[3]->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		break;

	}

	if (Input::IsTriggered(SDL_SCANCODE_ESCAPE))
		ofunction[3]();

	if (Input::IsTriggered(SDL_SCANCODE_SPACE)||Input::IsTriggered(SDL_SCANCODE_RETURN)) {
		
		ofunction[index1]();
		SOUNDMANAGER->PlaySounds(selectt1, false);
	}
}

void Option::Free(void)
{
	//INGAMELOGIC->InGameShutdown();
	SOUNDMANAGER->DeleteSounds();
	index1 = 0;
	//LEVELMANAGER->SaveLevel("option.json");
	printf("OptionFree\n");
}



void Option::Unload()
{
	printf("OptionUnload\n");
}
