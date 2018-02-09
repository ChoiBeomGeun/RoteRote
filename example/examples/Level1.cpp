/******************************************************************************/
/*!
\file   level1.cpp
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

All levels is running in this state
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Level1.h"
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
#include "ArchetypeEditor.h"
#include "InGameLogic.h"
#include "Trigger.h"
#include "Replay.h"
#include "logging.h"
#include "Clearzone.h"
#include "StateLists.h"
#include <random>
#define LOGGINGSTART false

using namespace TE;
static int i = 0;
static int j = 0;
float dangle = 0;
int replay = 0;
extern int i_angleOfspace;
bool IsAutoplay = false;
std::vector<glm::vec3> replayPos;
std::vector<Object> replayPlayer;
bool LosesoundOnetime = true;
bool IndicatorCheck = true;
std::string  path2;
Object * HUDLevelname;
Object * Indicator;
Object * Indicator1;
Object * Indicator2;
unsigned int Background;
unsigned int loseSound;
unsigned int winSound2;
Jsonclass file;
Json::Value root;
Object * player;
Level1::Level1()
{
	//   _centerOfScreen = { 0.f ,0.f };
}

Level1::~Level1()
{
}

void Level1::Load()
{


}

void Level1::Init()
{
	
    		LosesoundOnetime = true;
	CenterToPlayer = true;
	ZoomInToPlayer = false;
	XmovedCompleted = false;
	YmovedCompleted = false;
	path2 = STATEMANAGER->Loadtolevelname;
	char * Userinfo;
	size_t len = path2.size();
	_dupenv_s(&Userinfo, &len, "USERPROFILE");

	moving = false;
	//increasing = false;
	CAMERA->onPlayer(false);
	//mclicked = false;



	std::string saveLevel = path2;
#ifdef _DEBUG
	path2 = ".\\autoplays.\\" + path2;
#else
	path2 = Userinfo;
	path2 += "/Documents/RoteRote/autoplays/" + saveLevel;
#endif
	free(Userinfo);

	file.ReadFile(path2);

	Setshakeduration(2);
	loseSound = SOUNDMANAGER->LoadSound("lose.mp3");
	Background = SOUNDMANAGER->LoadSound("menu.mp3");
	winSound2 = SOUNDMANAGER->LoadSound("win.mp3");
	STATEMANAGER->b_IsRot90 = false;
	STATEMANAGER->b_IsRot180 = false;
	STATEMANAGER->b_IsGameLevel = true;
	STATEMANAGER->b_IsReplayStart = true;
	STATEMANAGER->ReplayPosition.clear();
	STATEMANAGER->AniSave.clear();
	INGAMELOGIC = new InGameLogic;

	LEVELMANAGER->LoadLevel(STATEMANAGER->Loadtolevelname);
	std::string levelname = std::to_string(STATEMANAGER->i_LevelSelect) + ".png";
	HUDLevelname = FACTORY->CreateHUD(glm::vec3(0, 0.9, 0), glm::vec3(0.1, 0.2, 0));
	HUDLevelname->sprite->texture_load(levelname);

	INGAMELOGIC->InGameInit();
	SOUNDMANAGER->PlaySounds(Background, true);
  	FACTORY->GetPlayer()->body->GroundType = Grounded::Ground;
	player = FACTORY->GetPlayer();
	player->animation->setFrame(1.0f / 8);
	player->animation->setTime(1.0f / 8);
	_camPaceSpeed = 5.0f;
	_camPacedirction = CAMERA->cameraPos - player->transform->position;
	_camStartPosition = glm::vec3{ 0.f, 0.f, 0.f };

	CAMERA->IsCameraAttached = true;
	CAMERA->cameraUp.x = 0;
	CAMERA->cameraUp.y = 1;


	cameraOriginPos = CAMERA->cameraPos;
	CAMERA->cameraPos.z = 1000.f;
	CAMERA->CenterOfCamera.x = FACTORY->LeftBoundary()->transform->position.x + (FACTORY->RightBoundary()->transform->position.x - FACTORY->LeftBoundary()->transform->position.x) *.5f;
	CAMERA->CenterOfCamera.y = FACTORY->DownBoundary()->transform->position.y + (FACTORY->UpBoundary()->transform->position.y - FACTORY->DownBoundary()->transform->position.y)*.5f;
	CAMERA->cameraPos.x = CAMERA->CenterOfCamera.x;
	CAMERA->cameraPos.y = CAMERA->CenterOfCamera.y;
	movingToCenter = false;
	STATEMANAGER->IsDrawing = false;
	//CAMERA->lookatMap(false); 

	movingToCenter = false;
}

void Level1::Update(float dt)
{
	static float timeringame = 200;
	if (IndicatorCheck &&timeringame <= 0 && STATEMANAGER->Loadtolevelname == "level1.json")
	{

		Indicator = FACTORY->CreateArchetype(ReadingArchetype("Button.json"));
		Indicator->transform->SetPosition(glm::vec3(FACTORY->GetPlayer()->transform->position.x - 80, 130 + FACTORY->GetPlayer()->transform->position.y, FACTORY->GetPlayer()->transform->position.z));
		Indicator->sprite->texture_load("left.png");
		Indicator->transform->scale.x = 80;
		Indicator->transform->scale.y = 80;

		Indicator1 = FACTORY->CreateArchetype(ReadingArchetype("Button.json"));
		Indicator1->transform->SetPosition(glm::vec3(FACTORY->GetPlayer()->transform->position.x + 80, 130 + FACTORY->GetPlayer()->transform->position.y, FACTORY->GetPlayer()->transform->position.z));
		Indicator1->sprite->texture_load("right.png");
		Indicator1->transform->scale.x = 80;
		Indicator1->transform->scale.y = 80;

		Indicator2 = FACTORY->CreateArchetype(ReadingArchetype("Button.json"));
		Indicator2->sprite->texture_load("space.png");
		Indicator2->transform->SetPosition(glm::vec3(FACTORY->GetPlayer()->transform->position.x, 210 + FACTORY->GetPlayer()->transform->position.y, FACTORY->GetPlayer()->transform->position.z));
		Indicator2->transform->scale.x = 210;
		Indicator2->transform->scale.y = 80;
		IndicatorCheck = false;
	}


	timeringame -= dt;
	
	//std::cout << CAMERA->cameraPos.x << " " << CAMERA->cameraPos.y << " \n";



#ifdef _DEBUG

	if (Input::IsTriggered(SDL_SCANCODE_F7))
		STATEMANAGER->MoveState(StatesList::MapEditor);
#endif


	if (Input::IsTriggered(SDL_SCANCODE_F8)&& STATEMANAGER->i_LevelSelect != 13 && !STATEMANAGER->b_IsRot180 && !STATEMANAGER->b_IsRot90)
	{

	/*	if (STATEMANAGER->i_LevelSelect == 9) {
			STATEMANAGER->ReplayPosition.clear();
			STATEMANAGER->AniSave.clear();
			STATEMANAGER->b_IsReplay = false;
			STATEMANAGER->b_Relplay = false;
			STATEMANAGER->b_IsPauseFirst = true;
			STATEMANAGER->MoveState(3);
		}
*/

		CAMERA->cameraUp.x = 0;
		CAMERA->cameraUp.y = 1;
		STATEMANAGER->ReplayPosition.clear();
		STATEMANAGER->AniSave.clear();
		std::string level = "level";
		level += std::to_string(++STATEMANAGER->i_LevelSelect);
		level += +".json";
		STATEMANAGER->Loadtolevelname = level;
		STATEMANAGER->b_IsReplay = false;
		STATEMANAGER->b_Relplay = false;
		STATEMANAGER->b_IsPauseFirst = true;
		STATEMANAGER->Restart();



	}
	if (Input::IsTriggered(SDL_SCANCODE_F11))
		STATEMANAGER->Restart();

	if (STATEMANAGER->b_IsReplayStart) {
		std::pair<glm::vec3, bool> temp;
		std::pair<float, float> tempTF;
		tempTF.first = FACTORY->GetPlayer()->animation->getTime();
		tempTF.second = FACTORY->GetPlayer()->animation->getFrame();
		temp.first = FACTORY->GetPlayer()->transform->GetPosition();
		temp.second = FACTORY->GetPlayer()->animation->isFlippedX();
		STATEMANAGER->ReplayPosition.push_back(temp);
		STATEMANAGER->AniSave.push_back(tempTF);
		//root["PositionX"][j] = FACTORY->GetPlayer()->transform->GetPosition().x;
		//root["PositionY"][j] = FACTORY->GetPlayer()->transform->GetPosition().y;
		//root["Frame"][j] = FACTORY->GetPlayer()->animation->getFrame();
		//root["FrameTime"][j] = FACTORY->GetPlayer()->animation->getTime();
		//root["isFlippedX"][j] = FACTORY->GetPlayer()->animation->isFlippedX();
		//
		//j++;
	}

	


	if (!ENGINE->GetGameStateIsOn())
	{
	
		delete INGAMELOGIC;
		delete LOGGINGSYSTEM;
	}
	if (Input::IsTriggered(SDL_SCANCODE_F9)&& !IsAutoplay)
	{
		
		STATEMANAGER->b_IsAutoplaying = true;
		IsAutoplay = true;
	}
	if (IsAutoplay)
	{
		float Xpos = file.mRoot["PositionX"][j].asFloat();
		float Ypos = file.mRoot["PositionY"][j].asFloat();
		float Frame = file.mRoot["Frame"][j].asFloat();
		float FrameTime = file.mRoot["FrameTime"][j].asFloat();
		bool Pressed = file.mRoot["isFlippedX"][j].asBool();
		FACTORY->GetPlayer()->transform->SetPosition(glm::vec3(Xpos, Ypos, 0));
		FACTORY->GetPlayer()->animation->setFrame(Frame);
		FACTORY->GetPlayer()->animation->setTime(FrameTime);
		FACTORY->GetPlayer()->animation->setFlipX(Pressed);
		j++;
	}


	INGAMELOGIC->InGameUpdate(dt);
	if (APP->b_Win) {
	

		
	 //file.WriteFile(path2, root);
 		Level1::Free();
		STATEMANAGER->b_IsReplayStart = false;
		STATEMANAGER->b_IsReplay = true;
	
		
	}
	
	if (Input::IsAnyTriggered())
	{
		movingToCenter = true;
	}

	if (movingToCenter)
	{
		if (CenterToPlayer)
		{
			CamMoveToPlayer(dt);
			/*if (Input::IsTriggered(SDL_SCANCODE_M))
			lookAtMap();*/
		}
		else
		{
			/*if (ZoomInToPlayer)
			zoomintoPlayer();
			if (Input::IsTriggered(SDL_SCANCODE_M))
			lookAtMap();*/
			if (FACTORY->GetPlayer()!= nullptr) {
				CAMERA->cameraPos.x = player->transform->position.x;
				CAMERA->cameraPos.y = player->transform->position.y;
				//CAMERA->cameraPos.z = 800.f;

				//CAMERA->lookat(CAMERA->cameraPos, CAMERA->cameraTarget, CAMERA->cameraUp);
			}
		}
	}


	if (APP->b_Lose)
	{
		if(LosesoundOnetime)
		SOUNDMANAGER->PlaySounds(loseSound, false);
		LosesoundOnetime = false;
		CAMERA->IsCameraShaking = true;
	}
	if (CAMERA->IsCameraShaking)
	{
	//	SOUNDMANAGER->StopSound(Background);
		cameraOriginPos = CAMERA->cameraPos;
		ShakeCamera(dt);
	}

}

