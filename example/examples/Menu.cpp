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

	 SOUNDMANAGER->PlaySounds(MenuSound, true);
	 LEVELMANAGER->LoadLevel("Menu.json");
}

void Menu::Update(float dt)
{
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
