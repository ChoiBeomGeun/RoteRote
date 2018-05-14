/******************************************************************************/
/*!
\file   LevelSelect.h
\author Kyungook.Park
\par    email: qkrruddn6680@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/04/19

Level selection stage header file
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/


#pragma once
#include "BaseStage.h"
#include "LevelList.h"
#include "CameraMovement.h"

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
	TE::CameraAction LvlSelectCam;

	int LevelList =  LevelList::level1;
	bool IsRotating;

	float selectAngle;
	bool IsLeftPressed = false;
	bool IsRightPreesed = false;

	TE::Object* obj_confirmation;
	bool IsConfirmationOn = false;


	void Rotation(void);

	unsigned int Levelpng[LevelList::quit + 1] = {0};
};
