#pragma once
	enum class MenuList {
		Menu_Start,
		Menu_Option,
		Menu_Quit,
		Menu_HowToPlay
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