/******************************************************************************/
/*!
\file	Particles.h
\author	HyunJun Yoo
\par	email: hyunjun306@gmail.com
\par	Class:GAM250
\par	ThumbUp Engine
\date	06/11/2018

Particle interface
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef  Particle_H
#define  Particle_H

#include "glm\glm.hpp"
#include "Component.h"
#include "ComponentType.h"
#include <string>
#include "Transform.h"
#include <glew.h>
#include "Transform.h"
#include <functional>
#include "Vertex.h"

namespace TE {


	struct Particle //!< struct of Particle
	{
		glm::vec3 pos; //!< position of particle
		glm::vec3 vel; //!< velocity of Particle
		float lifetime; //!< lifetime of Particle
		glm::vec3 scale;  //!< scale of Particle
		float color[4]; //!< color of Particle
		float angle; //!< rotation of particle
	};

	enum EmitterType //!< enum for Type
	{
		ET_EXPLOSION, //!< type of explosion
		ET_TRAIL,      //!<  type of trail
		ET_SELECTION, //!< type of Background
		ET_LASER, //!< Hazard
		ET_END

	};

	class Emitter : public Component //!< struct of Emitter
	{
	public:
		Emitter();
		~Emitter();
		virtual void Initialize() override;
		void CreateParticle();
		void SetEmitter(glm::vec3 EmitterPos, glm::vec3 EmitterVel, int pSize, int pCapacity, float plifeTime, EmitterType ptype, std::string particlePath);
		void SetTexture(int textureID);
		Particle*   pParticles; //!< pointer of Paritcles in this Emitter
		EmitterType type; //!< type of Emitter
		glm::vec3      pos;  //!< position of Emitter
		glm::vec3      vel;  //!< velocity of Emitter
		int         m_textureID; //!< textureID of Emitter
		int         size;     //!< size of Emitter
		int         capacity; //!< max amount for this Emitter
		int         emitterID; //!< ID of this Emitter
		float       lifeTime;  //!< lifeTime of this Emitter
		float		explifeTime;
		bool		isAdditive; // !< addtive or bleneded
		bool		isOn;
		std::string m_particlePath;
	};
	void defaultParticleUpdate(Particle& particle, float deltaTime);
	void trailParticleUpdate(Particle& particle, float deltaTime);
}

#endif // ! Particle_H




