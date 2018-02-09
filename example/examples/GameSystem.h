/*
GameSystem.h
TEGeun Choi
A pure virtual base class(an interface) that is the base class
for all systems used  by the Engine
*/


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