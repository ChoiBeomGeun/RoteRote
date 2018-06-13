/******************************************************************************/
/*!
\file	ParticleManager.h
\author	HyunJun Yoo
\par	email: hyunjun306@gmail.com
\par	Class:GAM250
\par	ThumbUp Engine
\date	06/11/2015

ParticleManager header
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#ifndef PARTICLETEST_H
#define PARTICLETEST_H

#include "System.h"

#include "glm\glm.hpp"
#include "Component.h"
#include "Particles.h"
#include <vector>


namespace TE {

	/******************************************************************************/
	/*!
	\class ParticleManager
	class of ParticleManager for assignment. There are functions for Emitters

	- PUBLIC
	- LoadFile
	- GetEmitters
	- GetEmmiterCount
	- AddEmitter
	- Delete_all_particles
	- UpdateEmitter
	- DrawEmitter

	- PRIVATE
	- init_win_condition_system_system
	- InitTrailSystem

	*/
	/******************************************************************************/
	class Emitter;

	class ParticleManager : public Systems
	{
	public:
		ParticleManager();
		~ParticleManager(void);

		void Initialize(void) override;
		Object * LoadEmitter(Object* pobject, std::string  path);
		void Update(float dt) override;
		void Initialize(Emitter * pEmitter);

		void AddEmitter(Emitter* pEmitter);
		void Delete_all_particles();
		
		std::vector <Emitter*> m_EmitterList;
		void initialize_life_time();
		void turn_on_particles(bool ison);
		int m_capacity;       //!< max amount of Emitter
		int m_size;           //!< number of Emitter
		int m_currentID;      //!< current ID of Emitter

		
		float m_minExpVel;    //!< minimum velocity of explosion
		float m_maxExpVel;    //!< maximum velocity of explosion
		float m_minExpScale;  //!< minimum scale of explosion
		float m_maxExpScale;  //!< maximum scale of explosion

		float m_minTrailVel;  //!< minimum velocity of trail
		float m_maxTrailVel;  //!< maximum velocity of trail
		float m_minTrailScale;//!< minimum scale of trail
		float m_maxTrailScale;//!< maximum scale of trail

		float m_minBackgroundDist; //!< minimum Distance of Background
		float m_maxBackgroundDist; //!< maximum Distance of Background
		float m_maxBackLifeTime; //!< maximum lifetime of Background
								 //float m_maxBackLimitsize; //!< maximum size of Background
		glm::vec3 m_laserVel;

		float m_scaleFactor;  //!< scale factor
		float m_expLife;      //!< explosion life
	private:
		void init_win_condition_system_system(Emitter* pEmitter);
		void InitTrailSystem(Emitter* pEmitter);
		void InitBackgroundSystem(Emitter* pEmitter);
		void init_particle_system(Emitter * pEmitter);
		void init_menu_system(Emitter * pEmitter);
	};

	extern ParticleManager * PARTICLEMANAGER;
}

#endif /*PARTICLETEST_H*/
