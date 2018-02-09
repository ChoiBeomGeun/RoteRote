#pragma once
/*
Button.h
Beomguen CHoi

*/
#pragma once
#include "Component.h"
#include "GameLogic.h"
//#include "vector3.h"
#include "Transform.h"


#include "glew.h"
typedef void(Action)(void);
//#include "Mat44.h"
namespace TE {

	class Button : public Component
	{
	public:
		Button();

		~Button();

		virtual void Initialize() override;

		void ActionSet(Action *action);


		
		Transform * pTransform;
		Action  * mAction;

		float Color[4] = { 0,0,0,0 };
		unsigned int TextureId;

	};
}