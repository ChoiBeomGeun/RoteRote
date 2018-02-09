/******************************************************************************/
/*!
\file   LevelSelect.cpp
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

Level selection stage header file
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/


#pragma once
#include "BaseStage.h"


class LevelSelect :public BaseStage
{
public:
	LevelSelect();
	~LevelSelect();
	void Load(void);
	void Init(void);
	void Update(float dt);
	void Free(void);
	void Unload(void);

private:
	int LevelSelectCounter;
};
