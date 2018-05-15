/******************************************************************************/
/*!
\file   InGameLogic.cpp
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

InGameLogic System source file
All content 2017 DigiPen (USA) Corporation, all rights reserved.
/******************************************************************************/

#include "InGameLogic.h"
#include  "Trigger.h"
#include  "Clearzone.h"
#include  "Factory.h"
#include "StateManager.h"
#include "Trigger.h"
#include "Replay.h"
#include "Application.h"
#include "LoseconditionLogic.h"
#include "logging.h"

using namespace  TE;


void* operator new(std::size_t sz, char * ch, long line) {
	std::printf("File name : %s\n Code line :  %d", ch, line);
	std::printf("global op new called, size = %zu\n", sz);
	return std::malloc(sz);
}


namespace TE {


	InGameLogic * INGAMELOGIC = nullptr;


}
InGameLogic::InGameLogic()
{
  	LOGGINGSYSTEM = new LoggingSystem;
	INGAMELOGIC = this;

}

InGameLogic::~InGameLogic()
{


}

void InGameLogic::InGameInit()
{

	for (auto IngameLogics : GameLogicList)
		IngameLogics->Initialize();
	bool b_playerExist = false;
	bool b_colorzoneExist = false;
	for (auto Objects : FACTORY->ObjectIDMap)
	{
		if (FACTORY->GetPlayer())
			b_playerExist = true;
		if (FACTORY->ObjectIDMap[Objects.first]->objectstyle == Objectstyle::Clearzone)
			b_colorzoneExist = true;
	}

	if (!(b_playerExist || b_colorzoneExist))
	//	DEBUG_ASSERT(true, "Opps ! There is no player or clearzone");
	APP->b_Win = false;
	APP->b_Lose = false;
	LOGGINGSYSTEM->Initialize();
}
void InGameLogic::InGameLogicAdd(GameLogicSystem * game_logic_system)
{
	GameLogicList.push_back(game_logic_system);

}
void TE::InGameLogic::DestoryAllGameLogics(void)
{

	for (unsigned i = 0; i <GameLogicList.size(); ++i)
	{
		delete GameLogicList[GameLogicList.size() - 1 - i];

	}
	GameLogicList.erase(GameLogicList.begin(), GameLogicList.end());
	
}
void InGameLogic::InGameUpdate(float dt)
{
	for (auto IngameLogics : GameLogicList)
		IngameLogics->Update(dt);
	LOGGINGSYSTEM->Update(dt);

	for (auto Objects : FACTORY->ObjectIDMap)
	{
		if (STATEMANAGER->b_IsRotating)
			if (FACTORY->ObjectIDMap[Objects.first]->objectstyle == Objectstyle::Box)
				FACTORY->ObjectIDMap[Objects.first]->GetComponent<Body>()->pm_velocity = glm::vec3(0);
	}
}

void InGameLogic::InGameShutdown(void)
{
#ifdef _DEBUG 
	for (auto IngameLogics : GameLogicList)
		IngameLogics->Free();
#endif // !

//	PHYSICS->Initialize();
//	LOGGINGSYSTEM->Free();
	//CAMERA->view.RotateZ(TUMath::DegreeToRadian(0));
//	PHYSICS->gravity = glm::vec3(0, -20, 0);
	//	PHYSICS->gravity.Set(Vector3(0, -20, 0));
}