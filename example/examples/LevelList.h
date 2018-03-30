#pragma once
#include <iostream>

enum LevelList{
	level1,
	level2,
	level3,
	quit
};

static std::string toString(int list)
{


	switch (list)
	{
	case level1: return "1.png";
		break;
	case level2: return "2.png";
		break;
	case level3: return "3.png";
		break;
	case quit: return "box.png";
		break;
	}

	return "box.png";
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
