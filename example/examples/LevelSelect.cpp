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
int levelindex = 0;
void level1(void);
void level2(void);
void level3(void);
void backmenu(void);
unsigned int Menus;
unsigned int moves;
unsigned int selectts;
typedef std::vector<std::pair<Object*, int>> ButtonObjectList;
ButtonObjectList buttons;
void(*levelfunction[4])(void) = { level1,level2,level3,backmenu };

void level1(void)
{
	STATEMANAGER->Loadtolevelname = "level1.json";
	STATEMANAGER->MoveState(4);
	STATEMANAGER->i_LevelSelect = 1;

}
void level2(void)
{
	STATEMANAGER->Loadtolevelname = "level2.json";
	STATEMANAGER->MoveState(4);
	STATEMANAGER->i_LevelSelect = 2;
}
void level3(void)
{
	
	STATEMANAGER->Loadtolevelname = "level3.json";
	STATEMANAGER->MoveState(4);
	STATEMANAGER->i_LevelSelect = 3;
}
void backmenu(void)
{
	
	STATEMANAGER->MoveState(1);

}

LevelSelect::LevelSelect()
{

}

LevelSelect::~LevelSelect()
{
}

void LevelSelect::Load()
{
	 Menus = SOUNDMANAGER->LoadSound("menu.mp3");

	
	 moves = TE::SOUNDMANAGER->LoadSound("menumove.mp3");
	 selectts = TE::SOUNDMANAGER->LoadSound("menuselect.mp3");
}


void LevelSelect::Init()
{
	Object * Init;
	int i = 0;
	SOUNDMANAGER->PlaySounds(Menus, true);
	LEVELMANAGER->LoadLevel("selectlevel.json");
	for (auto it : FACTORY->ObjectIDMap) {
		if (it.second->objectstyle == Objectstyle::Button)
		{
			if (it.second->GetComponent<Sprite>()->mTexutureDir == "back.png")
			{

				Init = it.second;
				continue;
			}
			buttons.push_back(ButtonObjectList::value_type(it.second,i));
			i++;
		}


	}
	buttons.push_back(ButtonObjectList::value_type(Init, i));
}

void LevelSelect::Update(float dt)
{
	
	dt = dt;
	//CAMERA->lookat(Vector3(0, 0, 3), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f));

	if (Input::IsTriggered(SDL_SCANCODE_LEFT))
		if (levelindex >= 0)
		{
			SOUNDMANAGER->PlaySounds(moves, false);
			levelindex--;
			if (levelindex == -1)
				levelindex = 13;
		}

	if (Input::IsTriggered(SDL_SCANCODE_RIGHT))
		if (levelindex <= 13)
		{
			SOUNDMANAGER->PlaySounds(moves, false);
			levelindex++;
			if (levelindex == 14)
				levelindex = 0;
		}
	if (buttons.size() > 1) {
		for (unsigned int i = 0; i < buttons.size(); i++)
		{
			if (buttons[i].second == levelindex)
				buttons[i].first->GetComponent<Sprite>()->ChangeColor(255, 255, 0, 255);
			else
				buttons[i].first->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		}

	}
	if (Input::IsTriggered(SDL_SCANCODE_ESCAPE)) {

		STATEMANAGER->MoveState(1);
		buttons.clear();


	}
	if (Input::IsTriggered(SDL_SCANCODE_SPACE)|| Input::IsTriggered(SDL_SCANCODE_RETURN)) {
		if (levelindex != 13) {
			SOUNDMANAGER->PlaySounds(selectts, false);
			std::string levelname = "level";
			levelname += std::to_string(levelindex + 1);
			levelname += ".json";
			STATEMANAGER->Loadtolevelname = levelname;
			STATEMANAGER->MoveState(4);
			STATEMANAGER->i_LevelSelect = levelindex + 1;
			buttons.clear();
		}
		else if (levelindex ==13)
		{
			STATEMANAGER->MoveState(1);
			buttons.clear();
		}
	}
}

void LevelSelect::Free(void)
{
	//INGAMELOGIC->InGameShutdown();
	levelindex = 0;
	SOUNDMANAGER->DeleteSounds();
}


void LevelSelect::Unload()
{
	printf("gd");
}
