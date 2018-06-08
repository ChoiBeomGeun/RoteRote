/******************************************************************************/
/*!
\file   Intro.cpp
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

Intro
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "Intro.h"
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
#include "Archetype.h"
using namespace TE;
Jsonclass fileIntro;
Json::Value rootIntro;
int autoindex = 0;
bool Enter = false;
Intro::Intro()
{

}

Intro::~Intro()
{
}

void Intro::Load()
{
	LEVELMANAGER->LoadLevel("intro.json");

}

void Intro::Init()
{

	std::string saveLevel = "SAVE";




	fileIntro.ReadFile(".\\autoplays.\\" + saveLevel);



}

void Intro::Update(float dt)
{
	static bool stop = true;

	if (Input::IsTriggered(SDL_SCANCODE_R))
	{
		stop = true;
		autoindex = 0;
	}

	if (Input::IsTriggered(SDL_SCANCODE_Y))
	{
		

		stop = false;

	}

	dt = dt;
	float Xpos = fileIntro.mRoot["PositionX"][autoindex].asFloat();
	float Ypos = fileIntro.mRoot["PositionY"][autoindex].asFloat();
	/*float Frame = fileIntro.mRoot["Frame"][autoindex].asFloat();
	float FrameTime = fileIntro.mRoot["FrameTime"][autoindex].asFloat();
	bool Pressed = fileIntro.mRoot["isFlippedX"][autoindex].asBool();*/

	if (stop) {
		if (!(int(FACTORY->ObjectIDMap[13]->GetComponent<Transform>()->position.x) > 200)) {
			FACTORY->ObjectIDMap[13]->GetComponent<Transform>()->SetPosition(glm::vec3(Xpos, Ypos, 0));
		
		}
		else
			Enter = true;
		if(!(int(FACTORY->ObjectIDMap[17]->GetComponent<Transform>()->position.x) ==171 && ((int)FACTORY->ObjectIDMap[17]->GetComponent<Transform>()->position.y == -255)))
		FACTORY->ObjectIDMap[17]->GetComponent<Transform>()->SetPosition(glm::vec3(Xpos + 90, Ypos, 0));
	}

	if(Enter)
	{
		camActIntro.ShakeCamera(dt);
		if (FACTORY->ObjectIDMap[14]->GetComponent<Transform>()->position.x >
			FACTORY->ObjectIDMap[13]->GetComponent<Transform>()->position.x) {
			FACTORY->ObjectIDMap[13]->GetComponent<Transform>()->position.x += dt * 100;
			
			FACTORY->ObjectIDMap[13]->GetComponent<Transform>()->rotation += dt ;
		}
		if (FACTORY->ObjectIDMap[14]->GetComponent<Transform>()->position.y >
			FACTORY->ObjectIDMap[13]->GetComponent<Transform>()->position.y) {
			FACTORY->ObjectIDMap[13]->GetComponent<Transform>()->position.y += dt * 100;
			FACTORY->ObjectIDMap[13]->GetComponent<Transform>()->rotation += dt ;
		}



	}


	//FACTORY->ObjectIDMap[13]->GetComponent<Animation>()->setFrame(Frame);
	//FACTORY->ObjectIDMap[13]->GetComponent<Animation>()->setTime(FrameTime);
	//FACTORY->ObjectIDMap[13]->GetComponent<Animation>()->setFlipX(Pressed);
	autoindex++;

}

void Intro::Free(void)
{
	//	delete INGAMELOGIC;
	printf("IntroFree\n");
	//	INGAMELOGIC->InGameShutdown();
}



void Intro::Unload()
{
	printf("IntroUnload\n");
}