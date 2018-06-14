/******************************************************************************/
/*!
\file	Particles.cpp
\author	HyunJun Yoo
\par	email: hyunjun306@gmail.com
\par	Class:GAM250
\par	ThumbUp Engine
\date	06/11/2018

Particles are created here.
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "Particles.h"
#include "TUMath.h"
#include "glm\glm.hpp"
#include "SOIL.h"
#include <cmath> /*atan2*/
#include <cstdio> /*FILE* fopen, fclose fscanf*/
#include "Graphics.h"
#include "ParticleManager.h"


namespace TE {
	Emitter::Emitter() : Component(ComponentType::CT_EMITTER), pParticles(nullptr)
	{
		pos = glm::vec3(0);
		vel = glm::vec3(0);
		type;
		m_textureID = 0;
		size = 0;
		capacity = 0;
		emitterID = 0;
		lifeTime = 0;
		explifeTime = 0;
		isAdditive = true;
		isOn = false;
		m_particlePath = "";
	}
	Emitter::~Emitter()
	{
		for(auto p : PARTICLEMANAGER->m_EmitterList)
		{
			delete[]p->pParticles;
			p->pParticles = NULL;
		}
		PARTICLEMANAGER->m_EmitterList.erase(std::find(PARTICLEMANAGER->m_EmitterList.begin(), PARTICLEMANAGER->m_EmitterList.end(), this));
	}
	void Emitter::Initialize()
	{

	}

	void Emitter::CreateParticle()
	{
		vel = glm::vec3(0.0f);
		
		if (pParticles == NULL)
		{
			pParticles = new Particle[capacity];
			pParticles->angle = 0;
			pParticles->lifetime = 0;
		}
		else
			return;
		for (int i = 0; i < capacity; ++i)
		{
			pParticles[i].color[0] = 255;
			pParticles[i].color[1] = 255;
			pParticles[i].color[2] = 255;
			pParticles[i].color[3] = 255;
			pParticles[i].scale = glm::vec3(0.0);
			pParticles[i].vel = glm::vec3(0);
		}
	}


	void Emitter::SetTexture(int textureID)
	{
		m_textureID = textureID;
	}

	

	void Emitter::SetEmitter(glm::vec3 EmitterPos, glm::vec3 EmitterVel, int pSize, int pCapacity, float plifeTime, EmitterType ptype, std::string particlePath = "PlayerTrail.json")
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
		particle.scale -= particle.scale *deltaTime;
	}

	void trailParticleUpdate(Particle & particle, float deltaTime)
	{
		particle.pos.z = 0.f;
		particle.scale -= particle.scale *deltaTime;
	}

}