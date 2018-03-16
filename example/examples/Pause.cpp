/******************************************************************************/
/*!
\file   Pause.cpp
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

Pause State for the game
This states include ¡°back game¡±, ¡°Restart game¡±, ¡°Go to level Select¡±, ¡°How to play¡±
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "Pause.h"
#include <stdio.h>
#include "Object.h"
#include "Factory.h"
#include "Input.h"
#include "StateManager.h"
#include "Engine.h"
#include "Graphics.h"
#include  "LevelManager.h"
#include  "Timer.h"
#include <glm/gtx/transform.hpp>
#include "Trigger.h"
#include "SoundManager.h"
using namespace TE;
Object * oBackbutton;
Object * oRestartbutton;
Object * oLevelbutton;
Object * oQuitthegame;
Object * oHUD;
Object * oHowToPlay;
Object * oconfirmation;
Object * oHowToPlayIcon;

unsigned int pmove;
unsigned int pselect;
//Object * oPauseHUD;
glm::vec3 Vec3Buttonscale = glm::vec3(.2f, .2f, 0);
//glm::vec3 Vec3buttonPostion = glm::vec3(CAMERA->cameraPos.x, CAMERA->cameraPos.y, CAMERA->cameraUp.z);
glm::vec3 Vec3buttonPostion = glm::vec3(-.85f, .1f, 0);
int pauseindex = 0;
void ReStartgame(void);
void LevelSelect(void);
void Backgame(void);
void Quitgame(void);
void HowToPlay(void);
bool oExitornot = false;
bool HowTOplay = false;
 void(*pausefunction[5])(void) = { Backgame,ReStartgame,LevelSelect ,Quitgame,HowToPlay };
void ReStartgame(void)
{
	
	STATEMANAGER->b_IsReplay = false;
	STATEMANAGER->b_Relplay = false;
	STATEMANAGER->b_IsPauseFirst = true;
	CAMERA->cameraUp.x = 0;
	CAMERA->cameraUp.y = 1;
	TRIGGERLOGIC->InitDegree();

	STATEMANAGER->Restart();
}

void LevelSelect(void)
{

	STATEMANAGER->b_IsReplay = false;
	STATEMANAGER->b_Relplay = false;
	STATEMANAGER->b_IsPauseFirst = true;
	CAMERA->cameraUp.x = 0;
	CAMERA->cameraUp.y = 1;
	TRIGGERLOGIC->InitDegree();
	STATEMANAGER->PauseReturn();
	STATEMANAGER->MoveState(3);
}

void Backgame(void)
{
	
	
	STATEMANAGER->PauseReturn();
}
void Quitgame(void)
{


	oconfirmation = FACTORY->CreateHUD(glm::vec3(0, 0, 0), glm::vec3(1.5, 0.7, 0));
	oconfirmation->GetComponent<Sprite>()->texture_load("Sure.png");

	oExitornot = true;
}


void HowToPlay(void)
{


	oHowToPlay = FACTORY->CreateHUD(glm::vec3(0, 0,0), glm::vec3(2, 1.5, 0));
	oHowToPlay->GetComponent<Sprite>()->texture_load("howtoplay.png");

	HowTOplay = true;
}
Pause::Pause()
{

}

Pause::~Pause()
{
}

void Pause::Load()
{
	pmove = TE::SOUNDMANAGER->LoadSound("menumove.mp3");
	pselect = TE::SOUNDMANAGER->LoadSound("menuselect.mp3");
	
}


void Pause::Init()
{
	
	SOUNDMANAGER->PauseAllSound();

	oBackbutton = FACTORY->CreateHUD(glm::vec3(Vec3buttonPostion.x, Vec3buttonPostion.y + .3f, Vec3buttonPostion.z), Vec3Buttonscale);
	oBackbutton->GetComponent<Sprite>()->texture_load("back.png");
	oRestartbutton = FACTORY->CreateHUD(glm::vec3(Vec3buttonPostion.x, Vec3buttonPostion.y, Vec3buttonPostion.z), Vec3Buttonscale);
	oRestartbutton->GetComponent<Sprite>()->texture_load("retry.png");
	oLevelbutton = FACTORY->CreateHUD(glm::vec3(Vec3buttonPostion.x, Vec3buttonPostion.y - .3f, Vec3buttonPostion.z), Vec3Buttonscale);
	oLevelbutton->GetComponent<Sprite>()->texture_load("levelselect.png");
	oQuitthegame = FACTORY->CreateHUD(glm::vec3(Vec3buttonPostion.x, Vec3buttonPostion.y - .6f, Vec3buttonPostion.z), Vec3Buttonscale);
	oQuitthegame->GetComponent<Sprite>()->texture_load("box.png");
	oHUD = FACTORY->CreateHUD(glm::vec3(Vec3buttonPostion.x, Vec3buttonPostion.y-.1f, Vec3buttonPostion.z), glm::vec3(0.8,2,0));
	oHUD->GetComponent<Sprite>()->texture_load("pausehud.png");
	oHowToPlayIcon = FACTORY->CreateHUD(glm::vec3(Vec3buttonPostion.x, Vec3buttonPostion.y-.9f, Vec3buttonPostion.z), Vec3Buttonscale);
	oHowToPlayIcon->GetComponent<Sprite>()->texture_load("question.png");


}

void Pause::Update(float dt)
{

	dt = dt;
	if (oBackbutton->GetComponent<Transform>()->position.x <= -.7f)
	{
		
		oBackbutton->GetComponent<Transform>()->position.x += 0.02f;
		oRestartbutton->GetComponent<Transform>()->position.x += 0.02f;
		oLevelbutton->GetComponent<Transform>()->position.x += 0.02f;
		oQuitthegame->GetComponent<Transform>()->position.x += 0.02f;
		oHowToPlayIcon->GetComponent<Transform>()->position.x += 0.02f;
		oHUD->GetComponent<Transform>()->position.x += 0.02f;
	}
	if (!HowTOplay && !oExitornot) {
		if (Input::IsTriggered(SDL_SCANCODE_DOWN ))
			if (pauseindex <= 4)
			{
				SOUNDMANAGER->PlaySounds(pmove,false);
				pauseindex++;
				if (pauseindex == 5)
					pauseindex = 0;
			}

		if (Input::IsTriggered(SDL_SCANCODE_UP))
			if (pauseindex >= 0)
			{
				SOUNDMANAGER->PlaySounds(pmove, false);
				pauseindex--;
				if (pauseindex == -1)
					pauseindex = 4;
			}
	}
	switch (pauseindex)
	{
	case 1:
		oRestartbutton->GetComponent<Sprite>()->ChangeColor(255, 255, 0,255);
		oBackbutton->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		oLevelbutton->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		oQuitthegame->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		oHowToPlayIcon->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		break;
	case 0:
		oBackbutton->GetComponent<Sprite>()->ChangeColor(255, 255, 0, 255);
		oRestartbutton->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		oLevelbutton->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		oQuitthegame->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		oHowToPlayIcon->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		break;
	case 2:
		oLevelbutton->GetComponent<Sprite>()->ChangeColor(255, 255, 0, 255);
		oRestartbutton->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		oBackbutton->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		oQuitthegame->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		oHowToPlayIcon->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		break;
	case 3:
		oLevelbutton->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		oRestartbutton->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		oBackbutton->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		oQuitthegame->GetComponent<Sprite>()->ChangeColor(255, 255, 0, 255);
		oHowToPlayIcon->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		break;
	case 4:
		oLevelbutton->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		oRestartbutton->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		oBackbutton->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		oQuitthegame->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
		oHowToPlayIcon->GetComponent<Sprite>()->ChangeColor(255, 255, 0, 255);
		break;
	}
	if (!HowTOplay && !oExitornot) {
		if (Input::IsTriggered(SDL_SCANCODE_SPACE) || Input::IsTriggered(SDL_SCANCODE_RETURN)) {
			SOUNDMANAGER->PlaySounds(pselect, false);
			pausefunction[pauseindex]();
		}

	}
	if (oExitornot) {

		if (Input::IsTriggered(SDL_SCANCODE_Y))
		{
			SOUNDMANAGER->PlaySounds(pselect, false);
			ENGINE->Quit();
		}
		if (Input::IsTriggered(SDL_SCANCODE_N)) {
			SOUNDMANAGER->PlaySounds(pselect, false);
			oExitornot = false;
			FACTORY->Destroy(oconfirmation);
		}
	}
	if (oExitornot) {


		if (Input::IsTriggered(SDL_SCANCODE_ESCAPE)) {

			SOUNDMANAGER->PlaySounds(pselect, false);
			oExitornot = false;
			FACTORY->Destroy(oconfirmation);
		
		}
	}
	if (HowTOplay) {


		if (Input::IsTriggered(SDL_SCANCODE_ESCAPE)) {
			SOUNDMANAGER->PlaySounds(pselect, false);
			HowTOplay = false;
			FACTORY->Destroy(oHowToPlay);
		}
	}
	
}

void Pause::Free(void)
{

	pauseindex = 0;
	SOUNDMANAGER->ResumeAllSound();
	if (oBackbutton)
		FACTORY->Destroy(oBackbutton);
	if (oRestartbutton)
		FACTORY->Destroy(oRestartbutton);
	if (oLevelbutton)
		FACTORY->Destroy(oLevelbutton);
	if (oQuitthegame)
		FACTORY->Destroy(oQuitthegame);
	if (oHUD)
		FACTORY->Destroy(oHUD);
	if (oconfirmation)
		FACTORY->Destroy(oconfirmation);
	if (oHowToPlayIcon)
		FACTORY->Destroy(oHowToPlayIcon);

//	SOUNDMANAGER->DeleteSounds();
}


void Pause::Unload()
{

}
