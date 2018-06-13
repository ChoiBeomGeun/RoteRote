/**
\file        Physics.h
\author      KyungOok Park
\par         email: qkrruddn6680@gmail.com
\par         course: GAM250
\par   ThumbUp Engine
\date        12/12/2017
\brief
Pair class
Pair class has two object's as a class member and their collision information.
Physics class

All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "System.h"
#include <vector>
#include "TUMath.h"
#include "Type.h"
#include <map>
#include "Transform.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/norm.hpp>
namespace TE {

	class Body;

	class Pair {
	public:

		Pair(Body*lhs, Body* rhs) :m_lhs(lhs), m_rhs(rhs) {
		}
		Body* m_lhs;
		Body* m_rhs;

		float penetration;
		glm::vec3 normal;
	private:
		Pair();
	};

	class Physics : public Systems
	{
	public:
		Physics();

		~Physics();
		void Initialize(void) override;
		void Update(float dt) override;
		//member variable ; vector of pointer to rigidbody
		std::vector<Body*> m_vecprb;
		std::map<unsigned int, Body*>  m_Body;
		std::vector<Body*> box_body;
		glm::vec3 gravity;

		Gravity GravityType;

		float gravityScale;
		void Gravity180();
		void Gravity90_to_left();
		void Gravity90_to_right();
		bool GetIsPlayerGround();
		bool IsMapEditorOn = false;

		static bool MouseVsRect(glm::vec3 mouse, Transform *Object);
		static bool RectvsRectCollisionCheck(Transform *pA, Transform *pB);
		static bool SimpleRectvsRectCollisionCheck(glm::vec3 *pApos, glm::vec3 *pAscale,glm::vec3 *pBpos,glm::vec3 *pBscale);
		std::vector<Pair> m_vecpair;
	private:
		bool AABBvsAABB(Body *pA, Body *pB, Pair *M);
		//friend void Body::Body(Vector3 vel,float invmass);
		//ExplictEuler (Forward Euler ) Integator
		void ExplictEulerIntegrator(float dt);
		void BroadPhase();
		void ResolveCollision(Pair *M);
		bool CircleCircleCollisionCheck(Body *pA, Body *pB, Pair *M);
		void KinematicBoxCollision(float &rhs_invmass, float &lhs_invmass, Pair *M);
		void PositionalCorrection(Pair *M);
		void Box_velocity_toZero();

		void PlayerGroundType(glm::vec3 normal, Body *pA, Body *pB);

		

		bool IsPlayerGround;

	};




	extern Physics * PHYSICS;


}