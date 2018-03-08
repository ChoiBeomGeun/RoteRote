/******************************************************************************/
/*!
\file   Menu.h
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

Menu state header file
There are ¡°go to level select state¡±, ¡°option state¡±, ¡°How to play state¡±
¡°Quit the game¡±
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once
#include "BaseStage.h"
#include "StateLists.h"
#include "MenuList.h"

class Menu  :public BaseStage
{
public:
	Menu();
	~Menu();
	void Load(void);
	void Init(void);
	void Update(float dt);
	void Free(void);
	void Unload(void);

	void DeltaAngle(void);

	void Selection_plus(void);
	void Selection_minus(void);
private:

	bool LeftRotate;
	bool RightRotate;

	bool IsRotating;
	bool IsSelected;

	int select_index;

	MenuList Selection;

	int delta_angle;
	int selection_angle;

	int rotation_radius;

	unsigned int MenuSound;
	unsigned int MoveSound;
	unsigned int SelectSound;
};
