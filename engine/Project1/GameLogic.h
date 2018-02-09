/******************************************************************************/
/*!
\file   GameLogic.h
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

Game Logic header file
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "System.h"
#include "DebugUtil.h"
#include <list>
namespace TE
{
	//Forward declaration of controller Component
	class Controller;
	class GameLogic :public Systems {
	public:
		GameLogic();
		~GameLogic();

		void Initialize() override;
		void Update(float dt) override;
		//Lists of controller components
		std::list<Controller*> ControllerList;
		
	private:
	
	};
	
	extern GameLogic * GAMELOGIC;

}