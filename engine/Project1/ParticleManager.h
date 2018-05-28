/******************************************************************************/
/*!
\file   ParticleManager.h
\author HyunJun Yoo
\par    email: hyunjun306\@gmail.com

\brief
This file has the implementation of the functions for Assignment3

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
	- DeleteEmitter
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
		//void SaveEmitter(std::string  path);

		void Update(float dt) override;
		void Initialize(Emitter * pEmitter);
		

		void AddEmitter(Emitter* pEmitter);
		void DeleteEmitter(std::vector <Emitter*>::iterator emitterIt);
		//void UpdateEmitter(Emitter* pEmitter, float dt);
		//void     DrawEmitter(const Emitter* pEmitter);
		std::vector <Emitter*> m_EmitterList;
		void initialize_life_time();
	private:
		void init_win_condition_system_system(Emitter* pEmitter);
		void InitTrailSystem(Emitter* pEmitter);
		void InitBackgroundSystem(Emitter* pEmitter);
		void init_particle_system(Emitter * pEmitter);
		void init_sliding_system(Emitter * pEmitter);

		//Emitter* m_pEmitters; //!< pointer of Emitter
		int m_capacity;       //!< max amount of Emitter
		int m_size;           //!< number of Emitter
		int m_currentID;      //!< current ID of Emitter

		int trailNumber;      //!< number of trail Emitter

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

		float m_scaleFactor;  //!< scale factor
		float m_expLife;      //!< explosion life
	};

	extern ParticleManager * PARTICLEMANAGER;
}

#endif /*PARTICLETEST_H*/
