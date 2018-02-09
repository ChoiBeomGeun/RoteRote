/******************************************************************************/
/*!
\file   Replay.h
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

#pragma once
#include "BaseStage.h"
#include  "GameLogicSystem.h"
#include "InGameLogic.h"
namespace  TE
{
	
	class ReplaySystem : public BaseStage
	{
	public:
		//Initialize the system
		void Load(void);
		void Init(void);
		void Update(float dt);
		void Free(void);
		void Unload(void);
		ReplaySystem();
		~ReplaySystem();


		unsigned int action = 0;

	};


}