void Level1::Free(void)
{
	CAMERA->isCentered = true;
	movingToCenter = false;
	CAMERA->IsCameraShaking = false;
	STATEMANAGER->b_IsAutoplaying = false;
	IsAutoplay = false;
	j = 0;

	LOGGINGSYSTEM->SavingLog();
	STATEMANAGER->b_IsGameLevel = false;
	CAMERA->cameraUp.x = 0;
	CAMERA->cameraUp.y = 1;
	PHYSICS->gravityScale = -20.f;
	PHYSICS->GravityType = Gravity::y_Minus;
	PHYSICS->gravity = glm::vec3(0, PHYSICS->gravityScale, 0);
	//SOUNDMANAGER->StopSound(Background);
	STATEMANAGER->b_IsReplayStart = true;

	INGAMELOGIC->InGameShutdown();
	SOUNDMANAGER->DeleteSounds();

	delete LEVELMANAGER;
	delete LOGGINGSYSTEM;
	
}
void Level1::Unload()
{
	STATEMANAGER->b_IsReplayStart = false;
	STATEMANAGER->b_IsReplay = false;
}

void Level1::zoomintoPlayer()
{
	if (ZoomInToPlayer)
	{
		
		if (CAMERA->cameraPos.z >= 800.f)
			CAMERA->cameraPos.z -= 5.f;
		if (CAMERA->cameraPos.z <= 800.f)
		{
			CAMERA->cameraPos.z = 800.f;
			ZoomInToPlayer = false;
		}
	}
}

