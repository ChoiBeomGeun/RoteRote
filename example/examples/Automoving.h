/******************************************************************************/
/*!
\file   Trigger.h
\author HyunJun Yoo
\par    email: hyunjun306@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/12/16

Trigger game logic header file
There are interacting between player and triggers.
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/


#pragma once
#include  "Factory.h"
#include "GameLogicSystem.h"

namespace TE
{
	class AutoMovingLogic : public GameLogicSystem
	{
	public:
		AutoMovingLogic();
		void Initialize() override;
		void Update(float dt) override;
		void Free(void) override;
		~AutoMovingLogic();

	private:


	};


	extern AutoMovingLogic* AUTOMOVINGLOGIC;

}

