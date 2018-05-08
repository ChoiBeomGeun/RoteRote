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
#include "Archetype.h"
#include "InGameLogic.h"
#include "Trigger.h"
#include "System.h"
#include "logging.h"
#include "Clearzone.h"
#include "StateLists.h"
#include "ReplayLogic.h"
#include "CameraMovement.h"
#include "Transform.h"
#include <random>
#include "PlayerController.h"
#include "cAutoMoving.h"
#include "Automoving.h"
#include "LoseconditionLogic.h"
#define LOGGINGSTART false

using namespace TE;
void MakingInstructions(float dt);
void CheatKeyFunctions(void);
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

Object * Movingtest;

Object * particle;
Level1::Level1()
{
	//   _centerOfScreen = { 0.f ,0.f };
}

Level1::~Level1()
{
}

void Level1::Load()
{
	INGAMELOGIC->InGameInit();

}

void Level1::Init()
{
	//Loading Button 
	/*Object * Loading = FACTORY->CreateHUD(glm::vec3(0, 0, 0), glm::vec3(2, 2, 0));
	Loading->IsLoadingObject = true;
	Loading->GetComponent<Sprite>()->texture_load("loading.png");
*/

	
	camAct.isCamToPlayer = true;
	LosesoundOnetime = true;
	CenterToPlayer = true;
	ZoomInToPlayer = false;
	XmovedCompleted = false;
	YmovedCompleted = false;
	path2 = STATEMANAGER->Loadtolevelname;
	_camPaceSpeed = 100.0f;


	moving = false;
	//increasing = false;
	CAMERA->onPlayer(false);
	//mclicked = false;



	std::string saveLevel = path2;

	path2 = ".\\autoplays.\\" + path2;


	file.ReadFile(path2);

	camAct.Setshakeduration(2);
	camAct.isCamToPlayer = false;
	loseSound = SOUNDMANAGER->LoadSound("lose.mp3");
	Background = SOUNDMANAGER->LoadSound("menu.mp3");
	winSound2 = SOUNDMANAGER->LoadSound("win.mp3");
	STATEMANAGER->b_IsRot90 = false;
	STATEMANAGER->b_IsRot180 = false;
	STATEMANAGER->b_IsGameLevel = true;
	STATEMANAGER->b_IsReplayStart = true;
	STATEMANAGER->b_IsReplay = false;
	STATEMANAGER->b_Relplay = false;

	STATEMANAGER->AniSave.clear();


	LEVELMANAGER->LoadLevel(STATEMANAGER->Loadtolevelname);

 	std::string levelname = std::to_string(STATEMANAGER->i_LevelSelect) + ".png";
	HUDLevelname = FACTORY->CreateHUD(glm::vec3(0, 0.9, 0), glm::vec3(0.1, 0.2, 0));
	HUDLevelname->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id(levelname);
	HUDLevelname->objectstyle = Objectstyle::Button;

	//PARTICLEMANAGER->LoadEmitter("particle1.json");

	INGAMELOGIC->InGameInit();
	SOUNDMANAGER->PlaySounds(Background, true);
	FACTORY->GetPlayer()->GetComponent<Body>()->GroundType = Grounded::Ground;
	player = FACTORY->GetPlayer();
	STATEMANAGER->InitplayerPos = player->GetComponent<Transform>()->GetPosition();
	player->GetComponent<Animation>()->setFrame(.25f);
	player->GetComponent<Animation>()->setTime(.25f);
	_camPaceSpeed = 5.0f;
	_camPacedirction = CAMERA->cameraPos - player->GetComponent<Transform>()->position;
	_camStartPosition = glm::vec3{ 0.f, 0.f, 0.f };

	CAMERA->IsCameraAttached = true;
	CAMERA->cameraUp.x = 0;
	CAMERA->cameraUp.y = 1;


	camAct.cameraOriginPos = CAMERA->cameraPos;
	CAMERA->cameraPos.z = 1000.f;
	CAMERA->CenterOfCamera.x = FACTORY->LeftBoundary()->GetComponent<Transform>()->position.x + (FACTORY->RightBoundary()->GetComponent<Transform>()->position.x - FACTORY->LeftBoundary()->GetComponent<Transform>()->position.x) *.5f;
	CAMERA->CenterOfCamera.y = FACTORY->DownBoundary()->GetComponent<Transform>()->position.y + (FACTORY->UpBoundary()->GetComponent<Transform>()->position.y - FACTORY->DownBoundary()->GetComponent<Transform>()->position.y)*.5f;
	CAMERA->cameraPos.x = CAMERA->CenterOfCamera.x;
	CAMERA->cameraPos.y = CAMERA->CenterOfCamera.y;
	STATEMANAGER->IsDrawing = false;
	//CAMERA->lookatMap(false); 


//	camerObj = FACTORY->CreateArchetype(ReadingArchetype("Button.json"));
//	camerObj->objectstyle = Objectstyle::Camera;

	/////////////////////////////////////////////////// Loading Delay Function
	//movingToCenter = false;
	//static float loadingtimer = 3;

	//while(loadingtimer >0)
	//{6
	//	
	//	loadingtimer -= Timer::GetDelta();
	//	

	//}
	//loadingtimer = 3;
	//Delete Loading Image
	//Loading->IsLoadingObject = false;
	//FACTORY->Destroy(Loading);
	/////////////////////////////////////////////////////
	_playerPosition.x = player->GetComponent<Transform>()->GetPosition().x;
	_playerPosition.y = player->GetComponent<Transform>()->GetPosition().y;
	_playerPosition.z = 500.f;

	//if (STATEMANAGER->Loadtolevelname == "test.json")
	//{
	//	glm::vec3 rightblock = glm::vec3(0), leftblock = glm::vec3(0), upblock = glm::vec3(0), downblock = glm::vec3(0);
	//	// this is current block of map 
	//	upblock = FACTORY->UpBoundary()->GetComponent<Transform>()->position;
	//	downblock = FACTORY->DownBoundary()->GetComponent<Transform>()->position;
	//	rightblock = FACTORY->RightBoundary()->GetComponent<Transform>()->position;
	//	leftblock = FACTORY->LeftBoundary()->GetComponent<Transform>()->position;

	//	CAMERA->CenterOfCamera.x = leftblock.x + (std::abs(leftblock.x - rightblock.x)*.5f);
	//	CAMERA->CenterOfCamera.y = upblock.y - (std::abs(upblock.y - downblock.y)*.5f);

	//	CAMERA->cameraPos = glm::vec3(CAMERA->CenterOfCamera, 500.f);
	//}
	static float loadingtimer = 0.1f;

	while (loadingtimer >0)
	{

		loadingtimer -= Timer::GetDelta();


	}
	loadingtimer = 0.1f;
}

