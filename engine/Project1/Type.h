/******************************************************************************/
/*!
\file	Type.h
\author KyungOok Park
\par    email: qkrruddn6680@gmail.com
\par    course: GAM250
\par	ThumbUp Engine
\date   12/12/2017
\brief
enum type, Gravity direction and Player's state.

All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once
#include <iostream>

enum Grounded
{
	Air = 0,
	Left,
	Right,
	Ground
};

enum Gravity
{
	y_Minus = 0,
	x_Minus,
	y_Plus,
	x_Plus
};