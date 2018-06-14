/******************************************************************************/
/*!
\file	Animation.cpp
\author	HyunJun Yoo
\par	email: hyunjun306@gmail.com
\par	Class:GAM250
\par	ThumbUp Engine
\date	06/13/2017

Animation class implementation file.
it is a component type.
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "Animation.h"
#include "Graphics.h"
#include "DebugUtil.h"
#include "SOIL.h"
#include "Object.h"

namespace TE {
	Animation::Animation() : Component(ComponentType::CT_ANIMATION),
		_time(0.f), _frame(0.f), isFlipX(FALSE), isFlipY(FALSE), _isPressed(FALSE)
	{
		GRAPHICS->AnimationList.push_back(this);
	}

	void Animation::Initialize()
	{

	}


	Animation::~Animation()
	{
		GRAPHICS->AnimationList.erase(std::find(GRAPHICS->AnimationList.begin(), GRAPHICS->AnimationList.end(), this));
	}

	void Animation::IterateFrame(float frame, float limit)
	{
			_frame += frame;
		if (_frame >= limit)
		{
			_frame = 0.f;
		}
	}
	void Animation::IterateSplash(float frame)
	{
		_frame += frame;
	}
	void Animation::setFlipX(bool isFlippedX)
	{
		isFlipX = isFlippedX;
	}
	void Animation::setFlipY(bool isFlippedY)
	{
		isFlipY = isFlippedY;
	}
	void Animation::setTime(float timeset)
	{
		_time = timeset;
	}
	void Animation::setPressed(bool isPressed)
	{
		_isPressed = isPressed;
	}
	void Animation::setFrame(float frameSet)
	{
		_frame = frameSet;
	}
	float Animation::getFrame()
	{
		return _frame;
	}
}