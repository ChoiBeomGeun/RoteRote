/******************************************************************************/
/*!
\file   LevelList.h
\author Kyungook.Park
\par    email: qkrruddn6680@gmail.com
\par    Class:GAM250
\par    ThumbUp Engine
\date   2018/05/29

LevelList header
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include <iostream>
#include <sstream>

enum LevelList{
	level1,
	level2,
	level3,
	level4,
	level5,
	level6,
	level7,
	level8,
	level9,
	level10,
	level11,
	level12,
	level13,
        level14,
        level15,
        level16,
        level17,
        level18,
        level19,
        level20,
	quit
};


template <typename T>
std::string NumberToString(T Number)
{
	if (Number == LevelList::quit + 1)
		return "box";

	std::ostringstream ss;
	ss << Number;
	return ss.str();
}

