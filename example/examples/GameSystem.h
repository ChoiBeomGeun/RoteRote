/******************************************************************************/
/*!
\file	GameSystem.h
\author	Choi Beom Geun
\par	email: o77151@gmail.com
\par	Class:GAM250
\par	ThumbUp Engine
\date	06/13/2018

A pure virtual base class(an interface) that is the base class
for all systems used  by the Engine
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/


#pragma once
namespace TE
{
	class GameSystems
	{
	public:
		//Initialize the system
		virtual void Initialize() {};
		//All systems are updated every frames
		virtual void Update(float dt) = 0;
		// All systems need a virtual destructor to their destructor caelled
		virtual ~GameSystems() {};
	};
} // namespace TE End