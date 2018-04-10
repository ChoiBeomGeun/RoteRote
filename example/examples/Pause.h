
/******************************************************************************/
/*!
\file   Pause.cpp
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

Pause State for the game
This states include ¡°back game¡±, ¡°Restart game¡±, ¡°Go to level Select¡±, ¡°How to play¡±
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "BaseStage.h"
#include "CameraMovement.h"

class Pause :public BaseStage
{
public:
	Pause();
	~Pause();
	void Load(void);
	void Init(void);
	void Update(float dt);
	void Free(void);
	void Unload(void);

private:
	int PauseCounter;
	TE::CameraAction PauseCam;
};
