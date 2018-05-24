/******************************************************************************/
/*!
\file   Clearzone.cpp
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

Clear zone game logic source file
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "Clearzone.h"
#include  "Factory.h"
#include  "InGameLogic.h"
//extern InGameLogic LevelInit;
#include "Application.h"
#include "Input.h"
#include "StateManager.h"
#include "SoundManager.h"
#include "ParticleManager.h"
using namespace TE;
static Object *player;
static Object *obj;
SOUNDID WinSound;
namespace TE {


	ClearzoneLogic * CLEARZONELOGIC = nullptr;


}




ClearzoneLogic::ClearzoneLogic()
{
	CLEARZONELOGIC = this;


}
void ClearzoneLogic::Initialize()
{
	player = FACTORY->GetPlayer();

}



void ClearzoneLogic::Update(float dt)
{
for (auto Objects : FACTORY->ObjectIDMap)
	{

		if (Objects.second->objectstyle == Objectstyle::Clearzone)
			obj = Objects.second;

	}
	if (obj != NULL)
		obj->GetComponent<Transform>()->angle += 20 * dt;
	if (obj != NULL)
	{
		player = FACTORY->GetPlayer();
		if (PHYSICS->RectvsRectCollisionCheck(player->GetComponent<Transform>(), obj->GetComponent<Transform>()))
		{

			WinSound = SOUNDMANAGER->LoadSound("win3.mp3");
			SOUNDMANAGER->PlaySounds(WinSound, false);

			APP-> b_Win = true;//	LevelInit.b_Win = true;
							  //STATEMANAGER->b_Relplay = true;
			
			
			PHYSICS->gravityScale = -20.f;
			PHYSICS->GravityType = Gravity::y_Minus;
			PHYSICS->gravity = glm::vec3(0, PHYSICS->gravityScale, 0);
			//STATEMANAGER->b_IsReplayFirst = true;
		}
	}
}


Object* ClearzoneLogic::GetClearzone(void)
{

	for (auto Objects : FACTORY->ObjectIDMap)
	{

		if (Objects.second->objectstyle == Objectstyle::Clearzone)
			return Objects.second;
	}
	return NULL;
}
void TE::ClearzoneLogic::Free(void)
{

	SOUNDMANAGER->DeleteSounds();
}
ClearzoneLogic::~ClearzoneLogic()
{
}