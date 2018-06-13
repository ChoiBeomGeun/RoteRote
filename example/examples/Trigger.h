/******************************************************************************/
/*!
\file	Trigger.h
\author	HyunJun Yoo
\par	email: hyunjun306@gmail.com
\par	Class:GAM250
\par	ThumbUp Engine
\date	2017/12/16

Trigger game logic header file
There are interacting between player and triggers.
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/


#pragma once
#include  "Factory.h"
#include "GameLogicSystem.h"

namespace TE
{
	class TriggerLogic : public GameLogicSystem
	{
	public:
		TriggerLogic();
		void Initialize() override;
		void Update(float dt) override;
		void Free(void) override;
		~TriggerLogic();
		void _90angle_toleft_button(Object* Trigger);
		void _90angle_toright_button(Object* Trigger);
		void _180anglebutton(Object* Trigger);
		void InitDegree(void);
		int NumberOfTriggersActivation = 0;
	private:
		bool isDegree[4]; // 90
		bool isDegree180[4];
		bool isDegree180double[4];

		unsigned int Limited90_3[3];
		unsigned int Limited180_3[3];

		unsigned int unusable_Trigger;

		void LimitedTexture(Object* trigger);
	};


	extern TriggerLogic* TRIGGERLOGIC;

}

