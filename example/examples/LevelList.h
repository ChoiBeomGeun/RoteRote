/*!
\file   LevelList.h
\author Kyungook.Park
\par    email: qkrruddn6680@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

LevelList header
All content 2017 DigiPen (USA) Corporation, all rights reserved.
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

//static std::string toString(int list)
//{
//
//
//	switch (list)
//	{
//	case level1: return "1.png";
//		break;
//	case level2: return "2.png";
//		break;
//	case level3: return "3.png";
//		break;
//	case quit: return "box.png";
//		break;
//	}
//
//	return "box.png";
//}

template <typename T>
std::string NumberToString(T Number)
{
	if (Number == LevelList::quit + 1)
		return "box";

	std::ostringstream ss;
	ss << Number;
	return ss.str();
}


//#define LIST_OF_LEVELS \
//	Level(lvl1)	\
//	Level(lvl2)	\
//	Level(lvl3) \
//	Level(Quit)
//
//enum LevelList {
//#define Level(lvl) lvl,
//	LIST_OF_LEVELS
//#undef Level
//};
//
//std::string toString(LevelList levellist)
//{
//	switch (levellist)
//	{
//#define Level(lvl) case lvl:	return #lvl;
//		LIST_OF_LEVELS
//#undef Level
//	}
//	return "Unknown";
//}

/*
*/
//#pragma once
//#include <iostream>
//
//#define LIST_OF_LEVELS \
//	LEVEL(ONE)		\
//	LEVEL(TWO)		\
//	LEVEL(THREE)		\
//	LEVEL(quit)
//
//
//enum LevelList {
//#define LEVEL(levels) levels,
//	LIST_OF_LEVELS
//#undef LEVEL
//};
//
////static std::string toString(int list)
////{
////
////
////	switch (list)
////	{
////	case level1: return "1.png";
////		break;
////	case level2: return "2.png";
////		break;
////	case level3: return "3.png";
////		break;
////	case quit: return "box.png";
////		break;
////	}
////
////	return "box.png";
////}
//
//static std::string toString(LevelList levellist)
//{
//	switch (levellist)
//	{
//#define LEVEL(levels) case levels:	return #levels;
//		LIST_OF_LEVELS
//#undef LEVEL
//	}
//	return "quit";
//}
//
////#define LIST_OF_LEVELS \
////	Level(lvl1)	\
////	Level(lvl2)	\
////	Level(lvl3) \
////	Level(Quit)
////
////enum LevelList {
////#define Level(lvl) lvl,
////	LIST_OF_LEVELS
////#undef Level
////};
////
