/******************************************************************************/
/*!
\file   splash.cpp
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

Splash state for the game
There is a digipen logo, and if any key is triggered , going to next state
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "splash.h"
#include <stdio.h>
#include "Object.h"
#include "Factory.h"
#include "Input.h"
#include "StateManager.h"
#include "Engine.h"
#include  "LevelManager.h"
#include  <string>
#include "Application.h"
#include "SoundManager.h"
#include "InGameLogic.h"
#include "ArchetypeEditor.h"
using namespace TE;
float Timer2 = 0;
float angle = 0;

splash::splash()
{

}

splash::~splash()
{
}

void splash::Load()
{
	
	printf("splashLoad\n");
}

void splash::Init()
{
//#ifndef _DEBUG
//	APP->toggle_fullscreen(APP->getWindow(), true);
//#endif
	
	
	//CAMERA->view.RotateZ(TUMath::DegreeToRadian(0.f));
	CAMERA->Initialize();
	std::string path2 = "splash.json";


	LEVELMANAGER->LoadLevel(path2);

	printf("splashInit\n");

}

void splash::Update(float dt)
{
	dt = dt;
	//CAMERA->lookat(Vector3(0, 0, 3), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f));

	Timer2 += 5;
	if (Input::IsAnyTriggered())
		STATEMANAGER->Next();



	if(CAMERA->cameraPos.x <= 1788 && Timer2 >= 200)
	CAMERA->cameraPos.x += 10.5;
	if (CAMERA->cameraPos.x >= 1788)
	{
		STATEMANAGER->Next();

	}
	//}


}

void splash::Free(void)
{
//	delete INGAMELOGIC;
	printf("splashFree\n");
//	INGAMELOGIC->InGameShutdown();
}



void splash::Unload()
{
	printf("splashUnload\n");
}