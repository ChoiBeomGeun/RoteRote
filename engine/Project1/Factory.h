/******************************************************************************/
/*!
\file   Factory.cpp
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

OvjectFactory handles the creation , managment , and
destruction of all Objects
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once
#include "System.h"
#include "Object.h"
#include <map>
#include <glm/glm.hpp>
#include "ArchetypeEditor.h"

namespace TE {

	class ObjectFactory : public Systems {
	public:


		ObjectFactory();
		~ObjectFactory();
		
		void Initialize() override;

		void Update(float dt) override;
		//Add an object to the destroy list for delay destruction
		void Destroy(Object *);

		//For the shutdown the Factory
		void DestroyAllObjects();
		//ID the Object and store it in the object list(map)
		void IDGameObject(Object* gameObject);
		int GetObjectID(int objectID);
		//Return an object with specific ID
		Object* GetObjectwID (ObjectID id);
		Object * CreateWall(const glm::vec3 & pos, const glm::vec3 & scale);
		Object * ObjectFactory::CreateButton(const glm::vec3 & pos, const glm::vec3& scale);
		Object * ObjectFactory::CreateBox(const glm::vec3 & pos, const glm::vec3 & scale);
		//Object * CreateCamera(const glm::vec3 & eye, const glm::vec3 & center, const glm::vec3 & up, const float angle);
		Object * CreateEmptyObject();
		//Showing how to create an object through hardcode
		//Todo : when applying data driven methods, use this as blueprint
		void CreateBoundary(void);
		Object * CreatePlayer (const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& velocity, float invmass);
		Object * CreateAsteroid(const glm::vec3& pos, const glm::vec3& scale,const glm::vec3& velocity, float invmass);
		Object * CreateTrigger(const glm::vec3 & pos, const glm::vec3& scale); 
		Object * CreateArchetype(Archetype path);
		Object * TE::ObjectFactory::CreateHUD(const glm::vec3 & pos, const glm::vec3 & scale);
		//used to incremetallly  generate unique ID
		ObjectID LastObjectID;
		ObjectID BodyID;
		Object * GetPlayer(void);
		Object * LeftBoundary(void);
		Object * RightBoundary(void);
		Object * UpBoundary(void);
		Object * DownBoundary(void);
		unsigned int GetAllObjectNumbers(void);
		//Map of Objects to their IDs used for safe referencing
		std::map<ObjectID, Object*> ObjectIDMap;
                std::vector<unsigned int> BodyNum;
		//List of objects to be deleted
		//To prevent problems of early or multi call of object's destruction
		std::vector<Object *> ObjectsToBeDeleted;
		
	};
	extern ObjectFactory * FACTORY;

}