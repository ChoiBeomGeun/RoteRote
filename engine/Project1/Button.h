/******************************************************************************/
/*!
\file   Button.h
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM250
\par    ThumbUp Engine
\date   2017/11/29

Button
*/
/******************************************************************************/
#pragma once
#include "Component.h"
#include "Transform.h"


#include "glew.h"
typedef void(Action)(void);

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