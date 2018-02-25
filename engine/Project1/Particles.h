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
		float scale;  //!< scale of Particle
		Color color; //!< color of Particle
	};

	enum EmitterType //!< enum for Type
	{
		ET_EXPLOSION, //!< type of explosion
		ET_TRAIL      //!<  type of trail
	};

	class Emitter : public Component//!< struct of Emitter
	{
	public:
		Emitter();
		~Emitter();
		virtual void Initialize() override;
		void ParticleInit(int maxCount, float lifetime);
		void Update(float dt);
		void setTextureID(unsigned int textureID);

		Particle*   pParticles; //!< pointer of Paritcles in this Emitter
		EmitterType type; //!< type of Emitter
		glm::vec3      pos;  //!< position of Emitter
		glm::vec3      vel;  //!< velocity of Emitter
		int         m_textureID; //!< textureID of Emitter
		int         size;     //!< size of Emitter
		int         capacity; //!< max amount for this Emitter
		int         emitterID; //!< ID of this Emitter
		float       lifeTime;  //!< lifeTime of this Emitter
	};


	void defaultParticleUpdate(Particle& particle, float deltaTime);

	void trailParticleUpdate(Particle& particle, float deltaTime);


}

#endif // ! Particle_H