void Level1::CamMoveToPlayer(float dt)
{
	dt;
	float player_x, player_y;
	player_x = FACTORY->GetPlayer()->transform->position.x;
	player_y = FACTORY->GetPlayer()->transform->position.y;
	if (CenterToPlayer)
	{

		if (player_x < CAMERA->cameraPos.x)
		{
			CAMERA->cameraPos.x -= 5.f;
		}

		if (player_x > CAMERA->cameraPos.x)
			CAMERA->cameraPos.x += 5.f;

		if (player_y < CAMERA->cameraPos.y)
		{
			CAMERA->cameraPos.y -= 5.f;
		}

		if (player_y > CAMERA->cameraPos.y)
		{
			CAMERA->cameraPos.y += 5.f;
		}
		if (!XmovedCompleted)
		{
			if ((int)CAMERA->cameraPos.x == (int)FACTORY->GetPlayer()->transform->position.x)
			{
				CAMERA->cameraPos.x = (float)FACTORY->GetPlayer()->transform->position.x;
				//CAMERA->cameraPos.y = (int)FACTORY->GetPlayer()->transform->position.y;
				XmovedCompleted = true;
			}
		}
		if (!YmovedCompleted)
		{
			if ((int)CAMERA->cameraPos.y == (int)FACTORY->GetPlayer()->transform->position.y)
			{
				CAMERA->cameraPos.y = (float)FACTORY->GetPlayer()->transform->position.y;
				//CAMERA->cameraPos.x = (int)FACTORY->GetPlayer()->transform->position.x;
				YmovedCompleted = true;
			}
		}

		if (XmovedCompleted && YmovedCompleted)
		{
			CenterToPlayer = false;
			CAMERA->isCentered = false;
			ZoomInToPlayer = true;
		}
		//CAMERA->lookat(glm::vec3(glm::vec2(player_x, player_y), CAMERA->cameraPos.z), CAMERA->cameraTarget, CAMERA->cameraUp);
	}
}
void StageLevel1Logic() {

	FACTORY->CreateArchetype(ReadingArchetype("Button.json"));
	



}
void Level1::lookAtMap()
{
	CAMERA->CenterOfCamera.x = FACTORY->LeftBoundary()->transform->position.x + (FACTORY->RightBoundary()->transform->position.x - FACTORY->LeftBoundary()->transform->position.x) *.5f;
	CAMERA->CenterOfCamera.y = FACTORY->DownBoundary()->transform->position.y + (FACTORY->UpBoundary()->transform->position.y - FACTORY->DownBoundary()->transform->position.y)*.5f;
	if (!ZoomInToPlayer)
		CAMERA->cameraPos.z = 1000.f;
	else
		CAMERA->cameraPos.z = 800.f;
	CAMERA->cameraPos = glm::vec3(glm::vec2(CAMERA->CenterOfCamera.x, CAMERA->CenterOfCamera.y), CAMERA->cameraPos.z);

	//CAMERA->projection = glm::perspective(glm::radians(CAMERA->angle), CAMERA->_aspect, CAMERA->_zNear, CAMERA->_zFar);
	/*if (CAMERA->cameraPos.z <= 1000.f)
	CAMERA->cameraPos.z += 5.f;
	if (CAMERA->cameraPos.z >= 1000.f)
	{
	CAMERA->cameraPos.z = 1000.f;
	}*/

}

