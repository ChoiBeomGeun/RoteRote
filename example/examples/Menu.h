/******************************************************************************/
/*!
\file   Menu.h
\author Kyungook.Park
\par    email: qkrruddn6680@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/04/19

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
#include "CameraMovement.h"

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

	void Selection_Text(void);

private:
	TE::CameraAction MenuCam;
	bool LeftRotate;
	bool RightRotate;
	bool ConfirmationIsOn = false;
	bool HowToPlayIsOn = false;
	bool IsRotating;
	bool IsSelected;
	bool IsTextChanged;

	int select_index;
	unsigned int Menu_Start;		//Is it Okay To sign texture as just int not unsigned int
	unsigned int Menu_HowToPlay;
	unsigned int Menu_Quit;
	unsigned int Menu_Option;

	MenuList Selection;

	float delta_angle;
	int selection_angle;

	int rotation_radius;

	unsigned int MenuSound;
	unsigned int MoveSound;
	unsigned int SelectSound;
};
