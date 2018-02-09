/**
\file        Body.h
\author      KyungOok Park
\par         email: qkrruddn6680@gmail.com
\par         course: GAM200
\par	ThumbUp Engine
\date        12/12/2017
\brief
	Body class (RigidBody)
Every Object has body component except button and HUD.
This body class includes information that is used in calculating physics.
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "Component.h"
#include "GameLogic.h"
//#include "vector3.h"
#include "Physics.h"
#include "Transform.h"
#include "Type.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace TE {

	class Body : public Component
	{
	public:
		Body(glm::vec3 vel, float invmass);
		~Body();

		virtual void Initialize();

		unsigned int BodyID = 0;
		virtual void Update(float dt);
		glm::vec3 m_force;
		glm::vec3 m_gravity;
		Transform * m_pTransform;
		bool ToDelete = false;
		friend class Physics;

		Grounded GroundType;

		bool Jump;

		Body() : Component(CT_BODY) {};

		void SetRestitution(float r);

		float pm_invmass;
		float pm_mass;
		bool gravityOn;
		glm::vec3 pm_velocity;

		float restitution;
	protected:
		std::list<Body*>::iterator myBody;
	};
}