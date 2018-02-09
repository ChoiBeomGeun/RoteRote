#include "Sprite.h"
#include "Graphics.h"
#include <algorithm>
#include "DebugUtil.h"
#include "SOIL.h"
#include "Object.h"

#include "Trigger.h"
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


}
void Trigger::ActionSet(Action * action)
{
	maction = action;
}














