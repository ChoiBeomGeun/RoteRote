/******************************************************************************/
/*!
\file   controller.h
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM250
\par    ThumbUp Engine
\date   2017/11/29

Controller component header file
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Component.h"
#include "GameLogic.h"

namespace TE {




	class Controller : public Component
	{
	public:
		Controller() : Component(ComponentType::CT_CONTROLLER)
		{

		}
		~Controller() { GAMELOGIC->ControllerList.erase(myController); }

		virtual void Initialize() {
			myController = GAMELOGIC->ControllerList.insert(GAMELOGIC->ControllerList.end(), this);
		}


		virtual void Update(float dt) { dt = dt; }

	protected:
		std::list<Controller*>::iterator myController;
	};

}