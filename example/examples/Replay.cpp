/******************************************************************************/
/*!
\file   Replay.cpp
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

Replay state for the game
This state will replay player¡¯s behavior from saved data.
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "Clearzone.h"
#include  "Factory.h"
#include  "InGameLogic.h"
#include "Replay.h"
#include "Trigger.h"
#include "LevelManager.h"
#include "StateManager.h"
#include "Input.h"
#include "SoundManager.h"
//extern InGameLogic LevelInit;
#include "Application.h"
using namespace TE;
Object * Replayer;
Object * oRBackbutton;
Object * oRRestartbutton;
Object * oRLevelbutton;
Object * ReplayImage;
Object * ReplayUI;
int id = 0;
glm::vec3 Vec3Buttonscale2 = glm::vec3(.2f, .2f, 0);
int pauseindex2 = 0;
glm::vec3 Vec3buttonPostion2 = glm::vec3(.85f, .1f, 0);
int replaysize = 0;
void ReStartgame2(void);
void LevelSelect2(void);
void Backgame2(void);
void(*pausefunction2[3])(void) = { Backgame2,ReStartgame2,LevelSelect2 };
unsigned int rmove;
unsigned int rselect;
unsigned int rback;
unsigned int rwin;
bool nextlevel = false;
void ReStartgame2(void)
{
	STATEMANAGER->ReplayPosition.clear();
	STATEMANAGER->b_IsReplay = false;
	STATEMANAGER->b_Relplay = false;
	STATEMANAGER->b_IsPauseFirst = true;
	STATEMANAGER->Restart();

}

void LevelSelect2(void)
{
	STATEMANAGER->ReplayPosition.clear();

	STATEMANAGER->b_IsReplay = false;
	STATEMANAGER->b_Relplay = false;
	STATEMANAGER->b_IsPauseFirst = true;
	STATEMANAGER->MoveState(3);

}

void Backgame2(void)
{
	if (STATEMANAGER->i_LevelSelect == 13) {
		LevelSelect2();
		return;
	}


   	CAMERA->cameraUp.x = 0;
	CAMERA->cameraUp.y = 1;
	STATEMANAGER->ReplayPosition.clear();
	std::string level = "level";
	level += std::to_string(++STATEMANAGER->i_LevelSelect);
	level += +".json";
	STATEMANAGER->Loadtolevelname = level;
	STATEMANAGER->b_IsReplay = false;
	STATEMANAGER->b_Relplay = false;
	STATEMANAGER->b_IsPauseFirst = true;
	STATEMANAGER->MoveState(4);

}


ReplaySystem::ReplaySystem()
{
    	INGAMELOGIC = new InGameLogic;


}

ReplaySystem::~ReplaySystem()
{



}
void TE::ReplaySystem::Load(void)
{
	rmove = SOUNDMANAGER->LoadSound("menumove.mp3");
	rselect = SOUNDMANAGER->LoadSound("menuselect.mp3");
	rback = SOUNDMANAGER->LoadSound("menu.mp3");
	rwin= SOUNDMANAGER->LoadSound("win.mp3");
}
void ReplaySystem::Init()
{
	CAMERA->cameraUp.x = 0;
	CAMERA->cameraUp.y = 1;
	PHYSICS->gravityScale = -20.f;
	PHYSICS->GravityType = Gravity::y_Minus;
	PHYSICS->gravity = glm::vec3(0, PHYSICS->gravityScale, 0);


	SOUNDMANAGER->PlaySounds(rwin, false);
	SOUNDMANAGER->PlaySounds(rback, true);
	LEVELMANAGER->LoadLevel(STATEMANAGER->Loadtolevelname);
	INGAMELOGIC = new InGameLogic;
	CAMERA->cameraUp.x = 0;
	CAMERA->cameraUp.y = 1;
	STATEMANAGER->b_IsRot90 = false;
	STATEMANAGER->b_IsRot180 = false;
	action = 0;
	INGAMELOGIC->InGameInit();
	Replayer = FACTORY->GetPlayer();
	PHYSICS->gravityScale = -20.f;
	PHYSICS->GravityType = Gravity::y_Minus;
	PHYSICS->gravity = glm::vec3(0, PHYSICS->gravityScale, 0);
	oRBackbutton = FACTORY->CreateHUD(glm::vec3(Vec3buttonPostion2.x, Vec3buttonPostion2.y + .5f, Vec3buttonPostion2.z), Vec3Buttonscale2);
	oRBackbutton->sprite->texture_load("nextlevel.png");
	oRRestartbutton = FACTORY->CreateHUD(glm::vec3(Vec3buttonPostion2.x, Vec3buttonPostion2.y, Vec3buttonPostion2.z), Vec3Buttonscale2);
	oRRestartbutton->sprite->texture_load("retry.png");
	oRLevelbutton = FACTORY->CreateHUD(glm::vec3(Vec3buttonPostion2.x, Vec3buttonPostion2.y - .5f, Vec3buttonPostion2.z), Vec3Buttonscale2);
	oRLevelbutton->sprite->texture_load("levelselect.png");
	ReplayImage = FACTORY->CreateHUD(glm::vec3(0, 0, Vec3buttonPostion2.z), glm::vec3(2, 2, 0));
	ReplayImage->sprite->texture_load("replay.png");
	APP->b_Win = false;


	ReplayUI = FACTORY->CreateHUD(glm::vec3(0, 0, -10), glm::vec3(1, 1, 0));
	ReplayUI->sprite->texture_load("replay.png");

	if (false) {
	
		if (STATEMANAGER->i_LevelSelect == 13) {
			LevelSelect2();
			return;
		}


		CAMERA->cameraUp.x = 0;
		CAMERA->cameraUp.y = 1;
		STATEMANAGER->ReplayPosition.clear();
		std::string level = "level";
		level += std::to_string(++STATEMANAGER->i_LevelSelect);
		level += +".json";
		STATEMANAGER->Loadtolevelname = level;
		STATEMANAGER->b_IsReplay = false;
		STATEMANAGER->b_Relplay = false;
		STATEMANAGER->b_IsPauseFirst = true;
		STATEMANAGER->MoveState(4);
		nextlevel = false;
		STATEMANAGER->LogicShutdown = true;
		return;



	}
}


void ReplaySystem::Update(float dt)
{
	//APP->b_Win = false;
	INGAMELOGIC->InGameUpdate(dt);
	APP->b_Lose = false;
	
	if (STATEMANAGER->b_IsReplay)
	{


		FACTORY->GetPlayer()->transform->SetPosition(STATEMANAGER->ReplayPosition[action].first);
		FACTORY->GetPlayer()->animation->setFlipX(STATEMANAGER->ReplayPosition[action].second);
		FACTORY->GetPlayer()->animation->setTime(STATEMANAGER->AniSave[action].first);
		FACTORY->GetPlayer()->animation->setFrame(STATEMANAGER->AniSave[action].second);
		CAMERA->cameraPos = FACTORY->GetPlayer()->transform->position;
		CAMERA->cameraPos.z = 999.f;
		action++;
	}



	if (STATEMANAGER->ReplayPosition.size() <= action)
	{
		nextlevel = true;
		//APP->b_Win = true;
		action = 0;
		STATEMANAGER->b_IsReplayStart = false;

		CAMERA->cameraUp.x = 0;
		CAMERA->cameraUp.y = 1;
		PHYSICS->gravityScale = -20.f;
		PHYSICS->GravityType = Gravity::y_Minus;
		PHYSICS->gravity = glm::vec3(0, PHYSICS->gravityScale, 0);
		//ReplaySystem::Free();
		//ReplaySystem::Free();
		//STATEMANAGER->Restart();
		//ReplaySystem::Init();
		//STATEMANAGER->RestartReplay();
		//STATEMANAGER->MoveState(3);
		//STATEMANAGER->ReplayPosition.clear();
	}
	//if (oRBackbutton->transform&& oRLevelbutton->transform && oRRestartbutton->transform) 
	//{
	//   if (oRBackbutton->transform->position.x <= .7f)
	//   {
	//      oRBackbutton->transform->position.x-= 0.02;
	//      oRRestartbutton->transform->position.x -= 0.02;
	//      oRLevelbutton->transform->position.x -= 0.02;
	//   }
	//}

	//APP->b_Win = false;
	if (APP->b_Win) {
	
		ReplaySystem::Free();
	}

	if (Input::IsTriggered(SDL_SCANCODE_DOWN))
		if (pauseindex2 <= 3)
		{
			SOUNDMANAGER->PlaySounds(rmove, false);
			pauseindex2++;


			if (pauseindex2 == 3)
				pauseindex2 = 0;
		}

	if (Input::IsTriggered(SDL_SCANCODE_UP))
		if (pauseindex2 >= 0)
		{
			SOUNDMANAGER->PlaySounds(rmove, false);
			pauseindex2--;

			if (pauseindex2 == -1)
			{

				pauseindex2 = 2;
			}
		}


	if ((Input::IsTriggered(SDL_SCANCODE_SPACE) || (Input::IsTriggered(SDL_SCANCODE_RETURN)))
		&& (!STATEMANAGER->b_IsRotating)
		)
	{
		SOUNDMANAGER->PlaySounds(rselect, false);
		pausefunction2[pauseindex2]();

	}






	if (oRBackbutton&& oRLevelbutton && oRRestartbutton) {
		switch (pauseindex2)
		{
		case 1:
			oRRestartbutton->sprite->ChangeColor(255, 255, 0, 255);
			oRBackbutton->sprite->ChangeColor(255, 255, 255, 255);
			oRLevelbutton->sprite->ChangeColor(255, 255, 255, 255);
			break;
		case 0:
			oRBackbutton->sprite->ChangeColor(255, 255, 0, 255);
			oRRestartbutton->sprite->ChangeColor(255, 255, 255, 255);
			oRLevelbutton->sprite->ChangeColor(255, 255, 255, 255);
			break;
		case 2:
			oRLevelbutton->sprite->ChangeColor(255, 255, 0, 255);
			oRRestartbutton->sprite->ChangeColor(255, 255, 255, 255);
			oRBackbutton->sprite->ChangeColor(255, 255, 255, 255);
			break;
		}
	}
	
}
void TE::ReplaySystem::Free(void)
{

	if (oRBackbutton)
		FACTORY->Destroy(oRBackbutton);
	if (oRRestartbutton)
		FACTORY->Destroy(oRRestartbutton);
	if (oRLevelbutton)
		FACTORY->Destroy(oRLevelbutton);

	STATEMANAGER->b_IsRot180 = false;
	STATEMANAGER->b_IsRot90 = false;
	CAMERA->cameraUp.x = 0;
	CAMERA->cameraUp.y = 1;
	PHYSICS->gravityScale = -20.f;
	PHYSICS->GravityType = Gravity::y_Minus;
	PHYSICS->gravity = glm::vec3(0, PHYSICS->gravityScale, 0);
	SOUNDMANAGER->DeleteSounds();
	STATEMANAGER->b_IsReplayStart = true;
	INGAMELOGIC->InGameShutdown();
	//Backgame2();

}
void TE::ReplaySystem::Unload(void)
{
	STATEMANAGER->b_IsReplayStart = false;
	STATEMANAGER->b_IsReplay = false;
	//STATEMANAGER->ReplayPosition.clear();
}