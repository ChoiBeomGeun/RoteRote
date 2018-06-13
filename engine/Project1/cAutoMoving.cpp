/******************************************************************************/
/*!
\file	cAutoMoving.cpp
\author HyunJun Yoo
\par    email: hyunjun306@gmail.com
\par    course: CS225
\date   12/16/2017
\brief

AutoMoving header
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "cAutoMoving.h"
#include "Graphics.h"
#include <algorithm>
#include "DebugUtil.h"
#include "SOIL.h"
#include "Object.h"
#include <string>
#include <algorithm>
using namespace TE;


AutoMoving::AutoMoving() : Component(ComponentType::CT_AutoMoving)
{



}

AutoMoving::~AutoMoving()
{

}

void AutoMoving::Initialize()
{
	mOriginPosition = GetOwner()->GetComponent<Transform>()->GetPosition();

}










