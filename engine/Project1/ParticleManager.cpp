/******************************************************************************/
/*!
\file   ParticleManager.cpp
\author HyunJun Yoo
\par    email: hyunjun306\@gmail.com
\brief
This file has the implementation of the functions for Assignment3

*/
/******************************************************************************/
#include "ParticleManager.h"

#include "glm\glm.hpp"
#include "DebugUtil.h"
#include "TUMath.h"

#include <cmath> /*atan2*/
#include <cstdio> /*FILE* fopen, fclose fscanf*/
#include <iostream>

namespace TE {
	ParticleManager * PARTICLEMANAGER = nullptr;

	namespace //!< namespace for values of this file
	{
		/*Color Info*/
		const int MAX_COLORS = 2; //!< max value of color
		const int RED = 0; //!< RED color
		const int YELLOW = 1; //!< YELLOW color
		unsigned  g_colors[MAX_COLORS] = { 0xFF0000FF, 0xFF00FFFF };
		//!< color array with unsigned
	}//end unnamed namespace
	 // /******************************************************************************/
	 // /*!

	 // function which load file for this function

	 // \param pFileName
	 // name of file to load

	 // */
	 // /******************************************************************************/
	 //void ParticleManager::LoadFile(const char* pFileName)
	 //{
	 //	FILE* pFile = 0;
	 //	fopen_s(&pFile, pFileName, "rt");
	 //	if (!pFile)
	 //		return;

	 //	fscanf_s(pFile, "%f", &m_minExpVel);
	 //	fscanf_s(pFile, "%f", &m_maxExpVel);
	 //	fscanf_s(pFile, "%f", &m_minExpScale);
	 //	fscanf_s(pFile, "%f", &m_maxExpScale);

	 //	fscanf_s(pFile, "%f", &m_minTrailVel);
	 //	fscanf_s(pFile, "%f", &m_maxTrailVel);
	 //	fscanf_s(pFile, "%f", &m_minTrailScale);
	 //	fscanf_s(pFile, "%f", &m_maxTrailScale);

	 //	fscanf_s(pFile, "%f", &m_scaleFactor);
	 //	fscanf_s(pFile, "%f", &m_expLife);

	 //	fscanf_s(pFile, "%x", &g_colors[0]);
	 //	fscanf_s(pFile, "%x", &g_colors[1]);

	 //	fclose(pFile);
	 //}


	ParticleManager::ParticleManager()
	{
		DEBUG_ASSERT(PARTICLEMANAGER != nullptr, "There should be only one ParticleManager");
		PARTICLEMANAGER = this;

	}
	/******************************************************************************/
	/*!

	function which load file for this function

	\param maxCount
	Max count of emitter. Set new emitter with this.

	*/
	/******************************************************************************/
	//ParticleManager::ParticleManager(int maxCount)
	//{
	//	
	//	

	//	// Load file with ParticleProperties.txt

	//	//LoadFile("ParticleProperties.txt");
	//}
	/******************************************************************************/
	/*!

	Deletes all pParticles from the emitters then deletes the emitter array.

	\attention
	don't forget delete consistently.

	*/
	/******************************************************************************/
	ParticleManager::~ParticleManager(void)
	{
		/*std::vector<Emitter*>::iterator emitIt = m_EmitterList.begin();
		for(emitIt; emitIt != m_EmitterList.end(); ++emitIt)
		{
		delete (*emitIt)->pParticles;
		(*emitIt)->pParticles = NULL;
		}*/

	}
	void ParticleManager::Initialize(void)
	{

		m_capacity = 0;      // Capacity of emitter
		m_size = 0;					// Number of emitter
		m_currentID = 0;			// ID of emitter

		m_minExpVel = 0;
		m_maxExpVel = 100;
		m_minExpScale = 0;
		m_maxExpScale = 100;

		m_minTrailVel = 0;
		m_maxTrailVel = 100;
		m_minTrailScale = 0;
		m_maxTrailScale = 50;

		m_scaleFactor = 30;
		m_expLife = 20;

		trailNumber = 0;
	}