void Level1::Update(float dt)
{


	
	//MakingInstructions(dt);
	
	camAct.Update(dt);

#ifdef _DEBUG
	CheatKeyFunctions();
#endif
		
	if (STATEMANAGER->b_IsReplayStart) {

		ReplayerInfo temp;
		temp.Pos = player->GetComponent<Transform>()->GetPosition();
		temp.aniframe = player->GetComponent<Animation>()->getFrame();
		temp.anitime = player->GetComponent<Animation>()->getTime();
		temp.mouseinfo = player->GetComponent<Animation>()->isFlippedX();
		STATEMANAGER->Replayerinfo.push(temp);
	}

	if (!ENGINE->GetGameStateIsOn())
	{
	
		delete INGAMELOGIC;
		delete LOGGINGSYSTEM;
	}

	if (IsAutoplay)
	{
		float Xpos = file.mRoot["PositionX"][j].asFloat();
		float Ypos = file.mRoot["PositionY"][j].asFloat();
		float Frame = file.mRoot["Frame"][j].asFloat();
		float FrameTime = file.mRoot["FrameTime"][j].asFloat();
		bool Pressed = file.mRoot["isFlippedX"][j].asBool();
		FACTORY->GetPlayer()->GetComponent<Transform>()->SetPosition(glm::vec3(Xpos, Ypos, 0));
		FACTORY->GetPlayer()->GetComponent<Animation>()->setFrame(Frame);
		FACTORY->GetPlayer()->GetComponent<Animation>()->setTime(FrameTime);
		FACTORY->GetPlayer()->GetComponent<Animation>()->setFlipX(Pressed);
		j++;
	}
	
	INGAMELOGIC->InGameUpdate(dt);
	if (STATEMANAGER->b_IsReplay)
	{
		APP->b_Win = false;
	}
	if (APP->b_Win)
	{
		STATEMANAGER->b_IsReplayStart = false;
		STATEMANAGER->b_IsReplay = true;
	}
	//camAct.cameraSetting(CameraPosType::EN_BOUNDARY);
	if (Input::IsAnyTriggered())
	{
		
		//camAct.cameraSetting(CameraPosType::EN_BOUNDARY);
	}

	
	if (APP->b_Lose)
	{
		if(LosesoundOnetime)
		SOUNDMANAGER->PlaySounds(loseSound, false);
		LosesoundOnetime = false;
		CAMERA->IsCameraShaking = LOSECONDITIONLOGIC->isBoundaryLose ? false : true;
	}

	if (CAMERA->IsCameraShaking)
	{
	
		camAct.cameraOriginPos = CAMERA->cameraPos;
		camAct.ShakeCamera(dt);
	}

	if (STATEMANAGER->b_IsReplay)
	{
		static bool first = true;
		if (STATEMANAGER->ReplayInit)
		{
			MakeReplayerUI();
			TRIGGERLOGIC->Initialize();
			CAMERA->cameraUp.x = 0;
			CAMERA->cameraUp.y = 1;
			PHYSICS->gravityScale = -20.f;
			PHYSICS->GravityType = Gravity::y_Minus;
			PHYSICS->gravity = glm::vec3(0, PHYSICS->gravityScale, 0);
		}		
		first = false;
		SetReplayer();
	}
	if (!PARTICLEMANAGER->m_EmitterList.empty() && FACTORY->GetPlayer() != nullptr)
		PARTICLEMANAGER->m_EmitterList[0]->pos = FACTORY->GetPlayer()->GetComponent<Transform>()->position;


}

