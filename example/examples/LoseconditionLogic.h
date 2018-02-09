/******************************************************************************/
/*!
\file   LoseconditionLogic.h
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

Lose condition game logic header file
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include  "Factory.h"
#include "GameLogicSystem.h"

namespace TE
{
	class LoseconditionLogic : public GameLogicSystem
	{
	public:
		LoseconditionLogic();
		void Initialize() override;
		void Update(float dt) override;
		void Free(void) override;
		~LoseconditionLogic();


	};


	extern LoseconditionLogic* LOSECONDITIONLOGIC;

}

