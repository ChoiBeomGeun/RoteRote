#include "Particles.h"
#include "TUMath.h"

#include "glm\glm.hpp"
#include "SOIL.h"
#include <cmath> /*atan2*/
#include <cstdio> /*FILE* fopen, fclose fscanf*/
#include "Graphics.h"
#include "ParticleManager.h"

namespace TE {
	Emitter::Emitter() : Component(ComponentType::CT_EMITTER)
	{
		GRAPHICS->EmitterList.push_back(this);
	}
	Emitter::~Emitter()
	{
		std::cout << "Emitter DEEEEEELEETTTE" << std::endl;
		/*delete [] pParticles;
		pParticles = NULL;*/
	}
	void Emitter::Initialize()
	{

	}

	void Emitter::CreateParticle(int maxCount, float lifetime)
	{
 		vel = glm::vec3(0.0f);
		size = maxCount;
		capacity = maxCount;
		emitterID = PARTICLEMANAGER->GetEmitters()->emitterID;
		lifeTime = lifetime;
		pParticles = new Particle[maxCount];
		for (int i = 0; i < maxCount; ++i)
		{
			pParticles[i].color.r = 255;
			pParticles[i].color.g = 255;
			pParticles[i].color.b = 255;
			pParticles[i].color.a = 255;
			pParticles[i].pos = PARTICLEMANAGER->GetEmitters()->pos;
			pParticles[i].scale = 20;
			pParticles[i].vel = glm::vec3(0);
		}
	}

	void Emitter::Update(float dt)
	{
		dt;
		for (int i = 0; i < PARTICLEMANAGER->GetEmitters()->size; ++i)
		{
			
		}
		
	}

	void Emitter::setTextureID(unsigned int textureID)
	{
		m_textureID = textureID;
	}


	void defaultParticleUpdate(Particle & particle, float deltaTime)
	{
		//particle.position += particle.velocity * deltaTime;
		//particle.position.z = 0.f;
		particle.scale -= particle.scale *deltaTime;
	}

	void trailParticleUpdate(Particle & particle, float deltaTime)
	{
		//particle.position += particle.velocity * deltaTime;
		particle.pos.z = 0.f;
		particle.scale -= particle.scale *deltaTime;
		//particle.color = 255;
	}


}