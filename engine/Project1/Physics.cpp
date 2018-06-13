/**
\file        Physics.cpp
\author      KyungOok Park
\par         email: qkrruddn6680@gmail.com
\par         course: GAM250
\par   ThumbUp Engine
\date        12/12/2017
\brief
This state checks collision between every objects and resolve it through BroadPhase.
Also by Explicit Euler integrator function loop, every objects move by their velocity.

All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Physics.h"
#include "DebugUtil.h"
#include "Body.h"

#include "Transform.h"
#include "Sprite.h"
#include "Application.h"
#include "Factory.h"
#include "StateManager.h"
#include "../../example/examples/PlayerController.h"

namespace TE {
	Physics * PHYSICS = nullptr;
}


using namespace TE;

Physics::Physics() {

	DEBUG_ASSERT(PHYSICS != nullptr, "Physics System is aleardy existing");
	PHYSICS = this;

}

Physics::~Physics()
{
	PHYSICS->m_vecprb.clear();
	PHYSICS->box_body.clear();
	PHYSICS->m_Body.clear();
}

void Physics::Initialize() {
	gravityScale = -20.f;
	GravityType = Gravity::y_Minus;
	gravity = glm::vec3(0, gravityScale, 0);
	IsPlayerGround = false;
}

void Physics::Update(float dt) {




	if ((!STATEMANAGER->b_IsPauseOn) && (!STATEMANAGER->b_IsRot180) && (!STATEMANAGER->b_IsRot90) && (!PHYSICS->IsMapEditorOn) && !APP->b_Win)
	{
		ExplictEulerIntegrator(dt);
		BroadPhase();
	}
	else if (STATEMANAGER->b_IsRot180 || STATEMANAGER->b_IsRot90)
		Box_velocity_toZero();

	if (STATEMANAGER->b_IsGravityChanged)
		Box_velocity_toZero();




}

void Physics::ExplictEulerIntegrator(float dt) {

	Transform * pTr = nullptr;
	//Current frame Position and next Frame Position
	glm::vec3 curfp, nexfp;

	box_body.clear();

	for (std::map<unsigned int, Body*>::iterator i = m_Body.begin();
		i != m_Body.end(); ++i) {




		if (STATEMANAGER->b_IsReplay || STATEMANAGER->b_IsAutoplaying)
		{
			if ((*i).second->GetOwner()->HasComponent<PlayerController>())
				continue;

		}
		//Explict Euler method
		//f(t+dt) = f(t) + f'(t)dt
		//Next Frame's Position = Current Frame's position + (vector *dt);

		pTr = (*i).second->m_pTransform;
		curfp = pTr->GetPosition();
		pTr->SetPosition(curfp + (*i).second->pm_velocity*dt);
		if ((*i).second->GetOwner()->objectstyle == Objectstyle::Hazard || (*i).second->GetOwner()->objectstyle == Objectstyle::Trigger90Right||
			(*i).second->GetOwner()->objectstyle == Objectstyle::Wall || (*i).second->GetOwner()->objectstyle == Objectstyle::Trigger90 || (*i).second->GetOwner()->objectstyle == Objectstyle::Trigger180 || (*i).second->GetOwner()->objectstyle == Objectstyle::AttachWall)
			continue;

		if ((*i).second->GetOwner()->objectstyle == Objectstyle::Box || (*i).second->GetOwner()->objectstyle == Objectstyle::AttachBox)
			box_body.push_back((*i).second);

		if (!(*i).second->gravityOn)
		{
			(*i).second->pm_velocity += gravity * 100.f * dt;
		}
		else if (!(*i).second->pOwner->objectstyle == Player)
			(*i).second->pm_velocity += (gravity + (*i).second->m_force * (*i).second->pm_invmass)*dt;

		if (gravity.x != 0.f)
			(*i).second->pm_velocity.y *= 0.99f * dt * 50.f;

		if (gravity.y != 0.f)
			(*i).second->pm_velocity.x *= 0.99f * dt * 50.f;
	}

}

void Physics::BroadPhase() {
	//k; number of objects
	//Big O Notation : O (n^2 / 2 )

	bool Attached = false;

	if (FACTORY->GetPlayer())
	{
		FACTORY->GetPlayer()->GetComponent<Body>()->Jump = false;
	}
	for (std::map<unsigned int, Body*>::iterator i = m_Body.begin();
		i != m_Body.end(); ++i) {

		if (i->second->GetOwner()->objectstyle == Objectstyle::Hazard)
			continue;

		for (std::map <unsigned int, Body*>::iterator j = std::next(i); j != m_Body.end(); j++) {
			//Check collsision between tow AABB objects,
			//put the pair into the container

			if (j->second->GetOwner()->objectstyle == Objectstyle::Hazard)
				continue;

			Pair ij(i->second, j->second);

			if (i->second->GetOwner()->objectstyle == Objectstyle::AttachWall && j->second->GetOwner()->objectstyle == Objectstyle::Player) {
				if (AABBvsAABB(i->second, j->second, &ij))
					Attached = true;
			}
			else if (i->second->GetOwner()->objectstyle == Objectstyle::Player && j->second->GetOwner()->objectstyle == Objectstyle::AttachWall) {
				if (AABBvsAABB(i->second, j->second, &ij))
					Attached = true;
			}

			else if(i->second->GetOwner()->objectstyle == Objectstyle::AttachBox && j->second->GetOwner()->objectstyle == Objectstyle::Player) {
				if (AABBvsAABB(i->second, j->second, &ij))
					Attached = true;
			}
			else if (i->second->GetOwner()->objectstyle == Objectstyle::Player && j->second->GetOwner()->objectstyle == Objectstyle::AttachBox) {
				if (AABBvsAABB(i->second, j->second, &ij))
					Attached = true;
			}

			if (i->second->pm_invmass == 0 && j->second->pm_invmass == 0)
				continue;
			if (i->second->GetOwner()->objectstyle == Objectstyle::Trigger180 || i->second->GetOwner()->objectstyle == Objectstyle::Trigger90 || (*i).second->GetOwner()->objectstyle == Objectstyle::Trigger90Right)
				continue;
			if (j->second->GetOwner()->objectstyle == Objectstyle::Trigger180 || j->second->GetOwner()->objectstyle == Objectstyle::Trigger90|| (*j).second->GetOwner()->objectstyle == Objectstyle::Trigger90Right)
				continue;




			if (AABBvsAABB(i->second, j->second, &ij))
			{
				PositionalCorrection(&ij);
				ResolveCollision(&ij);
			}
		}
	}
	if (FACTORY->GetPlayer()) {
		if (!(FACTORY->GetPlayer()->GetComponent<Body>()->Jump))
			FACTORY->GetPlayer()->GetComponent<Body>()->GroundType = Grounded::Air;

		if (Attached)
			FACTORY->GetPlayer()->GetComponent<PlayerController>()->IsAttachable = true;
		else
			FACTORY->GetPlayer()->GetComponent<PlayerController>()->IsAttachable = false;
	}
	IsPlayerGround = false;
}

void Physics::ResolveCollision(Pair *M)
{
	// Calculate relative velocity
	glm::vec3 rv = M->m_rhs->pm_velocity - M->m_lhs->pm_velocity;

	float lhs_invmass = M->m_lhs->pm_invmass;
	float rhs_invmass = M->m_rhs->pm_invmass;
	float lhs_mass = M->m_lhs->pm_mass;
	float rhs_mass = M->m_rhs->pm_mass;

	KinematicBoxCollision(rhs_invmass, lhs_invmass, M);

	if (lhs_invmass == 0)
		lhs_mass = 0;
	else
		lhs_mass = 1 / lhs_invmass;

	if (rhs_invmass == 0)
		rhs_mass = 0;
	else
		rhs_mass = 1 / rhs_invmass;


	// Calculate relative velocity in terms of the normal direction
	float velAlongNormal = glm::dot(rv, M->normal);
	// Do not resolve if velocities are separating
	if (velAlongNormal > 0)
		return;

	// Calculate restitution
	float e = TUMath::Min(M->m_lhs->restitution, M->m_rhs->restitution);

	// Calculate impulse scalar
	float j = -(1 + e) * velAlongNormal;

	j /= rhs_invmass + lhs_invmass;

	// Apply impulse
	glm::vec3 impulse = M->normal * j;

	float mass_sum = rhs_mass + lhs_mass;
	float ratio = lhs_mass / mass_sum;

	M->m_lhs->pm_velocity -= ratio * impulse;

	ratio = rhs_mass / mass_sum;

	M->m_rhs->pm_velocity += ratio * impulse;
}

bool Physics::CircleCircleCollisionCheck(Body * pA, Body * pB, Pair *M)
{
	// Setup a couple pointers to each object
	M->m_lhs = pA;
	M->m_rhs = pB;

	// Vector from A to B
	glm::vec3 normal = pB->m_pTransform->position - pA->m_pTransform->position;

	float A_radius = pA->m_pTransform->scale.x / 2;
	float B_radius = pB->m_pTransform->scale.x / 2;

	float r = A_radius + B_radius;
	float sq_r = r * r;

	if (normal.length() > sq_r)
		return false;

	// Circles have collided, now compute manifold
	float d = glm::length(normal); // perform actual sqrt

	// If distance between circles is not zero
	if (d != 0)
	{
		// Distance is difference between radius and distance
		M->penetration = r - d;

		// Utilize our d since we performed sqrt on it already within Length( )
		// Points from A to B, and is a unit vector
		M->normal = normal / d;
		return true;
	}

	// Circles are on same position
	else
	{
		// Choose random (but consistent) values
		M->penetration = A_radius;
		M->normal = glm::vec3(1, 0, 0);
		return true;
	}
}

void Physics::KinematicBoxCollision(float &rhs_invmass, float &lhs_invmass, Pair *M)
{
	if ((M->m_lhs->GetOwner()->objectstyle == Objectstyle::Box || M->m_lhs->GetOwner()->objectstyle == Objectstyle::AttachBox) && (M->m_rhs->GetOwner()->objectstyle == Objectstyle::Box || M->m_rhs->GetOwner()->objectstyle == Objectstyle::AttachBox))
	{
		if (gravity.y < 0)
		{
			if (M->m_rhs->m_pTransform->position.y < M->m_lhs->m_pTransform->position.y)
				rhs_invmass = 0;
			else if (M->m_rhs->m_pTransform->position.y > M->m_lhs->m_pTransform->position.y)
				lhs_invmass = 0;
		}
		else if (gravity.y > 0)
		{
			if (M->m_rhs->m_pTransform->position.y < M->m_lhs->m_pTransform->position.y)
				lhs_invmass = 0;
			else if (M->m_rhs->m_pTransform->position.y > M->m_lhs->m_pTransform->position.y)
				rhs_invmass = 0;
		}
		else if (gravity.x < 0)
		{
			if (M->m_rhs->m_pTransform->position.x < M->m_lhs->m_pTransform->position.x)
				rhs_invmass = 0;
			else if (M->m_rhs->m_pTransform->position.x > M->m_lhs->m_pTransform->position.x)
				lhs_invmass = 0;
		}
		else if (gravity.x > 0)
		{
			if (M->m_rhs->m_pTransform->position.x < M->m_lhs->m_pTransform->position.x)
				lhs_invmass = 0;
			else if (M->m_rhs->m_pTransform->position.x > M->m_lhs->m_pTransform->position.x)
				rhs_invmass = 0;
		}
	}
	else if (M->m_lhs->GetOwner()->objectstyle == Objectstyle::Player && (M->m_rhs->GetOwner()->objectstyle == Objectstyle::Box || M->m_rhs->GetOwner()->objectstyle == Objectstyle::AttachBox))
		rhs_invmass = 0;
	else if ((M->m_lhs->GetOwner()->objectstyle == Objectstyle::Box || M->m_lhs->GetOwner()->objectstyle == Objectstyle::AttachBox) && M->m_rhs->GetOwner()->objectstyle == Objectstyle::Player)
		lhs_invmass = 0;
}

bool Physics::MouseVsRect(glm::vec3 mouse, Transform * Object)
{
	if ((Object->position.x - Object->scale.x * 0.5f <= mouse.x) && (Object->position.x + Object->scale.x >= mouse.x))
		if ((Object->position.y - Object->scale.y * 0.f <= mouse.y) && (Object->position.y + Object->scale.y >= mouse.y))
			return true;
	return false;
}

bool Physics::RectvsRectCollisionCheck(Transform * pA, Transform * pB)
{

	if (pA && pB)
	{
		glm::vec3 asize = pA->GetScale();
		glm::vec3   apos = pA->GetPosition();
		glm::vec3 bsize = pB->GetScale();
		glm::vec3   bpos = pB->GetPosition();
		glm::vec3 atl, abr, btl, bbr;

		atl = glm::vec3(apos.x - (asize.x * .5f), apos.y + (asize.y * .5f), 0.f);
		abr = glm::vec3(apos.x + (asize.x * .5f), apos.y - (asize.y * .5f), 0.f);
		btl = glm::vec3(bpos.x - (bsize.x * .5f), bpos.y + (bsize.y * .5f), 0.f);
		bbr = glm::vec3(bpos.x + (bsize.x * .5f), bpos.y - (bsize.y * .5f), 0.f);

		// Vector from A to B
		glm::vec3 normal = pB->GetPosition() - pA->GetPosition();
		// pB->m_pTransform->Position - pA->m_pTransform->Position;

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
					return true;
				else
					return true;
			}
		}
	}
	return false;
}
bool TE::Physics::SimpleRectvsRectCollisionCheck(glm::vec3 * pApos, glm::vec3 * pAscale, glm::vec3 * pBpos, glm::vec3 * pBscale)
{
	if (pApos && pBpos)
	{
		glm::vec3 asize = *pAscale;
		glm::vec3   apos = *pApos;
		glm::vec3 bsize = *pBscale;
		glm::vec3   bpos = *pBpos;
		glm::vec3 atl, abr, btl, bbr;

		atl = glm::vec3(apos.x - (asize.x * .5f), apos.y + (asize.y * .5f), 0.f);
		abr = glm::vec3(apos.x + (asize.x * .5f), apos.y - (asize.y * .5f), 0.f);
		btl = glm::vec3(bpos.x - (bsize.x * .5f), bpos.y + (bsize.y * .5f), 0.f);
		bbr = glm::vec3(bpos.x + (bsize.x * .5f), bpos.y - (bsize.y * .5f), 0.f);

		// Vector from A to B
		glm::vec3 normal = *pBpos - *pApos;
		// pB->m_pTransform->Position - pA->m_pTransform->Position;

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
					return true;
				else
					return true;
			}
		}
	}
	return false;
}
bool Physics::AABBvsAABB(Body * pA, Body * pB, Pair * M)
{
	M->m_lhs = pA;
	M->m_rhs = pB;

	glm::vec3 asize = glm::vec3(pA->m_pTransform->GetScale());
	glm::vec3   apos = glm::vec3(pA->m_pTransform->GetPosition());
	glm::vec3 bsize = glm::vec3(pB->m_pTransform->GetScale());
	glm::vec3   bpos = glm::vec3(pB->m_pTransform->GetPosition());
	glm::vec3 atl, abr, btl, bbr;

	atl = glm::vec3(apos.x - (asize.x * .5f), apos.y + (asize.y * .5f), 0.f);
	abr = glm::vec3(apos.x + (asize.x * .5f), apos.y - (asize.y * .5f), 0.f);
	btl = glm::vec3(bpos.x - (bsize.x * .5f), bpos.y + (bsize.y * .5f), 0.f);
	bbr = glm::vec3(bpos.x + (bsize.x * .5f), bpos.y - (bsize.y * .5f), 0.f);

	// Vector from A to B
	glm::vec3 normal = pB->m_pTransform->position - pA->m_pTransform->position;

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
					M->normal = glm::vec3(-1, 0, 0);
				else
					M->normal = glm::vec3(1, 0, 0);

				if (!IsPlayerGround)
					PlayerGroundType(M->normal, pA, pB);
				M->penetration = x_overlap;
				M->m_lhs->Jump = true;
				M->m_rhs->Jump = true;
				return true;
			}
			else
			{
				// Point toward B knowing that n points from A to B
				if (normal.y < 0)
					M->normal = glm::vec3(0, -1, 0);
				else
					M->normal = glm::vec3(0, 1, 0);

				if (!IsPlayerGround)
					PlayerGroundType(M->normal, pA, pB);
				M->penetration = y_overlap;
				M->m_lhs->Jump = true;
				M->m_rhs->Jump = true;
				return true;
			}
		}
	}

	return false;
}

void Physics::PositionalCorrection(Pair *M)
{
	const float percent = 0.4f; // usually 20% to 80%
	const float slop = 0.05f; // usually 0.01 to 0.1
	float lhs_invmass = M->m_lhs->pm_invmass;
	float rhs_invmass = M->m_rhs->pm_invmass;




	if (M->m_lhs->GetOwner()->objectstyle == Objectstyle::Box && M->m_rhs->GetOwner()->objectstyle == Objectstyle::Box
		|| (M->m_lhs->GetOwner()->objectstyle == Objectstyle::AttachBox && M->m_rhs->GetOwner()->objectstyle == Objectstyle::AttachBox))
	{
		if (gravity.y < 0)
		{
			if (M->m_rhs->m_pTransform->position.y < M->m_lhs->m_pTransform->position.y)
				rhs_invmass = 0;
			else if (M->m_rhs->m_pTransform->position.y > M->m_lhs->m_pTransform->position.y)
				lhs_invmass = 0;
		}
		else if (gravity.y > 0)
		{
			if (M->m_rhs->m_pTransform->position.y < M->m_lhs->m_pTransform->position.y)
				lhs_invmass = 0;
			else if (M->m_rhs->m_pTransform->position.y > M->m_lhs->m_pTransform->position.y)
				rhs_invmass = 0;
		}
		else if (gravity.x < 0)
		{
			if (M->m_rhs->m_pTransform->position.x < M->m_lhs->m_pTransform->position.x)
				rhs_invmass = 0;
			else if (M->m_rhs->m_pTransform->position.x > M->m_lhs->m_pTransform->position.x)
				lhs_invmass = 0;
		}
		else if (gravity.x > 0)
		{
			if (M->m_rhs->m_pTransform->position.x < M->m_lhs->m_pTransform->position.x)
				lhs_invmass = 0;
			else if (M->m_rhs->m_pTransform->position.x > M->m_lhs->m_pTransform->position.x)
				rhs_invmass = 0;
		}
	}
	else if (M->m_lhs->GetOwner()->objectstyle == Objectstyle::Player && M->m_rhs->GetOwner()->objectstyle == Objectstyle::Box
		|| (M->m_lhs->GetOwner()->objectstyle == Objectstyle::Player && M->m_rhs->GetOwner()->objectstyle == Objectstyle::AttachBox))
		rhs_invmass = 0;
	else if ((M->m_lhs->GetOwner()->objectstyle == Objectstyle::Box && M->m_rhs->GetOwner()->objectstyle == Objectstyle::Player)||
		(M->m_lhs->GetOwner()->objectstyle == Objectstyle::AttachBox && M->m_rhs->GetOwner()->objectstyle == Objectstyle::Player))
		lhs_invmass = 0;

	glm::vec3 correction = TUMath::Max(M->penetration - slop, 0.0f) / (lhs_invmass + rhs_invmass) * percent * M->normal;

	M->m_lhs->m_pTransform->position -= lhs_invmass * correction;
	M->m_rhs->m_pTransform->position += rhs_invmass * correction;

}

void Physics::Box_velocity_toZero()
{
	for (auto box_itr = box_body.begin(); box_itr != box_body.end(); ++box_itr)
		(*box_itr)->pm_velocity = glm::vec3(0);

	STATEMANAGER->b_IsGravityChanged = false;
}

void Physics::PlayerGroundType(glm::vec3 normal, Body * pA, Body * pB)
{
	if (normal == glm::vec3(-1, 0, 0))
	{
		if (GravityType == Gravity::y_Minus)
		{
			if (pB->GetOwner()->objectstyle == Player)
				pB->GroundType = Grounded::Left;
			else if (pA->GetOwner()->objectstyle == Player)
				pA->GroundType = Grounded::Right;
		}
		else if (GravityType == Gravity::x_Minus)
		{
			if (pB->GetOwner()->objectstyle == Player)
				pB->GroundType = Grounded::Air;
			else if (pA->GetOwner()->objectstyle == Player)
			{
				pA->GroundType = Grounded::Ground;
				IsPlayerGround = true;
			}
		}
		else if (GravityType == Gravity::y_Plus)
		{
			if (pB->GetOwner()->objectstyle == Player)
				pB->GroundType = Grounded::Right;
			else if (pA->GetOwner()->objectstyle == Player)
				pA->GroundType = Grounded::Left;
		}
		else if (GravityType == Gravity::x_Plus)
		{
			if (pB->GetOwner()->objectstyle == Player)
			{
				IsPlayerGround = true;
				pB->GroundType = Grounded::Ground;
			}
			else if (pA->GetOwner()->objectstyle == Player)
				pA->GroundType = Grounded::Air;
		}
	}
	else if (normal == glm::vec3(1, 0, 0))
	{
		if (GravityType == Gravity::y_Minus)
		{
			if (pB->GetOwner()->objectstyle == Player)
				pB->GroundType = Grounded::Right;
			else if (pA->GetOwner()->objectstyle == Player)
				pA->GroundType = Grounded::Left;
		}
		else if (GravityType == Gravity::x_Minus)
		{
			if (pB->GetOwner()->objectstyle == Player)
			{
				IsPlayerGround = true;
				pB->GroundType = Grounded::Ground;
			}
			else if (pA->GetOwner()->objectstyle == Player)
				pA->GroundType = Grounded::Air;
		}
		else if (GravityType == Gravity::y_Plus)
		{
			if (pB->GetOwner()->objectstyle == Player)
				pB->GroundType = Grounded::Left;
			else if (pA->GetOwner()->objectstyle == Player)
				pA->GroundType = Grounded::Right;
		}
		else if (GravityType == Gravity::x_Plus)
		{
			if (pB->GetOwner()->objectstyle == Player)
				pB->GroundType = Grounded::Air;
			else if (pA->GetOwner()->objectstyle == Player)
			{
				pA->GroundType = Grounded::Ground;
				IsPlayerGround = true;
			}
		}
	}
	else if (normal == glm::vec3(0, -1, 0))
	{
		if (GravityType == Gravity::y_Minus)
		{
			if (pB->GetOwner()->objectstyle == Player)
				pB->GroundType = Grounded::Air;
			else if (pA->GetOwner()->objectstyle == Player)
			{
				pA->GroundType = Grounded::Ground;
				IsPlayerGround = true;
			}
		}
		else if (GravityType == Gravity::x_Minus)
		{
			if (pB->GetOwner()->objectstyle == Player)
				pB->GroundType = Grounded::Right;
			else if (pA->GetOwner()->objectstyle == Player)
				pA->GroundType = Grounded::Left;
		}
		else if (GravityType == Gravity::y_Plus)
		{
			if (pB->GetOwner()->objectstyle == Player)
			{
				pB->GroundType = Grounded::Ground;
				IsPlayerGround = true;
			}
			else if (pA->GetOwner()->objectstyle == Player)
				pA->GroundType = Grounded::Air;
		}
		else if (GravityType == Gravity::x_Plus)
		{
			if (pB->GetOwner()->objectstyle == Player)
				pB->GroundType = Grounded::Left;
			else if (pA->GetOwner()->objectstyle == Player)
				pA->GroundType = Grounded::Right;
		}
	}
	else if (normal == glm::vec3(0, 1, 0))
	{
		if (GravityType == Gravity::y_Minus)
		{
			if (pB->GetOwner()->objectstyle == Player)
			{
				pB->GroundType = Grounded::Ground;
				IsPlayerGround = true;
			}
			else if (pA->GetOwner()->objectstyle == Player)
				pA->GroundType = Grounded::Air;
		}
		else if (GravityType == Gravity::x_Minus)
		{
			if (pB->GetOwner()->objectstyle == Player)
				pB->GroundType = Grounded::Left;
			else if (pA->GetOwner()->objectstyle == Player)
				pA->GroundType = Grounded::Right;
		}
		else if (GravityType == Gravity::y_Plus)
		{
			if (pB->GetOwner()->objectstyle == Player)
				pB->GroundType = Grounded::Air;
			else if (pA->GetOwner()->objectstyle == Player)
			{
				pA->GroundType = Grounded::Ground;
				IsPlayerGround = true;
			}
		}
		else if (GravityType == Gravity::x_Plus)
		{
			if (pB->GetOwner()->objectstyle == Player)
				pB->GroundType = Grounded::Right;
			else if (pA->GetOwner()->objectstyle == Player)
				pA->GroundType = Grounded::Left;
		}
	}
}

void Physics::Gravity180()
{
	gravity *= -1.f;
	if (GravityType == Gravity::x_Minus)
		GravityType = Gravity::x_Plus;
	else if (GravityType == Gravity::y_Plus)
		GravityType = Gravity::y_Minus;
	else if (GravityType == Gravity::x_Plus)
		GravityType = Gravity::x_Minus;
	else if (GravityType == Gravity::y_Minus)
		GravityType = Gravity::y_Plus;
}

void Physics::Gravity90_to_left()
{
	if (GravityType == Gravity::x_Plus)
	{
		gravity = glm::vec3(0, gravityScale, 0);
		GravityType = Gravity::y_Minus;
	}
	else if (GravityType == Gravity::y_Plus)
	{
		gravity = glm::vec3(-gravityScale, 0, 0);
		GravityType = Gravity::x_Plus;
	}
	else if (GravityType == Gravity::x_Minus)
	{
		gravity = glm::vec3(0, -gravityScale, 0);
		GravityType = Gravity::y_Plus;
	}
	else if (GravityType == Gravity::y_Minus)
	{
		gravity = glm::vec3(gravityScale, 0, 0);
		GravityType = Gravity::x_Minus;
	}
}

void Physics::Gravity90_to_right()
{
	if (GravityType == Gravity::x_Plus)
	{
		gravity = glm::vec3(0, -gravityScale, 0);
		GravityType = Gravity::y_Plus;
	}
	else if (GravityType == Gravity::y_Plus)
	{
		gravity = glm::vec3(gravityScale, 0, 0);
		GravityType = Gravity::x_Minus;
	}
	else if (GravityType == Gravity::x_Minus)
	{
		gravity = glm::vec3(0, gravityScale, 0);
		GravityType = Gravity::y_Minus;
	}
	else if (GravityType == Gravity::y_Minus)
	{
		gravity = glm::vec3(-gravityScale, 0, 0);
		GravityType = Gravity::x_Plus;
	}
}

bool Physics::GetIsPlayerGround()
{
	return IsPlayerGround;
}