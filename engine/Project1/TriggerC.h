/******************************************************************************/
/*!
\file   TriggerC.h
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

Trigger Component header file.
*/
/******************************************************************************/
#pragma once

#pragma once
#include "Component.h"
#include "GameLogic.h"
//#include "vector3.h"
#include "Transform.h"


#include "glew.h"
typedef void(Action)(void);
//#include "Mat44.h"
namespace TE {

	class Trigger : public Component
	{
	public:
		Trigger();

		~Trigger();

		virtual void Initialize() override;


		Action * maction;
		float i_innertimer = 0;
		bool bIsTriggerOn = false;
		bool Trigger_useable = false;
		Transform * pTransform;
		Body * pbody;
		void ActionSet(Action *action);
		std::string TriggerType = "180";


	};
}