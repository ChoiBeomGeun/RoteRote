/******************************************************************************/
/*!
\file   Body.cpp
\author KyungOok Park
\par    email: qkrruddn6680@gmail.com
\par    course: GAM250
\par	ThumbUp Engine
\date   12/12/2017
\brief

Every Object has body component except button and HUD.
This body class includes information that is used in calculating physics.

All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/


#include "Body.h"
#include "Physics.h"
#include "Transform.h"
//Get owner method returns pointer to Object
#include "object.h"
#include  "Factory.h"
#pragma once
using namespace TE;

Body::Body() : Component(CT_BODY)
{
	PHYSICS->m_vecprb.push_back(this);
	PHYSICS->m_Body[FACTORY->BodyID] = this;

	this->BodyID = FACTORY->BodyID;
	FACTORY->BodyID++;
}

Body::Body(glm::vec3 vel, float invmass) : Component(CT_BODY), pm_velocity(vel), pm_invmass(invmass)
{
}

Body::~Body()
{
	
}
void Body::Initialize()
{
	m_pTransform = GetOwner()->GetComponent<Transform>();
	m_force = glm::vec3(0);

	Jump = false;
	if (pm_invmass == 0)
		pm_mass = 0;
	else
		pm_mass = 1 / pm_invmass;
	GroundType = Grounded::Ground;
	//Put the pointer to component to the vector container,
	//so that Physics system can iterate through
}

void TE::Body::Update(float dt)
{
	dt = dt;
}



void Body::SetRestitution(float r)
{
	restitution = r;
}
