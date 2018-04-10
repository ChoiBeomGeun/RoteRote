/**
\file        Body.cpp
\author      KyungOok Park
\par         email: qkrruddn6680@gmail.com
\par         course: GAM200
\par	ThumbUp Engine
\date        12/12/2017
\brief
	Every Object has body component except button and HUD.
This body class includes information that is used in calculating physics.
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
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
	//restitution = 0.2f;
	m_force = glm::vec3(0);

	Jump = false;
	if (pm_invmass == 0)
		pm_mass = 0;
	else
		pm_mass = 1 / pm_invmass;
	GroundType = Grounded::Ground;
	//Jump = true;
	//gravityOn = true;
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

GravityBox::GravityBox() : Body(glm::vec3(0), 1)
{
	GravityBoxBody.push_back(this);
	IsColliding = false;
	IsTriggered = false;
}

GravityBox::~GravityBox()
{
}

void GravityBox::PlayerIsColliding()
{
	//this->GetOwner()->GetComponent<Trigger>()->
	if (PHYSICS->RectvsRectCollisionCheck(m_pTransform, FACTORY->GetPlayer()->GetComponent<Transform>()))
	{
		IsColliding = true;
	}
	else
		IsColliding = false;
}

bool GravityBox::IsBoxTriggered()
{
	if (FACTORY->GetPlayer()->GetComponent<Body>()->GroundType != Grounded::Ground)
		return false;

	Pair ij(this, FACTORY->GetPlayer()->GetComponent<Body>());

	glm::vec3 asize = glm::vec3(ij.m_lhs->m_pTransform->GetScale());
	glm::vec3   apos = glm::vec3(ij.m_lhs->m_pTransform->GetPosition());
	glm::vec3 bsize = glm::vec3(ij.m_rhs->m_pTransform->GetScale());
	glm::vec3   bpos = glm::vec3(ij.m_rhs->m_pTransform->GetPosition());
	glm::vec3 atl, abr, btl, bbr;

	atl = glm::vec3(apos.x - (asize.x * .5f), apos.y + (asize.y * .5f), 0.f);
	abr = glm::vec3(apos.x + (asize.x * .5f), apos.y - (asize.y * .5f), 0.f);
	btl = glm::vec3(bpos.x - (bsize.x * .5f), bpos.y + (bsize.y * .5f), 0.f);
	bbr = glm::vec3(bpos.x + (bsize.x * .5f), bpos.y - (bsize.y * .5f), 0.f);

	// Vector from A to B
	glm::vec3 normal = ij.m_rhs->m_pTransform->position - ij.m_lhs->m_pTransform->position;

	// Calculate half extents along x axis for each object
	float a_extent = (abr.x - atl.x) / 2;
	float b_extent = (bbr.x - btl.x) / 2;

	float x_overlap = a_extent + b_extent - std::abs(normal.x);

	if (x_overlap > 0)
	{
		a_extent = (atl.y - abr.y) / 2;
		b_extent = (btl.y - bbr.y) / 2;

		float y_overlap = a_extent + b_extent - std::abs(normal.y);

		if (y_overlap > 0)
		{
			// Find out which axis is axis of least penetration
			if (x_overlap < y_overlap)
			{
				// Point towards B knowing that n points from A to B
				if (normal.x < 0)
					ij.normal = glm::vec3(-1, 0, 0);
				else
					ij.normal = glm::vec3(1, 0, 0);
			}
			else
			{
				// Point toward B knowing that n points from A to B
				if (normal.y < 0)
					ij.normal = glm::vec3(0, -1, 0);
				else
					ij.normal = glm::vec3(0, 1, 0);
			}
		}
	}
	if (IsColliding)
	{
		switch (PHYSICS->GravityType)
		{
		case Gravity::y_Minus:
			if (ij.normal == glm::vec3(0, 1, 0))
				return true;
			break;
		case Gravity::x_Minus:
			if (ij.normal == glm::vec3(1, 0, 0))
				return true;
			break;
		case Gravity::y_Plus:
			if (ij.normal == glm::vec3(0, -1, 0))
				return true;
			break;
		case Gravity::x_Plus:
			if (ij.normal == glm::vec3(-1, 0, 0))
				return true;
			break;
		}
	}

	return false;
}

void GravityBox::Initialize()
{
	IsColliding = false;
	IsTriggered = false;
}