	void ParticleManager::Update(float dt)
	{
		std::vector<Emitter*>::iterator emitterIT;
		for (emitterIT = m_EmitterList.begin(); emitterIT != m_EmitterList.end(); ++emitterIT)
		{
			switch ((*emitterIT)->type)
			{
			case ET_EXPLOSION:
			{
				for (int i = 0; i < (*emitterIT)->capacity; ++i)
				{
					Particle& particle = (*emitterIT)->pParticles[i];

					//Update particle position based on velocity and dt
					particle.pos += (particle.vel * dt);
					//Update particle position based on velocity and dt
					particle.scale -= m_scaleFactor * dt;
					//Clamp particle scale to 0 and maxExpScale
					if (particle.scale <= 0)
						particle.scale = 0;
				}
				//Update emitter lifetime based on dt
				(*emitterIT)->lifeTime += dt;
				//If lifetime is greater than expLife delete this emitter
				if ((*emitterIT)->lifeTime >= m_expLife)
				{
					(*emitterIT)->lifeTime = 0.0f;
					float rotation = 0.0f;
					for (int i = 0; i < (*emitterIT)->capacity; ++i)
					{
						// set position of particle to emitter
						(*emitterIT)->pParticles[i].pos = (*emitterIT)->pos;
						// set rotation with random
						rotation = TUMath::GetRandomFloat(0, TUMath::TWO_PI);
						// Then Get a random rotation between 0 and 2PI
						// and set the velocity x and y based on the rotation.
						(*emitterIT)->pParticles[i].vel = { cosf(rotation), sinf(rotation) , 0.0f };
						(*emitterIT)->pParticles[i].vel
							*= TUMath::GetRandomFloat(m_minExpScale, m_maxExpScale);;
						(*emitterIT)->pParticles[i].scale
							= TUMath::GetRandomFloat(m_minExpScale, m_maxExpScale);
						// set color with g_colors
						(*emitterIT)->pParticles[i].color[0] = 0 / 255.f;
						(*emitterIT)->pParticles[i].color[1] = 255/255.f;
						(*emitterIT)->pParticles[i].color[2] = 255 /255.f;
						(*emitterIT)->pParticles[i].color[3] = 255 /255.f;
					}
				}
				break;
			}
			case ET_TRAIL:
			{
				for (int i = 0; i < (*emitterIT)->capacity; ++i)
				{
					Particle& particle = (*emitterIT)->pParticles[i];
					//If scale of particle is 0
					if (particle.scale <= 0)
					{
						//Get rotation of emitter using atan2
						float rotation = atan2f((*emitterIT)->vel.y, (*emitterIT)->vel.x);
						// Rotate that value by PI
						rotation += TUMath::PI;
						// Add a random rotation between + or PI/4
						rotation += TUMath::GetRandomFloat(-TUMath::PI / 4, TUMath::PI / 4);
						// Use rotation to calculate velocity x and y
						particle.vel = { cosf(rotation), sinf(rotation), 0.0f };
						//Scale velocity by random float
						//between minTrailVel and maxTrailVel
						particle.vel *= TUMath::GetRandomFloat(m_minTrailVel, m_maxTrailVel);
						//Set particle position to emitter position.
						particle.pos = (*emitterIT)->pos;
						//Set particle scale to random float
						//between minTrailScale and maxTrailScale
						particle.scale
							= TUMath::GetRandomFloat(m_minTrailScale, m_maxTrailScale);
					}
					//For every particle
					//Update particle position based on velocity and dt
					//	particle.pos += particle.vel * dt;
					//particle.pos += (*emitterIT)->vel * dt;
					//Update particle scale based on scaleFactor and dt
					particle.scale -= m_scaleFactor * dt;
					//Clamp particle scale to 0 and maxTrailScale
					TUMath::Clamp(particle.scale, 0, m_maxTrailScale);

					// set colour with RED
					particle.color[0] = 127 /255.f;
					particle.color[1] = 104 / 255.f;
					particle.color[2] = 81 / 255.f;
					particle.color[3] = 180 / 255.f;


				}
				break;
			}
			}
		}
	}

	void ParticleManager::AddEmitter(Emitter* pEmitter)
	{
		switch (pEmitter->type)
		{
		case ET_EXPLOSION:
			InitExplosionSystem(pEmitter);
			break;
		case ET_TRAIL:
			InitTrailSystem(pEmitter);
			break;
		}
		// todo add emitter
		m_EmitterList.push_back(pEmitter);
	}

	void ParticleManager::DeleteEmitter(std::vector <Emitter*>::iterator emitterIt)
	{
		// Todo find emitter to delete
		m_EmitterList.erase(emitterIt);
	}

	/*!

	Initialize explosion with many mathematical function.

	\param pEmitter
	emitter to set the explosion.

	\attention
	Use random properly because of the float and double values.

	*/
	/******************************************************************************/
	void ParticleManager::InitExplosionSystem(Emitter* pEmitter)
	{
		float rotation = 0; // rotation of particle

		for (int i = 0; i < pEmitter->capacity; ++i)
		{
			// set position of particle to emitter
			pEmitter->pParticles[i].pos = pEmitter->pos;
			// set rotation with random
			rotation = TUMath::GetRandomFloat(0, TUMath::TWO_PI);
			// Then Get a random rotation between 0 and 2PI
			// and set the velocity x and y based on the rotation.
			pEmitter->pParticles[i].vel = { cosf(rotation), sinf(rotation) , 0.0f };
			pEmitter->pParticles[i].vel
				*= TUMath::GetRandomFloat(m_minExpScale, m_maxExpScale);;
			pEmitter->pParticles[i].scale
				= TUMath::GetRandomFloat(m_minExpScale, m_maxExpScale);
			// set color with g_colors
			pEmitter->pParticles[i].color[0] = g_colors[TUMath::GetRandomInt(-1, MAX_COLORS - 1)];
			pEmitter->pParticles[i].color[1] = g_colors[TUMath::GetRandomInt(-1, MAX_COLORS - 1)];
			pEmitter->pParticles[i].color[2] = g_colors[TUMath::GetRandomInt(-1, MAX_COLORS - 1)];
			pEmitter->pParticles[i].color[3] = g_colors[TUMath::GetRandomInt(-1, MAX_COLORS - 1)];
		}

	}
	/******************************************************************************/
	/*!

	Initialize trail particles' scale with loop.

	\param pEmitter
	Emitter to initialize trail.

	*/
	/******************************************************************************/
	void ParticleManager::InitTrailSystem(Emitter* pEmitter)
	{
		// fill all particles in emitter with loop
		for (int i = 0; i < pEmitter->capacity; ++i)
		{
			pEmitter->pParticles[i].scale = 0;
			pEmitter->pParticles[i].color[0] = 255 / 255.f;
			pEmitter->pParticles[i].color[1] = 128 / 255.f;
			pEmitter->pParticles[i].color[2] = 128 / 255.f;
			pEmitter->pParticles[i].color[3] = 255 / 255.f;
		}
	}

}
