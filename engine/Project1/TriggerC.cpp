/******************************************************************************/
/*!
\file   TriggerC.cpp
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM250
\par    ThumbUp Engine
\date   2017/11/29

Trigger Component source file.
*/
/******************************************************************************/

#include "Sprite.h"
#include "Graphics.h"
#include <algorithm>
#include "DebugUtil.h"
#include "SOIL.h"
#include "Object.h"

#include "TriggerC.h"
using namespace TE;

typedef void(Action)(void);
Trigger::Trigger() : Component(ComponentType::CT_TRIGGER)
{



}
Trigger::~Trigger()
{


}



void Trigger::Initialize()
{
	LifeTime = MaxLife;

}


void Trigger::ActionSet(Action * action)
{
	maction = action;
}
