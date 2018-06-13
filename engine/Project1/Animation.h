/******************************************************************************/
/*!
\file	Animation.h
\author	HyunJun Yoo
\par	email: hyunjun306@gmail.com
\par	Class:GAM250
\par	ThumbUp Engine
\date	06/13/2018

Animation class header file.
It contains animation information
Flipping, Frame, _isPressed
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once


#include "Component.h"

#include "Transform.h"
#include  <string>
#include "glew.h"
#include "SDL.h"
#include <iostream>

namespace TE {
	class Animation : public Component
	{
	public:
		Animation();
		virtual void Initialize() override;
		~Animation();

		void IterateFrame(float frame, float limit);
		void IterateSplash(float frame);
		//setter
		void setFlipX(bool isFlippedX);
		void setFlipY(bool isFlippedY);
		void setTime(float timeset);
		void setPressed(bool isPressed);
		void setFrame(float frameSet);
		//getter
		float getTime() { return _time; }
		float getFrame();
		bool isFlippedX() { return isFlipX; }
		bool isFlippedY() { return isFlipY; }
		bool isPressed() { return _isPressed; }

		// public variables
		bool isJumping;
		bool _isOnWall;
		float _limit_frame;
	private:
		float _time; // Timer
		float _frame; // animationX
		bool isFlipX;
		bool isFlipY;
		bool _isPressed;
		float _movingframe;



	};

}


