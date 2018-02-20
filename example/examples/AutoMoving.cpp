
/******************************************************************************/
/*!
\file   Trigger.cpp
\author HyunJun Yoo
\par    email: hyunjun306@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/12/16

Trigger game logic source file
There are interacting between player and triggers.
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/


#include "Trigger.h"
#include "StateManager.h"
#include "SoundManager.h"
#include "Timer.h"
#include "PlayerController.h"
#include "Automoving.h"
#include "cAutoMoving.h"

using namespace TE;
std::vector<Object*> VoAutoMovingObjects;

namespace TE {


	AutoMovingLogic * AUTOMOVINGLOGIC = nullptr;


}




AutoMovingLogic::AutoMovingLogic()
{

	AUTOMOVINGLOGIC = this;


}
void AutoMovingLogic::Initialize(void)
{
	for(auto oMovingthings : FACTORY->ObjectIDMap)
	{
		if (oMovingthings.second->HasComponent<AutoMoving>())
			VoAutoMovingObjects.push_back(oMovingthings.second);
	}

}

void AutoMovingLogic::Update(float dt)
{
	for(auto oMovingthing : VoAutoMovingObjects )
	{
		

	}
}

void AutoMovingLogic::Free(void)
{

}


AutoMovingLogic::~AutoMovingLogic()
{

}