//void Level1::CamMoveToCenter()
//{
//   CAMERA->CenterOfCamera.x = FACTORY->LeftBoundary()->transform->position.x + (FACTORY->RightBoundary()->transform->position.x - FACTORY->LeftBoundary()->transform->position.x) *.5f;
//   CAMERA->CenterOfCamera.y = FACTORY->DownBoundary()->transform->position.y + (FACTORY->UpBoundary()->transform->position.y - FACTORY->DownBoundary()->transform->position.y)*.5f;
//
//   if (movingToCenter)
//   {
//      std::cout << CAMERA->CenterOfCamera.x << " " << CAMERA->CenterOfCamera.y << "\n";
//      std::cout << CAMERA->cameraPos.x << " " << CAMERA->cameraPos.y << "\n";
//      if (CAMERA->cameraPos.x < CAMERA->CenterOfCamera.x)
//      {
//         CAMERA->cameraPos.x += 5.0f;
//      }
//
//      if (CAMERA->cameraPos.x > CAMERA->CenterOfCamera.x)
//         CAMERA->cameraPos.x -= 5.0f;
//
//
//      if (CAMERA->cameraPos.y < CAMERA->CenterOfCamera.y)
//      {
//         CAMERA->cameraPos.y += 5.0f;
//      }
//
//      if (CAMERA->cameraPos.y > CAMERA->CenterOfCamera.y)
//      {
//         CAMERA->cameraPos.y -= 5.0f;
//      }
//
//      if (CAMERA->cameraPos.x == CAMERA->CenterOfCamera.x)
//      {
//         CAMERA->cameraPos.x = CAMERA->CenterOfCamera.x;
//      }
//      if (CAMERA->cameraPos.y == CAMERA->CenterOfCamera.y)
//      {
//         CAMERA->cameraPos.y = CAMERA->CenterOfCamera.y;
//      }
//
//      if (glm::vec2(CAMERA->cameraPos.x, CAMERA->cameraPos.y) == CAMERA->CenterOfCamera)
//      {
//         CAMERA->cameraPos.x = CAMERA->CenterOfCamera.x;
//         CAMERA->cameraPos.y = CAMERA->CenterOfCamera.y;
//         movingToCenter = false;
//      }
//   }
//}

void Level1::ShakeCamera(float dt)
{
	if (shakeDuration > 0)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(-10.0, 10.0);

		CAMERA->cameraPos = cameraOriginPos + glm::vec3(dis(gen) * shakeAmount, dis(gen) * shakeAmount, 1);
		CAMERA->cameraPos.z = 999.f;
		shakeDuration -= dt * decreaseFactor;
	}
	else
	{

		CAMERA->IsCameraShaking = false;
		shakeDuration = 0.f;
		CAMERA->cameraPos = cameraOriginPos;
	}
}

void Level1::FollowPlayer(glm::vec3 startCamPos, float dt)
{
	glm::vec3 displacement = FACTORY->GetPlayer()->transform->position - CAMERA->cameraPos;
	_camStartPosition = startCamPos;
//	auto distanceFromStart = glm::length(displacement);

	_camPacedirction = -displacement;

	_camPacedirction = glm::normalize(_camPacedirction);

	CAMERA->cameraPos += _camPacedirction * dt * _camPaceSpeed;

}