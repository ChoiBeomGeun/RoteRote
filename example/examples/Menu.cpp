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
#include "StateLists.h"
#include "InGameLogic.h"
using namespace TE;
float Timer3 = 0;
Object * button1;
Object * button2;
Object * button3;
int index = 0;
void Start(void);
void Setting(void);
void Exit(void);
bool YesOrNot = false;
 unsigned int menu;
 unsigned int move;
 unsigned int selectt;
 Object * confirmation;
 bool oHowTOplay = false;
 bool Exitornot = false;
 bool Exitornot2 = false;
 void HowToPlay2(void);
 Object * ooHowToPlay;
void(*function[4])(void) = { Start,Setting,HowToPlay2,Exit };

Menu::Menu()
{

}

Menu::~Menu()
{
}

void Menu::Load()
{
	
	menu = TE::SOUNDMANAGER->LoadSound("menu.mp3");
	move = TE::SOUNDMANAGER->LoadSound("menumove.mp3");
	selectt = TE::SOUNDMANAGER->LoadSound("menuselect.mp3");
}
void Start(void)
{
	STATEMANAGER->MoveState(3);

}
void Setting(void)
{
	STATEMANAGER->MoveState(2);

}
void HowToPlay2(void)
{
	ooHowToPlay = FACTORY->CreateHUD(glm::vec3(0, 0, 0), glm::vec3(2, 1.5, 0));
	ooHowToPlay->sprite->texture_load("howtoplay.png");

	oHowTOplay = true;

}
void Exit(void)
{
	confirmation = FACTORY->CreateHUD(glm::vec3(0, 0, 0), glm::vec3(1.5, 0.7, 0));
	confirmation->sprite->texture_load("Sure.png");
	
	Exitornot = true;
	

}
 void Menu::Init()
{


	 SOUNDMANAGER->PlaySounds(menu, true);
	 LEVELMANAGER->LoadLevel("Menu.json");

}

void Menu::Update(float dt)
{
	//CAMERA->lookat(Vector3(0, 0, 3), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f));
	//CAMERA->view.RotateZ(TUMath::DegreeToRadian(0));

	dt = dt;



	if (!oHowTOplay && !Exitornot) {
		if (Input::IsTriggered(SDL_SCANCODE_LEFT))
			if (index >= 0)
			{
				SOUNDMANAGER->PlaySounds(move, false);
				index--;
				if (index == -1)
					index = 3;
			}

		if (Input::IsTriggered(SDL_SCANCODE_RIGHT))
			if (index <= 3)
			{
				SOUNDMANAGER->PlaySounds(move, false);
				index++;
				if (index == 4)
					index = 0;
			}
	}

	switch (index)
	{
		case 0:
		TE::FACTORY->ObjectIDMap[1]->sprite->ChangeColor(255, 255, 0, 255);
		TE::FACTORY->ObjectIDMap[2]->sprite->ChangeColor(255, 255, 255, 255);
		TE::FACTORY->ObjectIDMap[3]->sprite->ChangeColor(255, 255, 255, 255);
		TE::FACTORY->ObjectIDMap[6]->sprite->ChangeColor(255, 255, 255, 255);
		break;
		case 1:
		TE::FACTORY->ObjectIDMap[2]->sprite->ChangeColor(255, 255, 0, 255);
		TE::FACTORY->ObjectIDMap[1]->sprite->ChangeColor(255, 255, 255, 255);
		TE::FACTORY->ObjectIDMap[3]->sprite->ChangeColor(255, 255, 255, 255);
		TE::FACTORY->ObjectIDMap[6]->sprite->ChangeColor(255, 255, 255, 255);
		break;
		case 2:
			TE::FACTORY->ObjectIDMap[3]->sprite->ChangeColor(255, 255, 255, 255);
			TE::FACTORY->ObjectIDMap[2]->sprite->ChangeColor(255, 255, 255, 255);
			TE::FACTORY->ObjectIDMap[1]->sprite->ChangeColor(255, 255, 255, 255);
			TE::FACTORY->ObjectIDMap[6]->sprite->ChangeColor(255, 255, 0, 255);
			break;
		case 3:
		TE::FACTORY->ObjectIDMap[3]->sprite->ChangeColor(255, 255, 0, 255);
		TE::FACTORY->ObjectIDMap[2]->sprite->ChangeColor(255, 255, 255, 255);
		TE::FACTORY->ObjectIDMap[1]->sprite->ChangeColor(255, 255, 255, 255);
		TE::FACTORY->ObjectIDMap[6]->sprite->ChangeColor(255, 255, 255, 255);
		break;

	}

	if (!oHowTOplay && !Exitornot) {
		if (Input::IsTriggered(SDL_SCANCODE_SPACE)|| Input::IsTriggered(SDL_SCANCODE_RETURN)) {
			SOUNDMANAGER->PlaySounds(selectt, false);
			if (index == 0)
				function[0]();
			if (index == 1)
				function[1]();
			if (index == 2)
				function[2]();
			if (index == 3)
				function[3]();
		}
	}

	if (Input::IsTriggered(SDL_SCANCODE_ESCAPE)&&!Exitornot &&!oHowTOplay)
		function[3]();

	if (Exitornot ) {

		if (Input::IsTriggered(SDL_SCANCODE_Y))
			ENGINE->Quit();
		if (Input::IsTriggered(SDL_SCANCODE_N)) {
			Exitornot = false;
			FACTORY->Destroy(confirmation);
		}
	}
	if (Exitornot2) {

		if (Input::IsTriggered(SDL_SCANCODE_Y))
			ENGINE->Quit();
		if (Input::IsTriggered(SDL_SCANCODE_N)) {
			Exitornot = false;
			FACTORY->Destroy(confirmation);
		}


	}




	if (oHowTOplay) {


		if (Input::IsTriggered(SDL_SCANCODE_ESCAPE)) {
			oHowTOplay = false;
			FACTORY->Destroy(ooHowToPlay);
		}
	}
}

void Menu::Free(void)
{
	//INGAMELOGIC->InGameShutdown();
	SOUNDMANAGER->DeleteSounds();
	 index = 0;

}


void Menu::Unload()
{

}
