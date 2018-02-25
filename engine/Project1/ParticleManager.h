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
#include "particle.h"
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
	- InitExplosionSystem
	- InitTrailSystem

	*/
	/******************************************************************************/
	class Emitter;

	class ParticleManager : public Systems
	{
	public:
		ParticleManager();
		//ParticleManager(int maxCount);
		~ParticleManager(void);

		void Initialize(void) override;
		void EmitterInit(int maxCount);


		void Update(float dt) override;


		//void     LoadFile(const char* pFileName);


		Emitter* GetEmitters(void);
		int      GetEmmiterCount(void);
		int      AddEmitter(int count, const glm::vec3& pos,
			const glm::vec3& vel, int textureID, EmitterType type);
		void     DeleteEmitter(int emitterID);
		void     UpdateEmitter(Emitter* pEmitter, float dt);
		//void     DrawEmitter(const Emitter* pEmitter);

	private:
		void InitExplosionSystem(Emitter* pEmitter);
		void InitTrailSystem(Emitter* pEmitter);

		//std::vector <Emitter*> m_EmitterList;
		Emitter* m_pEmitters; //!< pointer of Emitter
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

		float m_scaleFactor;  //!< scale factor
		float m_expLife;      //!< explosion life
	};

	extern ParticleManager * PARTICLEMANAGER;
}

#endif /*PARTICLETEST_H*/
