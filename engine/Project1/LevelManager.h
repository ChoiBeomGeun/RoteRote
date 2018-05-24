/******************************************************************************/
/*!
\file   LevelManager.h
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

Using jsoncpp, loading and saving the level
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

#include <string>
#include <vector>
namespace TE {
	class LevelManager
	{
	public:
		LevelManager();

		void LoadLevel(std::string  path);
		void SaveLevel(std::string  path);

	};

	extern LevelManager* LEVELMANAGER;
}
