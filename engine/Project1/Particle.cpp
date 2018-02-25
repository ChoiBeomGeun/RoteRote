#include "particle.h"
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
		//GRAPHICS->EmitterList.erase(std::find(GRAPHICS->EmitterList.begin(), GRAPHICS->EmitterList.end(), this));
		/*delete []pParticles;
		pParticles = NULL;*/
	}
	void Emitter::Initialize()
	{
		
	}

	void Emitter::ParticleInit(int maxCount, float lifetime)
	{
		//pParticles = new Particle[maxCount];
		//
		//for (int i = 0; i < maxCount; ++i)
		//{
		//	pParticles[i].pos = FACTORY->GetPlayer()->GetComponent<Transform>()->position;// PARTICLEMANAGER->GetEmitters()[PARTICLEMANAGER->GetEmitters()->emitterID].pos;
		//	pParticles[i].color = 255;
		//	pParticles[i].scale = 20.f;
		//	pParticles[i].vel = PARTICLEMANAGER->GetEmitters()[i].vel;
		//}
		pos = FACTORY->GetPlayer()->GetComponent<Transform>()->position;
		vel = glm::vec3(0.0f);
		size = maxCount;
		capacity = maxCount;
		emitterID = PARTICLEMANAGER->GetEmitters()->emitterID;
		lifeTime = lifetime;
	}

	void Emitter::Update(float dt)
	{
		for (int i = 0; i < PARTICLEMANAGER->GetEmitters()->size; ++i)
		{

		}
		//PARTICLEMANAGER->
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