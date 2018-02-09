#pragma once
#pragma once
/*
Button.h
Beomguen CHoi

*/
#pragma once
#include "Component.h"
#include "GameLogic.h"
#include "vector3.h"
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


		Transform * pTransform;
		Body * pbody;
		void ActionSet(Action *action);



	};
}