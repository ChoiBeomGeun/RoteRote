/******************************************************************************/
/*!
\file   ComponentType.h
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

ComponentType List
*/
/******************************************************************************/

#pragma once

namespace TE{

	typedef enum {


	CT_INVALID = 0,
	CT_TRANSFORM,
	CT_CONTROLLER,
	CT_SPRITE,
	/*CT_PARTICLE,*/
	CT_BODY,
	CT_BUTTON,
	CT_TRIGGER,
	CT_ANIMATION,
	CT_NUMCOMPONENT


	}ComponentType;





}