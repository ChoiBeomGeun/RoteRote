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
#include "json/JsonClass.h"
#include "StateManager.h"
#include "..\..\example\examples\PlayerController.h"
#include "Factory.h"
#include "Object.h"

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

	Object * ParticleManager::LoadEmitter(Object* pobject, std::string path)
	{
		char * Userinfo;
		size_t len = path.size();
		_dupenv_s(&Userinfo, &len, "USERPROFILE");


		std::string loadParticle = path;
#ifdef _DEBUG
		path = ".\\Emitters.\\" + path;
#else
		path = Userinfo;
		path += "/Documents/RoteRote/Emitters/" + loadParticle;
#endif
		free(Userinfo);

		//FACTORY->DestroyAllObjects();

		//STATEMANAGER->Loadtolevelname = loadParticle;
		Jsonclass file;
		std::string object = "Emitter";

		//	char *path = (char*)JSON_FILE;
		file.ReadFile(path);
		/*m_capacity = file.mRoot.get("Capacity", false).asInt();
		m_size = file.mRoot.get("Size", false).asInt();
		m_currentID = file.mRoot.get("CurrentID", false).asInt();
		m_minExpVel = file.mRoot.get("m_minExpVel", false).asFloat();
		m_maxExpVel = file.mRoot.get("m_maxExpVel", false).asFloat();
		m_minExpScale = file.mRoot.get("m_minExpScale", false).asFloat();
		m_maxExpScale = file.mRoot.get("m_maxExpScale", false).asFloat();
		m_minTrailVel = file.mRoot.get("m_minTrailVel", false).asFloat();
		m_maxTrailVel = file.mRoot.get("m_maxTrailVel", false).asFloat();
		m_minTrailScale = file.mRoot.get("m_minTrailScale", false).asFloat();
		m_maxTrailScale = file.mRoot.get("m_maxTrailScale", false).asFloat();
		m_scaleFactor = file.mRoot.get("m_scaleFactor", false).asFloat();
		m_expLife = file.mRoot.get("m_expLife", false).asFloat();
		trailNumber = file.mRoot.get("trailNumber", false).asInt();*/

		/*"PARTICLEMANAGER" :
		{
			"Capacity" : 20,
				"Size" : 0,
				"CurrentID" : 0,
				"m_minExpVel" : 0,
				"m_maxExpVel" : 100,
				"m_minExpScale" : 0,
				"m_maxExpScale" : 100,
				"m_minTrailVel" : 0,
				"m_maxTrailVel" : 100,
				"m_minTrailScale" : 0,
				"m_maxTrailScale" : 100,
				"m_scaleFactor" : 0,
				"m_expLife" : 100,
				"trailNumber" : 0
		},*/
		//Object *tempObject = FACTORY->CreateEmptyObject();

		for (int i = 1; i < file.mRoot.get("NumberOfEmitter", false).asInt() + 1; i++)
		{
			// initialize PARTICLEMANAGER member variables
		
			
			//	int objectID = file.mRoot.get(object + to_string(i), false).get("ObjectID", false).asInt();
			std::string Objectstyle = file.mRoot.get(object + to_string(i), false).get("ObjectType", false).asString();

			std::string emitterType = file.mRoot.get(object + to_string(i), false).get("EmitterType", false).asString();

			unsigned int emitterTypeID;
			if(emitterType == "ET_EXPLOSION")
			{
				emitterTypeID = ET_EXPLOSION;
			}
			else if (emitterType == "ET_TRAIL")
			{
				emitterTypeID = ET_TRAIL;
			}
			std::cout << emitterType <<  std::endl;;
			
			float Xpos = file.mRoot.get(object + to_string(i), false).get("Position", false).get("x", false).asFloat();
			float Ypos = file.mRoot.get(object + to_string(i), false).get("Position", false).get("y", false).asFloat();
			//		float Zpos = file.mRoot.get(object + to_string(i), false).get("Position", false).get("z", false).asFloat();
			float Xvel = file.mRoot.get(object + to_string(i), false).get("Vel", false).get("x", false).asFloat();
			float Yvel = file.mRoot.get(object + to_string(i), false).get("Vel", false).get("y", false).asFloat();
			float Zvel = file.mRoot.get(object + to_string(i), false).get("Vel", false).get("z", false).asFloat();

			std::string textureDir = file.mRoot.get(object + to_string(i), false).get("TextureDir", false).asString();
			int emittersize = file.mRoot.get(object + to_string(i), false).get("EmitterSize", false).asInt();
			int emitterID = file.mRoot.get(object + to_string(i), false).get("EmitterID", false).asInt();
			float emitterlifeTime = file.mRoot.get(object + to_string(i), false).get("EmitterLifeTime", false).asFloat();
			int capacity = file.mRoot.get(object + to_string(i), false).get("Emittercapacity", false).asInt();



			for (unsigned int indexC = 0; file.mRoot.get("NumberOfComponents", false).asInt(); indexC++) {
				if(pobject->HasComponent<Sprite>())
				{
					//continue;
				}
				if(pobject->HasComponent<Transform>())
				{
					//continue;
				}
				//if (file.mRoot[object + to_string(i)]["Components"][indexC].asString() == "TRANSFORM") {
				//	tempObject->AddComponent<Transform>();
				//	/*tempObject->GetComponent<Transform>()->position.x = Xpos;
				//	tempObject->GetComponent<Transform>()->position.y = Ypos;*/
				//}
				//if (file.mRoot[object + to_string(i)]["Components"][indexC].asString() == "SPRITE") {
				//	tempObject->AddComponent<Sprite>();
				//	tempObject->GetComponent<Sprite>()->depth = 0.0f;
				//	tempObject->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
				//	tempObject->GetComponent<Sprite>()->isPerspective = true;
				//	tempObject->GetComponent<Sprite>()->texture_load(textureDir);
				//}
				if (file.mRoot[object + to_string(i)]["Components"][indexC].asString() == "PARTICLE") {
					pobject->AddComponent<Emitter>();
					pobject->GetComponent<Sprite>()->texture_load("test.png");
					pobject->GetComponent<Transform>()->position = glm::vec3(Xpos, Ypos, 0);
					glm::vec3 t_Vel(Xvel, Yvel, Zvel);
					pobject->GetComponent<Emitter>()->size = emittersize;
					pobject->GetComponent<Emitter>()->emitterID = emitterID;
					pobject->GetComponent<Emitter>()->lifeTime = emitterlifeTime;
					pobject->GetComponent<Emitter>()->capacity = capacity;
					pobject->GetComponent<Emitter>()->SetEmitter(pobject->GetComponent<Transform>()->position, t_Vel, emittersize, capacity, emitterlifeTime, (EmitterType)emitterTypeID);
					pobject->GetComponent<Emitter>()->SetTexture(pobject->GetComponent<Sprite>()->TextureId);
					pobject->GetComponent<Emitter>()->CreateParticle();
					pobject->GetComponent<Transform>()->scale = glm::vec3(10.0f);

					PARTICLEMANAGER->AddEmitter(pobject->GetComponent<Emitter>());
				}
				if (Objectstyle == "Particle")
				{
					pobject->objectstyle = Objectstyle::Particle;
				}

				if (file.mRoot[object + to_string(i)]["Components"][indexC].asString() == "")
					return pobject;//	break;
				/*if (Objectstyle == "Particle")
				{
					tempObject->objectstyle = Objectstyle::Particle;
				}
				else
					DEBUG_ASSERT(true, "Invaild Object Style");*/

				/*if (tempObject->GetComponent<Sprite>())
				{
					FACTORY->ObjectIDMap[tempObject->objID]->GetComponent<Sprite>()->mTexutureDir = (const_cast<char*>(textureDir.c_str()));
					tempObject->GetComponent<Sprite>()->texture_load(const_cast<char*>(textureDir.c_str()));
				}*/
				/*if (tempObject->GetComponent<Sprite>())
				{
					FACTORY->ObjectIDMap[tempObject->objID]->GetComponent<Sprite>()->mTexutureDir = (const_cast<char*>(textureDir.c_str()));
					tempObject->GetComponent<Sprite>()->texture_load(const_cast<char*>(textureDir.c_str()));
				}*/
				pobject->Initialize();
			}
		}
		

	}
	

	void ParticleManager::SaveEmitter(std::string path)
	{
		char * Userinfo;
		size_t len = path.size();
		_dupenv_s(&Userinfo, &len, "USERPROFILE");

		std::string saveLevel = path;
#ifdef _DEBUG
		path = ".\\Emitters.\\" + path;
#else
		path = Userinfo;
		path += "/Documents/RoteRote/Emitters/" + saveLevel;
#endif
		free(Userinfo);
		Jsonclass file;
		Json::Value root;
		std::string object = "Object";


		root["EmitterTypel"] = 1;
		root["WorldSize"]["x"] = STATEMANAGER->v_StatesLists[STATEMANAGER->i_CurrentStateNumber]->WorldSizeX;
		root["WorldSize"]["y"] = STATEMANAGER->v_StatesLists[STATEMANAGER->i_CurrentStateNumber]->WorldSizeY;
		root["DefalutCamera"]["EYE"]["x"] = CAMERA->cameraPos.x;
		root["DefalutCamera"]["EYE"]["y"] = CAMERA->cameraPos.y;
		root["DefalutCamera"]["EYE"]["z"] = CAMERA->cameraPos.z;
		root["DefalutCamera"]["TARGET"]["x"] = CAMERA->cameraTarget.x;
		root["DefalutCamera"]["TARGET"]["y"] = CAMERA->cameraTarget.y;
		root["DefalutCamera"]["TARGET"]["z"] = CAMERA->cameraTarget.z;
		root["DefalutCamera"]["UP"]["x"] = CAMERA->cameraUp.x;
		root["DefalutCamera"]["UP"]["y"] = CAMERA->cameraUp.y;
		root["DefalutCamera"]["UP"]["z"] = CAMERA->cameraUp.z;
		root["NumberOfObjects"] = FACTORY->ObjectIDMap.size();
		int  i = 1;
		for (auto it : FACTORY->ObjectIDMap)
		{
			if (it.second->HasComponent<Emitter>())
			{

				it.second->GetComponent<Transform>()->position.z = 0;
				static unsigned particleSaveIndex = 0;
				if (it.second->GetComponent<Emitter>() != nullptr) {
					root[object + to_string(i)]["Components"][particleSaveIndex] = "PARTICLE";
					if (it.second->GetComponent<Emitter>()->type == EmitterType::ET_TRAIL)
						root[object + to_string(i)]["EmitterType"] = "TRAIL";
					else if (it.second->GetComponent<Emitter>()->type == EmitterType::ET_EXPLOSION)
						root[object + to_string(i)]["EmitterType"] = "ET_EXPLOSION";

					root[object + to_string(i)]["EmitterPos"]["x"] = it.second->GetComponent<Transform>()->position.x;
					root[object + to_string(i)]["EmitterPos"]["y"] = it.second->GetComponent<Transform>()->position.y;
					root[object + to_string(i)]["EmitterPos"]["z"] = it.second->GetComponent<Transform>()->position.z;
					
					root[object + to_string(i)]["EmitterVel"]["x"] = it.second->GetComponent<Emitter>()->vel.x;
					root[object + to_string(i)]["EmitterVel"]["y"] = it.second->GetComponent<Emitter>()->vel.y;
					root[object + to_string(i)]["EmitterVel"]["z"] = it.second->GetComponent<Emitter>()->vel.z;

					root[object + to_string(i)]["textureID"] = it.second->GetComponent<Emitter>()->m_textureID;
					root[object + to_string(i)]["Emittersize"] = it.second->GetComponent<Emitter>()->size;
					root[object + to_string(i)]["EmitterID"] = it.second->GetComponent<Emitter>()->emitterID;
					root[object + to_string(i)]["EmitterlifeTime"] = it.second->GetComponent<Emitter>()->lifeTime;
					root[object + to_string(i)]["Emittercapacity"] = it.second->GetComponent<Emitter>()->capacity;


					particleSaveIndex++;
				}
				particleSaveIndex = 0;
			}
			switch (it.second->objectstyle)
			{
			case Objectstyle::Particle:
				root[object + to_string(i)]["ObjectType"] = "Particle";
				break;
			default:
				DEBUG_ASSERT(true, "Invaild Object Style");


			}
			/*root[object + to_string(i)]["ObjectID"] = i;
			root[object + to_string(i)]["Position"]["x"] = it.second->GetComponent<Transform>()->position.x;
			root[object + to_string(i)]["Position"]["y"] = it.second->GetComponent<Transform>()->position.y;
			root[object + to_string(i)]["Position"]["z"] = it.second->GetComponent<Transform>()->position.z;
			root[object + to_string(i)]["Scale"]["x"] = it.second->GetComponent<Transform>()->scale.x;
			root[object + to_string(i)]["Scale"]["y"] = it.second->GetComponent<Transform>()->scale.y;
			if (FACTORY->ObjectIDMap[it.first]->GetComponent<Body>() != nullptr)
				root[object + to_string(i)]["Mass"] = it.second->GetComponent<Body>()->pm_mass;
			root[object + to_string(i)]["Rotation"] = it.second->GetComponent<Transform>()->angle;
	
			root[object + to_string(i)]["TextureDir"] = FACTORY->ObjectIDMap[it.first]->GetComponent<Sprite>()->mTexutureDir;*/
			i++;
		}


		file.WriteFile(path, root);

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
