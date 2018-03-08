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
		//PARTICLEMANAGER->m_EmitterList.push_back(this);
	}
	Emitter::~Emitter()
	{
		//std::cout << "Emitter DEEEEEELEETTTE" << std::endl;
		delete pParticles;
		pParticles = NULL;
		PARTICLEMANAGER->m_EmitterList.erase(std::find(PARTICLEMANAGER->m_EmitterList.begin(), PARTICLEMANAGER->m_EmitterList.end(), this));

		//GRAPHICS->EmitterList.erase(std::find(GRAPHICS->EmitterList.begin(), GRAPHICS->EmitterList.end(), this));

	}
	void Emitter::Initialize()
	{
		/*auto iter = PARTICLEMANAGER->m_EmitterList.begin();
		for (; iter!= PARTICLEMANAGER->m_EmitterList.end(); ++iter)
		{*/

		if (PARTICLEMANAGER->m_EmitterList.empty())
		{
			pParticles = NULL;
			type = ET_EXPLOSION;
			pos = glm::vec3(0);
			vel = glm::vec3(0);
			m_textureID = 0;
			size = 0;
			capacity = 0;
			emitterID = 0;
			lifeTime = 0;
		}
		else
			return;

	}

	void Emitter::CreateParticle()
	{
		vel = glm::vec3(0.0f);
		if (pParticles == NULL)
			pParticles = new Particle[capacity];
		else
			return;
		for (int i = 0; i < capacity; ++i)
		{
			pParticles[i].color[0] = 255;
			pParticles[i].color[1] = 255;
			pParticles[i].color[2] = 255;
			pParticles[i].color[3] = 255;
			pParticles[i].scale = 0.0f;
			pParticles[i].scale = 20;
			pParticles[i].vel = glm::vec3(0);
		}
	}

	void Emitter::Update(float dt)
	{
		for (auto p : PARTICLEMANAGER->m_EmitterList)
		{
			p->pos = this->GetOwner()->GetComponent<Transform>()->position;
		}
	}

	void Emitter::SetTexture(int textureID)
	{
		m_textureID = textureID;
	}

	void Emitter::SetEmitter(glm::vec3 EmitterPos, glm::vec3 EmitterVel, int pSize, int pCapacity, int plifeTime, EmitterType ptype)
	{
		pos = EmitterPos;
		vel = EmitterVel;
		size = pSize;
		capacity = pCapacity;
		lifeTime = plifeTime;
		type = ptype;
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