void Level1::Free(void)
{
	while (!STATEMANAGER->Replayerinfo.empty())
		STATEMANAGER->Replayerinfo.pop();
	CAMERA->isCentered = true;
	CAMERA->IsCameraShaking = false;
	STATEMANAGER->b_IsAutoplaying = false;
	IsAutoplay = false;
	j = 0;

	//LOGGINGSYSTEM->SavingLog();
	STATEMANAGER->b_IsGameLevel = false;
 	CAMERA->cameraUp.x = 0;
	CAMERA->cameraUp.y = 1;
	PHYSICS->gravityScale = -20.f;
	PHYSICS->GravityType = Gravity::y_Minus;
	PHYSICS->gravity = glm::vec3(0, PHYSICS->gravityScale, 0);
	//SOUNDMANAGER->StopSound(Background);
	STATEMANAGER->b_IsReplayStart = true;
	STATEMANAGER->b_IsReplay = false;
	STATEMANAGER->b_Relplay = false;
	INGAMELOGIC->InGameShutdown();
	//SOUNDMANAGER->DeleteSounds();
	FACTORY->DestroyAllObjects();
//	delete LEVELMANAGER;
	//delete LOGGINGSYSTEM;
	
}
void Level1::Unload()
{
	STATEMANAGER->b_IsReplayStart = false;
	STATEMANAGER->b_IsReplay = false;
}




void CheatKeyFunctions(void) {

	if (Input::IsTriggered(SDL_SCANCODE_F7))
		STATEMANAGER->MoveState(StatesList::MapEditor-1);



	if (Input::IsTriggered(SDL_SCANCODE_F8) && STATEMANAGER->i_LevelSelect != 13 && !STATEMANAGER->b_IsRot180 && !STATEMANAGER->b_IsRot90)
	{

		CAMERA->cameraUp.x = 0;
		CAMERA->cameraUp.y = 1;
	
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
	if (Input::IsTriggered(SDL_SCANCODE_F9) && !IsAutoplay)
	{

		STATEMANAGER->b_IsAutoplaying = true;
		IsAutoplay = true;
	}

	if (Input::IsTriggered(SDL_SCANCODE_M))
	{
		

		system("start RoteMap.exe");
	}
}



void MakingInstructions(float dt) 
{

	static float timeringame = 9;


	timeringame -= dt;
	if (IndicatorCheck &&timeringame <= 0 && STATEMANAGER->Loadtolevelname == "level1.json")
	{

		Indicator = FACTORY->CreateArchetype(ReadingArchetype("Button.json"));
		Indicator->GetComponent<Transform>()->SetPosition(glm::vec3(FACTORY->GetPlayer()->GetComponent<Transform>()->position.x - 80, 130 + FACTORY->GetPlayer()->GetComponent<Transform>()->position.y, FACTORY->GetPlayer()->GetComponent<Transform>()->position.z));
		Indicator->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("left.png");
		Indicator->GetComponent<Transform>()->scale.x = 80;
		Indicator->GetComponent<Transform>()->scale.y = 80;

		Indicator1 = FACTORY->CreateArchetype(ReadingArchetype("Button.json"));
		Indicator1->GetComponent<Transform>()->SetPosition(glm::vec3(FACTORY->GetPlayer()->GetComponent<Transform>()->position.x + 80, 130 + FACTORY->GetPlayer()->GetComponent<Transform>()->position.y, FACTORY->GetPlayer()->GetComponent<Transform>()->position.z));
		Indicator1->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("right.png");
		Indicator1->GetComponent<Transform>()->scale.x = 80;
		Indicator1->GetComponent<Transform>()->scale.y = 80;

		Indicator2 = FACTORY->CreateArchetype(ReadingArchetype("Button.json"));
		Indicator2->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("space.png");
		Indicator2->GetComponent<Transform>()->SetPosition(glm::vec3(FACTORY->GetPlayer()->GetComponent<Transform>()->position.x, 210 + FACTORY->GetPlayer()->GetComponent<Transform>()->position.y, FACTORY->GetPlayer()->GetComponent<Transform>()->position.z));
		Indicator2->GetComponent<Transform>()->scale.x = 210;
		Indicator2->GetComponent<Transform>()->scale.y = 80;
		IndicatorCheck = false;
	}




}