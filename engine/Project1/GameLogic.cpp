/******************************************************************************/
/*!
\file   GameLogic.cpp
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM250
\par    ThumbUp Engine
\date   2017/11/29

Game Logic cpp file
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "GameLogic.h"
#include "Controller.h"
#include  <list>
using namespace TE;

namespace TE {

	GameLogic * GAMELOGIC = nullptr;


}

GameLogic::GameLogic()
{
	DEBUG_ASSERT(GAMELOGIC != nullptr, "There can be only 1! Game Logic System ");
GAMELOGIC =  this;

}

GameLogic::~GameLogic()
{

	
}

void GameLogic::Initialize()
{

}

void GameLogic::Update(float dt)
{
//	std::list<Controller>::iterator itr = ControllerList.begin();
	for (auto element : ControllerList)
		element->Update(dt);
		

}
