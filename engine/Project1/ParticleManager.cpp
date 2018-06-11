/******************************************************************************/
/*!
\file		ParticleManager.cpp
\author	HyunJun Yoo
\par		email: hyunjun306@gmail.com
\par		Class:GAM250
\par		ThumbUp Engine
\date		06/11/2015

This is where particles are controlled
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "ParticleManager.h"

#include "glm\glm.hpp"
#include "DebugUtil.h"
#include "TUMath.h"

#include <cmath> /*atan2*/
#include <cstdio> /*FILE* fopen, fclose fscanf*/
#include <iostream>
#include "json/JsonClass.h"
#include "StateManager.h"
#include "..\..\example\examples\PlayerController.h"
#include "Factory.h"
#include "Object.h"
#include "Application.h"
#include <random>


using namespace std;

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

	ParticleManager::ParticleManager()
	{
		DEBUG_ASSERT(PARTICLEMANAGER != nullptr, "There should be only one ParticleManager");
		PARTICLEMANAGER = this;

	}

	/******************************************************************************/
	/*!

	Deletes all pParticles from the emitters then deletes the emitter array.

	\attention
	don't forget delete consistently.

	*/
	/******************************************************************************/
	ParticleManager::~ParticleManager(void)
	{
		std::vector<Emitter*>::iterator emitIt = m_EmitterList.begin();
		if(!m_EmitterList.empty())
		for(emitIt; emitIt != m_EmitterList.end(); ++emitIt)
		{
			delete (*emitIt)->pParticles;
			(*emitIt)->pParticles = NULL;
		}

	}
	void ParticleManager::Initialize(void)
	{

		m_capacity = 0;      // Capacity of emitter
		m_size = 0;					// Number of emitter
		m_currentID = 0;			// ID of emitter

		m_minExpVel = 0;
		m_maxExpVel = 100;
		m_minExpScale = 0;
		m_maxExpScale = 75;

		m_minTrailVel = 0;
		m_maxTrailVel = 100;
		m_minTrailScale = 0;
		m_maxTrailScale = 50;

		m_minBackgroundDist = -(APP->_screenWidth * .3f);
		m_maxBackgroundDist = APP->_screenWidth * .3f;
		m_maxBackLifeTime = 20;
		m_scaleFactor = 30;
		m_expLife = 20;
		m_laserVel = glm::vec3(0);
		m_EmitterList.clear();
	}

	void ParticleManager::Initialize(Emitter* /*pEmitter*/)
	{
		std::vector<Emitter*>::iterator emitterIT;
		for (emitterIT = m_EmitterList.begin(); emitterIT != m_EmitterList.end(); ++emitterIT)
		{
			switch ((*emitterIT)->type)
			{
			case ET_EXPLOSION:
			{
				init_win_condition_system_system(*emitterIT);
			}
			break;
			case ET_TRAIL:
			{
				InitTrailSystem(*emitterIT);
			}
			break;
			case ET_SELECTION:
			{
				init_menu_system(*emitterIT);
			}
			break;
			case ET_LASER:
			{
				init_particle_system(*emitterIT);
			}
			break;
			}
		}
	}

	Object * ParticleManager::LoadEmitter(Object* pobject, std::string path)
	{

		
		std::string loadParticle = ".\\Emitters.\\";
		loadParticle+= path;
		
	/*	#ifdef _DEBUG
		
		#else
		path = Userinfo;
		path += "/Documents/RoteRote/Emitters/" + loadParticle;
		#endif
		free(Userinfo);*/
	
		Jsonclass file;
		std::string object = "Emitter";

		//	char *path = (char*)JSON_FILE;
		file.ReadFile(loadParticle);


		for (int i = 1; i < file.mRoot.get("NumberOfEmitter", false).asInt() + 1; i++)
		{
			std::string Objectstyle = file.mRoot.get(object + to_string(i), false).get("ObjectType", false).asString();
			std::string emitterType = file.mRoot.get(object + to_string(i), false).get("EmitterType", false).asString();
			unsigned int emitterTypeID = 0;
			if (emitterType == "ET_EXPLOSION")
			{
				emitterTypeID = ET_EXPLOSION;
			}
			else if (emitterType == "ET_TRAIL")
			{
				emitterTypeID = ET_TRAIL;
			}
			else if (emitterType == "ET_SELECTION")
			{
				emitterTypeID = ET_SELECTION;
			}
			else if (emitterType == "ET_LASER")
				emitterTypeID = ET_LASER;
			
			float Xpos = file.mRoot.get(object + to_string(i), false).get("Position", false).get("x", false).asFloat();
			float Ypos = file.mRoot.get(object + to_string(i), false).get("Position", false).get("y", false).asFloat();
			float Xscale = file.mRoot.get(object + to_string(i), false).get("Scale", false).get("x", false).asFloat();
			float Yscale = file.mRoot.get(object + to_string(i), false).get("Scale", false).get("y", false).asFloat();

			float Xvel = file.mRoot.get(object + to_string(i), false).get("Vel", false).get("x", false).asFloat();
			float Yvel = file.mRoot.get(object + to_string(i), false).get("Vel", false).get("y", false).asFloat();
			float Zvel = file.mRoot.get(object + to_string(i), false).get("Vel", false).get("z", false).asFloat();
			float ColorR = file.mRoot.get(object + to_string(i), false).get("Color", false).get("r", false).asFloat();
			float ColorG = file.mRoot.get(object + to_string(i), false).get("Color", false).get("g", false).asFloat();
			float ColorB = file.mRoot.get(object + to_string(i), false).get("Color", false).get("b", false).asFloat();
			float ColorA = file.mRoot.get(object + to_string(i), false).get("Color", false).get("a", false).asFloat();


			std::string textureDir = file.mRoot.get(object + to_string(i), false).get("TextureDir", false).asString();
			int emittersize = file.mRoot.get(object + to_string(i), false).get("EmitterSize", false).asInt();
			int emitterID = file.mRoot.get(object + to_string(i), false).get("EmitterID", false).asInt();
			float emitterlifeTime = file.mRoot.get(object + to_string(i), false).get("EmitterLifeTime", false).asFloat();
			int capacity = file.mRoot.get(object + to_string(i), false).get("Emittercapacity", false).asInt();
			bool isBlened = file.mRoot.get(object + to_string(i), false).get("RenderMode", false).asBool();


			for (unsigned int indexC = 0; file.mRoot.get("NumberOfComponents", false).asInt(); indexC++) {
				if (pobject->HasComponent<Sprite>())
				{
					//continue;
				}
				if (pobject->HasComponent<Transform>())
				{
					//continue;
				}

				if (file.mRoot[object + to_string(i)]["Components"][indexC].asString() == "PARTICLE") {
					pobject->AddComponent<Emitter>();
					pobject->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id(textureDir);
					pobject->GetComponent<Sprite>()->depth = -2.0f;
					pobject->GetComponent<Transform>()->position = glm::vec3(Xpos, Ypos, 0);
					pobject->GetComponent<Transform>()->scale = glm::vec3(0, 0, 0);
					glm::vec3 t_Vel(Xvel, Yvel, Zvel);
					pobject->GetComponent<Emitter>()->size = emittersize;
					pobject->GetComponent<Emitter>()->emitterID = emitterID;
					pobject->GetComponent<Emitter>()->lifeTime = emitterlifeTime;
					pobject->GetComponent<Emitter>()->capacity = capacity;
					// switch for particles
					pobject->GetComponent<Emitter>()->isOn = false;
					pobject->GetComponent<Emitter>()->SetEmitter(pobject->GetComponent<Transform>()->position, t_Vel, emittersize, capacity, emitterlifeTime, (EmitterType)emitterTypeID,pobject->GetComponent<Emitter>()->m_particlePath);
					pobject->GetComponent<Emitter>()->SetTexture(pobject->GetComponent<Sprite>()->m_TextureID);
					pobject->GetComponent<Emitter>()->m_particlePath = path;
					pobject->GetComponent<Emitter>()->CreateParticle();
					pobject->GetComponent<Emitter>()->pParticles->scale = glm::vec3(Xscale, Yscale, 0);

					for (int j = 0; j < pobject->GetComponent<Emitter>()->capacity; ++j)
					{
						pobject->GetComponent<Emitter>()->pParticles[j].color[0] = ColorR / 255.f;
						pobject->GetComponent<Emitter>()->pParticles[j].color[1] = ColorG / 255.f;
						pobject->GetComponent<Emitter>()->pParticles[j].color[2] = ColorB / 255.f;
						pobject->GetComponent<Emitter>()->pParticles[j].color[3] = ColorA / 255.f;

					}
					pobject->GetComponent<Emitter>()->isAdditive = isBlened;

					PARTICLEMANAGER->AddEmitter(pobject->GetComponent<Emitter>());
				}
				if (Objectstyle == "Particle")
				{
					pobject->objectstyle = Objectstyle::Particle;
				}

				if (file.mRoot[object + to_string(i)]["Components"][indexC].asString() == "")
					return pobject;//	break;

				pobject->Initialize();
			}
		}
		return nullptr;
	}

	void ParticleManager::Update(float dt)
	{
		std::vector<Emitter*>::iterator EIT = m_EmitterList.begin();
		if(!m_EmitterList.empty())
			while (EIT != m_EmitterList.end())
			{
				if ((*EIT)->pParticles)
				{
					switch ((*EIT)->type)
					{

					case ET_EXPLOSION:
					{

						for (int i = 0; i < (*EIT)->capacity; ++i)
						{
							Particle& particle = (*EIT)->pParticles[i];

							//Update particle position based on velocity and dt
							particle.pos += (particle.vel * dt);
							//Update particle position based on velocity and dt
							particle.scale -= m_scaleFactor * dt;
							//Clamp particle scale to 0 and maxExpScale
							if (particle.scale.x <= 0 || particle.scale.y <= 0)
								particle.scale = glm::vec3(0);
						}
						//Update emitter lifetime based on dt
						(*EIT)->lifeTime += dt;
						//If lifetime is greater than expLife delete this emitter
						if ((*EIT)->lifeTime >= m_expLife)
						{
							(*EIT)->lifeTime = 0.0f;
							float rotation = 0.0f;
							for (int i = 0; i < (*EIT)->capacity; ++i)
							{
								// set position of particle to emitter
								(*EIT)->pParticles[i].pos = (*EIT)->pos;
								// set rotation with random
								rotation = TUMath::GetRandomFloat(0, TUMath::TWO_PI);
								// Then Get a random rotation between 0 and 2PI
								// and set the velocity x and y based on the rotation.
								(*EIT)->pParticles[i].vel = { cosf(rotation), sinf(rotation) , 0.0f };
								(*EIT)->pParticles[i].vel
									*= TUMath::GetRandomFloat(m_minExpScale, m_maxExpScale);;
								(*EIT)->pParticles[i].scale	= glm::vec3(TUMath::GetRandomFloat(m_minExpScale, m_maxExpScale));
							}
						}
					}
					break;

					case ET_TRAIL:
					{ 
						for (int i = 0; i < (*EIT)->capacity; ++i)
						{
							Particle& particle = (*EIT)->pParticles[i];
							//If scale of particle is 0

							if (particle.scale.x <= 0 || particle.scale.y <= 0)
							{
								//Set particle position to emitter position.
								particle.pos = (*EIT)->pos;
								//Set particle scale to random float
								//between minTrailScale and maxTrailScale
								particle.scale = glm::vec3(TUMath::GetRandomFloat(m_minTrailScale, m_maxTrailScale));
							}
							//Update particle scale based on scaleFactor and dt
							particle.scale -= m_scaleFactor * dt;
							//Clamp particle scale to 0 and maxTrailScale
							//TUMath::Clamp(particle.scale, 0, m_maxTrailScale);
						}
						break;
					}
					case ET_SELECTION:
					{
						for (int i = 0; i < (*EIT)->capacity; ++i)
						{
							Particle& particle = (*EIT)->pParticles[i];

							particle.pos.x += cosf(TUMath::PI * .1f * i + dt);
							particle.pos.y += sinf(TUMath::PI * .1f * i + dt);

							if (particle.scale.x  <= 0 || particle.scale.y <= 0)
							{
								//Set particle position to emitter position.
								particle.pos = (*EIT)->pos;
								//Set particle scale to random float
								//between minTrailScale and maxTrailScale
								particle.scale = glm::vec3(20.f);
									
							}
							//Update particle scale based on scaleFactor and dt
							particle.scale -= m_scaleFactor * dt;
						}
						//Update emitter lifetime based on dt
						
						
						break;
					}
					case ET_LASER:
					{
						(*EIT)->vel = m_laserVel;
						//float xscale = 20.f;
						for (int i = 0; i < (*EIT)->capacity; ++i)
						{
							Particle & particle = (*EIT)->pParticles[i];
							particle.vel = (*EIT)->vel *dt;
							particle.pos += particle.vel;

							particle.lifetime += dt;
							if (particle.lifetime > (*EIT)->explifeTime)
							{
								particle.pos = (*EIT)->pos;

								particle.lifetime = 0;
								particle.scale.x = TUMath::GetRandomFloat(m_minTrailScale, m_maxTrailScale);
								particle.scale.y = TUMath::GetRandomFloat(m_minTrailScale, m_maxTrailScale);

							}							
						}
						break;
					}
					}

					++EIT;
				}
			}
	}

	void ParticleManager::AddEmitter(Emitter* pEmitter)
	{
		switch (pEmitter->type)
		{
		case ET_EXPLOSION:
			init_win_condition_system_system(pEmitter);
			break;
		case ET_TRAIL:
			InitTrailSystem(pEmitter);
			break;
		case ET_SELECTION:
			init_menu_system(pEmitter);
			break;
		case ET_LASER:
			init_particle_system(pEmitter);
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

	void ParticleManager::initialize_life_time()
	{
		for(auto p : m_EmitterList)
		{
			if(p->type== ET_EXPLOSION)
			{
				p->lifeTime = m_expLife;
			}
		}
	}

	void ParticleManager::turn_on_particles(bool ison)
	{
		if(ison)
		{
			for (auto p : m_EmitterList)
				p->isOn = true;
		}
		else
		{
			for (auto p : m_EmitterList)
				p->isOn = false;
			
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
	void ParticleManager::init_win_condition_system_system(Emitter* pEmitter)
	{
		float rotation = 0; // rotation of particle
		pEmitter->explifeTime = m_expLife;
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
			pEmitter->pParticles[i].scale.x
				= TUMath::GetRandomFloat(m_minExpScale, m_maxExpScale);
			pEmitter->pParticles[i].scale.y
				= TUMath::GetRandomFloat(m_minExpScale, m_maxExpScale);
			// set color with g_colors
			pEmitter->pParticles[i].color[0] = i / 255.f;
			pEmitter->pParticles[i].color[1] = ((i*i)%200 )/ 255.f;
			pEmitter->pParticles[i].color[2] = (2*i%200) / 255.f;
			pEmitter->pParticles[i].color[3] = ((i % 255) + i) / 255.f;
			pEmitter->pParticles[i].lifetime = 0.0f;
			pEmitter->pParticles[i].angle = rotation;
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
		pEmitter->explifeTime = m_expLife;

		// fill all particles in emitter with loop
		for (int i = 0; i < pEmitter->capacity; ++i)
		{
			pEmitter->pParticles[i].scale = glm::vec3(0);
			pEmitter->pParticles[i].pos = glm::vec3(0);
			pEmitter->pParticles[i].color[0] = 255 / 255.f;
			pEmitter->pParticles[i].color[1] = 255 / 255.f;
			pEmitter->pParticles[i].color[2] = 255 / 255.f;
			pEmitter->pParticles[i].color[3] = 255 / 255.f;
			pEmitter->pParticles[i].lifetime = 0.0f;
			pEmitter->pParticles[i].angle = 0.f;
		}
	}
	/******************************************************************************/
	/*!

	Initialize background particles' scale with loop.

	\param pEmitter
	Emitter to initialize background.

	*/
	/******************************************************************************/
	void ParticleManager::InitBackgroundSystem(Emitter* pEmitter)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(m_minBackgroundDist, m_maxBackgroundDist);
		std::uniform_real_distribution<> rot(0, TUMath::TWO_PI);
		pEmitter->explifeTime = m_expLife;

		for (int i = 0; i< pEmitter->capacity; ++i)
		{
			pEmitter->pParticles[i].pos = glm::vec3(dis(gen) *.9f, dis(gen)*.5f, 0);
			// set rotation with random
			//float rotation = TUMath::GetRandomFloat(0, TUMath::TWO_PI);
			pEmitter->pParticles[i].scale.x
				= TUMath::GetRandomFloat(m_minExpScale, m_maxExpScale);
			//pEmitter->pParticles[i].pos = glm::vec3(0);
			pEmitter->pParticles[i].scale.y = TUMath::GetRandomFloat(m_minExpScale, m_maxExpScale);
			pEmitter->pParticles[i].color[0] = 255 / 255.f;
			pEmitter->pParticles[i].color[1] = 255 / 255.f;
			pEmitter->pParticles[i].color[2] = 255 / 255.f;
			pEmitter->pParticles[i].color[3] = 255 / 255.f;
			pEmitter->pParticles[i].vel = glm::vec3(0);
			pEmitter->pParticles[i].lifetime = TUMath::GetRandomFloat(0, m_maxBackLifeTime);
			pEmitter->pParticles[i].angle = static_cast<float>(rot(gen));
		}
	}
	void ParticleManager::init_particle_system(Emitter * pEmitter)
	{
		pEmitter->explifeTime = m_expLife;
		pEmitter->vel = m_laserVel;
		for (int i = 0; i < pEmitter->capacity; ++i)
		{
			pEmitter->pParticles[i].pos = pEmitter->pos;
			pEmitter->pParticles[i].angle = 0;
			pEmitter->pParticles[i].lifetime = i;
			pEmitter->pParticles[i].scale = glm::vec3(0);
			pEmitter->pParticles[i].vel = pEmitter->vel;
		}
	}

	void ParticleManager::init_menu_system(Emitter* pEmitter)
	{
		pEmitter->explifeTime = m_expLife;

		for (int i = 0; i < pEmitter->capacity; ++i)
		{
			pEmitter->pParticles[i].pos = pEmitter->pos;
			
			pEmitter->pParticles[i].angle = 0;
			pEmitter->pParticles[i].lifetime = 0;
			pEmitter->pParticles[i].scale = glm::vec3(0);
			pEmitter->pParticles[i].vel = glm::vec3(cosf(TUMath::DegreeToRadian(i)), sinf(TUMath::DegreeToRadian(i)),0.f);
			pEmitter->pParticles[i].color[0] = ((i*i) % 50 + i) / 255.f;
			pEmitter->pParticles[i].color[1] = 200 / 255.f;
			pEmitter->pParticles[i].color[2] = (2 * i % 200) / 255.f;
			pEmitter->pParticles[i].color[3] = (255 - i) / 255.f;

		}
	}

	
}
