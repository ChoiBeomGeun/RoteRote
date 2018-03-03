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
		std::cout << "Emitter DEEEEEELEETTTE" << std::endl;
		// Delete all particle in the emitters
		for (int i = 0; i < m_capacity; ++i)
		{
			delete[] m_pEmitters[i].pParticles;
			m_pEmitters[i].pParticles = 0;
		}
		// and delete emitter array
		delete[] m_pEmitters;
		m_pEmitters = 0;
	}
	void ParticleManager::Initialize(void)
	{
		if (m_pEmitters == NULL)
			return;
		else
		{
			for (int i = 0; i < m_size; ++i)
			{
				m_pEmitters[i].capacity = 10;
				m_pEmitters[i].emitterID = 0;
				m_pEmitters[i].lifeTime = 0;
				m_pEmitters[i].pos = { 0, 0,0 };
				m_pEmitters[i].size = 0;
				m_pEmitters[i].m_textureID = 0;
				m_pEmitters[i].type = ET_TRAIL;
				m_pEmitters[i].vel = { 0, 0 ,0 };
			}
		}
		//m_capacity = 0;      // Capacity of emitter
		//m_size = 0;					// Number of emitter
		//m_currentID = 0;			// ID of emitter

		//m_minExpVel = 0;
		//m_maxExpVel = 100;
		//m_minExpScale = 0;
		//m_maxExpScale = 100;

		//m_minTrailVel = 0;
		//m_maxTrailVel = 100;
		//m_minTrailScale = 0;
		//m_maxTrailScale = 100;

		//m_scaleFactor = 30;
		//m_expLife = 20;

		//trailNumber = 0;
	}
	void ParticleManager::CreateMaxNumEmitter(int maxCount)
	{
		// allocate emitter based on maxcount
		if (m_pEmitters == NULL)
			m_pEmitters = new Emitter[maxCount];
		else
			return;

		// initialize all emitter data as well
		for (int i = 0; i < maxCount; ++i)
		{
			m_pEmitters[i].pParticles = 0;
			m_pEmitters[i].capacity = 10;
			m_pEmitters[i].emitterID = 0;
			m_pEmitters[i].lifeTime = 0;
			m_pEmitters[i].pos = { 0, 0,0 };
			m_pEmitters[i].size = 0;
			m_pEmitters[i].m_textureID = 0;
			m_pEmitters[i].type = ET_TRAIL;
			m_pEmitters[i].vel = { 0, 0 ,0 };
		}

		// initializes class members
		m_capacity = maxCount;      // Capacity of emitter
		m_size = 0;					// Number of emitter
		m_currentID = 0;			// ID of emitter

		m_minExpVel = 0;
		m_maxExpVel = 100;
		m_minExpScale = 0;
		m_maxExpScale = 100;

		m_minTrailVel = 0;
		m_maxTrailVel = 100;
		m_minTrailScale = 0;
		m_maxTrailScale = 100;

		m_scaleFactor = 30;
		m_expLife = 20;

		trailNumber = 0;
		//this->GetEmitters()->ParticleInit(maxCount, 20);
	}
	void ParticleManager::Update(float dt)
	{
		if (m_pEmitters == NULL)
			return;
		else {
			for (int i = 0; i < m_pEmitters->size; ++i)
			{
				m_pEmitters->Update(dt);
			}
		}
	}
	/******************************************************************************/
	/*!

	Returns the dynamically allocated emitter array.

	\return m_pEmitters
	Emittter array

	*/
	/******************************************************************************/
	Emitter* ParticleManager::GetEmitters(void)
	{
		// return to emittter array
		return m_pEmitters;
	}
	/******************************************************************************/
	/*!

	Returns the count of emitters currently in use.

	\return m_size
	size count Emitter

	*/
	/******************************************************************************/
	int      ParticleManager::GetEmmiterCount(void)
	{
		// Returns the count of emitters
		return m_size;
	}
	/******************************************************************************/
	/*!

	This function create 'Emitter'. Depending on the type,
	check amount of space and create emitter. If can't create,
	return to -1 and if you can, return with pEmitter->emitterID

	\param count
	amount of particle

	\param pos
	position of emitter

	\param vel
	velocity of emitter

	\param textureID
	texture of emitter.

	\param type
	type of emitter.

	\return -1
	If can not create emitter, return to -1

	\return pEmitter->emitterID
	can create emitter, return with emitter ID

	\attention
	I use trailNumber to check proper amount of emitter

	*/
	/******************************************************************************/
	int      ParticleManager::AddEmitter(int count, const glm::vec3& pos,
		const glm::vec3& vel, int textureID,
		EmitterType type)
	{
		//If we have reached the capacity for emitters then return 1.
		if (m_size == m_capacity)
			return -1;

		// if type is TRAIL
		if (type == ET_TRAIL)
		{
			++trailNumber; // add 1 to trailNumber
		}
		else if (type == ET_EXPLOSION)
		{
			// if Explosion, -1 to trailNumber
			--trailNumber;
		}

		// if capacity is bigger than half of capacity
		if (trailNumber > m_capacity / 2)
		{
			// -1 again to trailNumber
			//(check half of capacity because of capacity)
			--trailNumber;
			return -1;
		}


		// Adds emitter data to the end of the emitter array.
		Emitter* pEmitter = &m_pEmitters[m_size];
		//If the count is greater than the current particle capacity
		if (pEmitter->capacity < count)
		{
			//old array is deleted
			delete[] pEmitter->pParticles;
			//and a new one is created in place.
			pEmitter->pParticles = new Particle[count];
			pEmitter->size = count;
			pEmitter->capacity = count;
		}
		else
		{
			pEmitter->size = count;
		}

		//set emitter
		pEmitter->pos = pos;
		pEmitter->vel = vel;
		pEmitter->m_textureID = textureID;
		pEmitter->type = type;
		pEmitter->emitterID = m_currentID;
		pEmitter->lifeTime = 0;


		//use either InitExplostionSystem or InitTrailSystem to initialize
		switch (pEmitter->type)
		{
		case ET_EXPLOSION:
		{
			InitExplosionSystem(pEmitter);
		}
		break;
		case ET_TRAIL:
		{
			InitTrailSystem(pEmitter);
		}
		break;
		}
		++m_size;
		++m_currentID;
		// return to emitter ID
		return pEmitter->emitterID;
	}
	/******************************************************************************/
	/*!

	Delete function with ID

	\param emitterID
	ID of emitter to delete


	\attention
	don't forget to move particles with loop

	*/
	/******************************************************************************/
	void     ParticleManager::DeleteEmitter(int emitterID)
	{
		// - 1 to size because I delete emitter
		--m_size;

		for (int i = 0; i <= m_size; ++i)
		{
			//Swap the correct emitter with the last active one in the array.
			if (m_pEmitters[i].emitterID == emitterID)
			{
				// if size of emitter is smaller than current emitter,
				if (m_pEmitters[i].size < m_pEmitters[m_size].size)
				{
					// create more space for save.
					delete[] m_pEmitters[i].pParticles;
					m_pEmitters[i].pParticles = new Particle[m_pEmitters[m_size].size];
				}

				// fill the emitter to delete with current emitter
				m_pEmitters[i].type = m_pEmitters[m_size].type;
				m_pEmitters[i].pos = m_pEmitters[m_size].pos;
				m_pEmitters[i].vel = m_pEmitters[m_size].vel;
				m_pEmitters[i].m_textureID = m_pEmitters[m_size].m_textureID;
				m_pEmitters[i].size = m_pEmitters[m_size].size;
				m_pEmitters[i].capacity = m_pEmitters[m_size].capacity;
				m_pEmitters[i].lifeTime = m_pEmitters[m_size].lifeTime;
				m_pEmitters[i].emitterID = m_pEmitters[m_size].emitterID;

				// fill every particle with loop
				for (int j = 0; j < m_pEmitters[i].size; ++j)
				{
					m_pEmitters[i].pParticles[j].color
						= m_pEmitters[m_size].pParticles[j].color;
					m_pEmitters[i].pParticles[j].pos
						= m_pEmitters[m_size].pParticles[j].pos;
					m_pEmitters[i].pParticles[j].scale
						= m_pEmitters[m_size].pParticles[j].scale;
					m_pEmitters[i].pParticles[j].vel
						= m_pEmitters[m_size].pParticles[j].vel;
				}

			}
		}

	}
	/******************************************************************************/
	/*!

	Update the emitter based on the type which is ET_EXPLOSION and ET_TRAIL.

	\param pEmitter
	emitter to update.

	\param dt
	value to calculate the time length.

	*/
	/******************************************************************************/
	void     ParticleManager::UpdateEmitter(Emitter* pEmitter, float dt)
	{
		// Update the emitter based on the type.
		switch (pEmitter->type)
		{
		case ET_EXPLOSION:
		{
			for (int i = 0; i < pEmitter->size; ++i)
			{
				Particle& particle = pEmitter->pParticles[i];

				//Update particle position based on velocity and dt
				particle.pos += (particle.vel * dt);
				//Update particle position based on velocity and dt
				particle.scale -= m_scaleFactor * dt;
				//Clamp particle scale to 0 and maxExpScale
				if (particle.scale <= 0)
					particle.scale = 0;
			}
			//Update emitter lifetime based on dt
			pEmitter->lifeTime += dt;
			//If lifetime is greater than expLife delete this emitter
			if (pEmitter->lifeTime >= m_expLife)
			{
				DeleteEmitter(pEmitter->emitterID);
			}
			break;
		}
		case ET_TRAIL:
		{
			for (int i = 0; i < pEmitter->size; ++i)
			{
				Particle& particle = pEmitter->pParticles[i];
				//If scale of particle is 0
				if (particle.scale <= 0)
				{
					//Get rotation of emitter using atan2
					float rotation = atan2f(pEmitter->vel.y, pEmitter->vel.x);
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
					particle.pos = pEmitter->pos;
					//Set particle scale to random float
					//between minTrailScale and maxTrailScale
					particle.scale
						= TUMath::GetRandomFloat(m_minTrailScale, m_maxTrailScale);
				}
				//For every particle
				//Update particle position based on velocity and dt
				particle.pos += particle.vel * dt;
				particle.pos += pEmitter->vel * dt;
				//Update particle scale based on scaleFactor and dt
				particle.scale -= m_scaleFactor * dt;
				//Clamp particle scale to 0 and maxTrailScale
				TUMath::Clamp(particle.scale, 0, m_maxTrailScale);

				// set colour with RED
				particle.color.r = 255;
				particle.color.g = 0;
				particle.color.b = 0;
				particle.color.a = 255;


			}
			break;
		}
		}
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

		for (int i = 0; i < pEmitter->size; ++i)
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
			pEmitter->pParticles[i].color.r = g_colors[TUMath::GetRandomInt(-1, MAX_COLORS - 1)];
			pEmitter->pParticles[i].color.g = g_colors[TUMath::GetRandomInt(-1, MAX_COLORS - 1)];
			pEmitter->pParticles[i].color.b = g_colors[TUMath::GetRandomInt(-1, MAX_COLORS - 1)];
			pEmitter->pParticles[i].color.a = g_colors[TUMath::GetRandomInt(-1, MAX_COLORS - 1)];
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
		for (int i = 0; i < pEmitter->size; ++i)
		{
			pEmitter->pParticles[i].scale = 0;
		}
	}

}
