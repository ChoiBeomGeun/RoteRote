/******************************************************************************/
/*!
\file   Button.cpp
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM250
\par    ThumbUp Engine
\date   2017/11/29

implementation for Button
*/
/******************************************************************************/
#include "Sprite.h"
#include "Graphics.h"
#include <algorithm>
#include "DebugUtil.h"
#include "SOIL.h"
#include "Object.h"
#include "Button.h"
using namespace TE;

typedef void(Action)(void);
Button::Button() : Component(ComponentType::CT_BUTTON)
{

 

}
Button::~Button()
{
	

}



void Button::Initialize()
{


}

void Button::ActionSet(Action * action)
{
	mAction = action;
}














