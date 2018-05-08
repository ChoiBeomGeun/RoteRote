/*!
\file  OptionList.h
\author Kyungook.Park
\par    email: qkrruddn6680@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/04/19

MenuList header
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
enum class OptionList {
	Option_SoundOnOff,
	Option_Credits,
	Option_Back,
	Option_Full
};

//MenuList& operator++(MenuList &selection) {
//	switch (selection)
//	{
//	case MenuList::Menu_Start: return selection = MenuList::Menu_HowToPlay;
//	case MenuList::Menu_HowToPlay: return selection = MenuList::Menu_Quit;
//	case MenuList::Menu_Quit: return selection = MenuList::Menu_Option;
//	case MenuList::Menu_Option: return selection = MenuList::Menu_Start;
//	default:
//		return selection;
//	}
//}