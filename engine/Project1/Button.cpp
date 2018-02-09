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














