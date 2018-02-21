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
#define LOGGINGSTART false

using namespace TE;
void MakingInstructions(void);
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
	STATEMANAGER->ReplayPosition.clear();
	STATEMANAGER->AniSave.clear();


	LEVELMANAGER->LoadLevel(STATEMANAGER->Loadtolevelname);

	std::string levelname = std::to_string(STATEMANAGER->i_LevelSelect) + ".png";
	HUDLevelname = FACTORY->CreateHUD(glm::vec3(0, 0.9, 0), glm::vec3(0.1, 0.2, 0));
	HUDLevelname->GetComponent<Sprite>()->texture_load(levelname);
	HUDLevelname->objectstyle = Objectstyle::Button;

	
	/////////////// MovingObject TestCode 
	/////////////// If you want to test Moving Object, delete below comment
	//Movingtest = FACTORY->CreateArchetype(ReadingArchetype("Wall.json"));
	//Movingtest->GetComponent<Transform>()->SetPosition(glm::vec3(-550, -75, 0));
	//Movingtest->AddComponent<AutoMoving>();
	//Paths tempinit;
	//tempinit.pathway.x = -550;
	//tempinit.pathway.y = -75;
	//Paths temp;
	//temp.pathway.x = -950;
	//temp.pathway.y = 450;
	//Paths temp2;
	//temp2.pathway.x = -550;
	//temp2.pathway.y = 650;
	//Paths temp3;
	//temp3.pathway.x = 250;
	//temp3.pathway.y = 450;
	//Movingtest->GetComponent<AutoMoving>()->mPaths.push_back(tempinit);
	//Movingtest->GetComponent<AutoMoving>()->mPaths.push_back(temp);
	//Movingtest->GetComponent<AutoMoving>()->mPaths.push_back(temp2);
	//Movingtest->GetComponent<AutoMoving>()->mPaths.push_back(temp3);




	INGAMELOGIC->InGameInit();
	SOUNDMANAGER->PlaySounds(Background, true);
	FACTORY->GetPlayer()->GetComponent<Body>()->GroundType = Grounded::Ground;
	player = FACTORY->GetPlayer();
	STATEMANAGER->InitplayerPos = player->GetComponent<Transform>()->GetPosition();
	player->GetComponent<Animation>()->setFrame(1.0f / 8);
	player->GetComponent<Animation>()->setTime(1.0f / 8);
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
	movingToCenter = false;
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



	lookAtMap();
}

void Level1::Update(float dt)
{
	MakingInstructions();


#ifdef _DEBUG
	CheatKeyFunctions();
#endif
	//std::cout << CAMERA->cameraPos.x << " " << CAMERA->cameraPos.y << " \n";





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
	
	if (Input::IsAnyTriggered())
	{
		movingToCenter = true;
	}

	if (movingToCenter)
	{
		if (CenterToPlayer)
		{
			camAct.FollowPlayer(&CAMERA->m_camerObject->pos, &CAMERA->m_camerObject->scale, dt);
			//FollowPlayer(CAMERA->cameraPos, dt);
			
		}
		else
		{			
			CAMERA->cameraPos.x = FACTORY->GamePlayer->GetComponent<Transform>()->GetPosition().x;
			CAMERA->cameraPos.y = FACTORY->GamePlayer->GetComponent<Transform>()->GetPosition().y;
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


}

void Level1::Free(void)
{
	while (!STATEMANAGER->Replayerinfo.empty())
		STATEMANAGER->Replayerinfo.pop();
	CAMERA->isCentered = true;
	movingToCenter = false;
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


void Level1::lookAtMap()
{
	CAMERA->CenterOfCamera.x = FACTORY->LeftBoundary()->GetComponent<Transform>()->position.x + (FACTORY->RightBoundary()->GetComponent<Transform>()->position.x - FACTORY->LeftBoundary()->GetComponent<Transform>()->position.x) *.5f;
	CAMERA->CenterOfCamera.y = FACTORY->DownBoundary()->GetComponent<Transform>()->position.y + (FACTORY->UpBoundary()->GetComponent<Transform>()->position.y - FACTORY->DownBoundary()->GetComponent<Transform>()->position.y)*.5f;
	std::cout << "CenterOfX: " << CAMERA->CenterOfCamera.x << '\n';
	std::cout << "CenterOfY: " << CAMERA->CenterOfCamera.y << '\n';

	CAMERA->cameraPos = glm::vec3(glm::vec2(CAMERA->CenterOfCamera.x, CAMERA->CenterOfCamera.y), CAMERA->cameraPos.z);
	CAMERA->cameraPos.z = 999.f;
}

void CheatKeyFunctions(void) {

	if (Input::IsTriggered(SDL_SCANCODE_F7))
		STATEMANAGER->MoveState(StatesList::MapEditor);



	if (Input::IsTriggered(SDL_SCANCODE_F8) && STATEMANAGER->i_LevelSelect != 13 && !STATEMANAGER->b_IsRot180 && !STATEMANAGER->b_IsRot90)
	{

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
	if (Input::IsTriggered(SDL_SCANCODE_F9) && !IsAutoplay)
	{

		STATEMANAGER->b_IsAutoplaying = true;
		IsAutoplay = true;
	}

}



void MakingInstructions(void) {

	static float timeringame = 200;


	timeringame -= Timer::GetDelta();
	if (IndicatorCheck &&timeringame <= 0 && STATEMANAGER->Loadtolevelname == "level1.json")
	{

		Indicator = FACTORY->CreateArchetype(ReadingArchetype("Button.json"));
		Indicator->GetComponent<Transform>()->SetPosition(glm::vec3(FACTORY->GetPlayer()->GetComponent<Transform>()->position.x - 80, 130 + FACTORY->GetPlayer()->GetComponent<Transform>()->position.y, FACTORY->GetPlayer()->GetComponent<Transform>()->position.z));
		Indicator->GetComponent<Sprite>()->texture_load("left.png");
		Indicator->GetComponent<Transform>()->scale.x = 80;
		Indicator->GetComponent<Transform>()->scale.y = 80;

		Indicator1 = FACTORY->CreateArchetype(ReadingArchetype("Button.json"));
		Indicator1->GetComponent<Transform>()->SetPosition(glm::vec3(FACTORY->GetPlayer()->GetComponent<Transform>()->position.x + 80, 130 + FACTORY->GetPlayer()->GetComponent<Transform>()->position.y, FACTORY->GetPlayer()->GetComponent<Transform>()->position.z));
		Indicator1->GetComponent<Sprite>()->texture_load("right.png");
		Indicator1->GetComponent<Transform>()->scale.x = 80;
		Indicator1->GetComponent<Transform>()->scale.y = 80;

		Indicator2 = FACTORY->CreateArchetype(ReadingArchetype("Button.json"));
		Indicator2->GetComponent<Sprite>()->texture_load("space.png");
		Indicator2->GetComponent<Transform>()->SetPosition(glm::vec3(FACTORY->GetPlayer()->GetComponent<Transform>()->position.x, 210 + FACTORY->GetPlayer()->GetComponent<Transform>()->position.y, FACTORY->GetPlayer()->GetComponent<Transform>()->position.z));
		Indicator2->GetComponent<Transform>()->scale.x = 210;
		Indicator2->GetComponent<Transform>()->scale.y = 80;
		IndicatorCheck = false;
	}




}