/******************************************************************************/
/*!
\file   Option.h
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

Option state source file
There are changing resolution , mute sounds, toggle fullscreen
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once
#include "BaseStage.h"


class Option :public BaseStage
{
public:
	Option();
	~Option();
	void Load(void);
	void Init(void);
	void Update(float dt);
	void Free(void);
	void Unload(void);

private:
	int MenuCounter;
};
