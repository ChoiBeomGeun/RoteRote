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



#include "Factory.h"
#include "DebugUtil.h"
#include "..\..\example\examples\PlayerController.h"
#include "Sprite.h"
#include "Body.h"
#include <iterator>
#include "Graphics.h"
#include  "Button.h"
#include "Application.h"
#include "TriggerC.h"
#include "Animation.h"
#include <algorithm>

namespace TE {

	ObjectFactory * FACTORY = nullptr;
}

using namespace TE;

ObjectFactory::ObjectFactory()
{
	DEBUG_ASSERT(FACTORY != nullptr, "Factory already created! There can only be 1");
	FACTORY = this;
	LastObjectID = 0; // There is over 4 billion numbers
	BodyID = 1;

}

ObjectFactory::~ObjectFactory()
{



}

void ObjectFactory::Initialize()
{


}

void ObjectFactory::Update(float /*dt*/)
{
	// (FACTORY->ObjectIDMap[i.first]->GetComponent(ComponentType::CT_TRANSFORM) != nullptr)

      	GamePlayer = FACTORY->GetPlayer();

	//Todo : Delete All object in the objectTobedeleted list

	std::vector<Object*>::iterator it = ObjectsToBeDeleted.begin();

	for ( it = ObjectsToBeDeleted.begin(); it != ObjectsToBeDeleted.end(); ++it)
	{
		Object * gameObject = *it;

		//std::map<ObjectID, Object*>::iterator IDit = ObjectIDMap.find(gameObject->objID);
		auto IDit = ObjectIDMap.find(gameObject->objID);


	//	DEBUG_ASSERT(IDit == ObjectIDMap.end(), "Object is bad memory");

		if (IDit != ObjectIDMap.end())
		{
			//delete and remove from the map
			if (FACTORY->ObjectIDMap[gameObject->objID]->HasComponent<Body>())
			{
				auto IDit2 = PHYSICS->m_Body.find(gameObject->GetComponent<Body>()->BodyID);
				PHYSICS->m_Body.erase(IDit2);
			}
		

			delete gameObject;
			ObjectIDMap.erase(IDit);
		}
	}




	ObjectsToBeDeleted.clear();
}

void ObjectFactory::Destroy(Object* gameObject)
{

	ObjectsToBeDeleted.push_back(gameObject);

}

void ObjectFactory::DestroyAllObjects()
{
	//std::map<ObjectID, Object* >::iterator it = ObjectIDMap.begin();
 	for ( auto it : ObjectIDMap)
	{
		if (it.second->IsLoadingObject)
			continue;
		delete it.second;
	}
	BodyID = 0;
	LastObjectID = 0;
	ObjectIDMap.clear();
	PHYSICS->m_Body.clear();
}


void ObjectFactory::IDGameObject(Object * gameObject)
{
	//Just increment the lastObjectID used. Does not handdle over flow
	//but it would take over 4 billion object to break;
	++LastObjectID;


	gameObject->objID = LastObjectID;

	//store the game Object in the ObjectIDMAp;
	ObjectIDMap[LastObjectID] = gameObject;
}

int ObjectFactory::GetObjectID(int objectID)
{

	return ObjectIDMap[objectID]->objID;
}

Object * ObjectFactory::GetObjectwID(ObjectID id)
{
	std::map<ObjectID, Object*>::iterator it = ObjectIDMap.find(id);
	if (it != ObjectIDMap.end())
		return it->second;

	return nullptr;

}

Object * ObjectFactory::CreateEmptyObject()
{
	Object*gameObject = new Object();
	IDGameObject(gameObject);
	return gameObject;
}
Object * ObjectFactory::CreateArchetype(Archetype path)
{
	Object * temp = CreateEmptyObject();
	//temp->objectstyle = Wall;


	for (unsigned i = 0; i < path.HavingComponents.size(); i++)
	{
		if (path.HavingComponents[i] == ComponentType::CT_BODY)
		{
			temp->AddComponent<Body>();
		
			temp->GetComponent<Body>()->pm_mass = path.mass;
			temp->GetComponent<Body>()->pm_velocity = glm::vec3(0, 0, 0);

			temp->GetComponent<Body>()->gravityOn = path.GravityOn;
			temp->GetComponent<Body>()->restitution = 0;
		
			//'++;
		}
		if (path.HavingComponents[i] == ComponentType::CT_BUTTON)
		{
	//		Button * button = new Button();
			temp->AddComponent<Button>();
			temp->objectstyle = Objectstyle::Button;

		}
		if (path.HavingComponents[i] == ComponentType::CT_TRANSFORM)
		{
			temp->AddComponent<Transform>();
		//	Transform * transform = new Transform();
			temp->GetComponent<Transform>()->rotation = glm::vec3(0, 0, 1);
			temp->GetComponent<Transform>()->SetPosition(path.DefaultPosition);
			temp->GetComponent<Transform>()->SetScale(path.DefaultScale);
			temp->GetComponent<Transform>()->SetRotationZ(path.rotation)  ;
	




		}
		if (path.HavingComponents[i] == ComponentType::CT_SPRITE)
		{
	//		Sprite * sprite = new Sprite();
			temp->AddComponent<Sprite>();
			temp->GetComponent<Sprite>()->depth = 0.0f;
			temp->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
			temp->GetComponent<Sprite>()->isPerspective = true;
			temp->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id(path.DefaultTexture);
			
	




		}
		if (path.HavingComponents[i] == ComponentType::CT_CONTROLLER)
		{
			temp->AddComponent<PlayerController>();
			//Controller * controller = new Controller();
			temp->objectstyle = Player;

			temp->GetComponent<Body>()->gravityOn = true;
			temp->GetComponent<Body>()->restitution = 0;
			//temp->AddComponent(controller);
		}

		if (path.HavingComponents[i] == ComponentType::CT_TRIGGER)
		{
			Trigger * trigger = new Trigger();
			if (path._180triggerornot)
				temp->objectstyle = Trigger180;
			else
				temp->objectstyle = Trigger90;

			temp->AddComponent<Trigger>();

		}

		if (path.HavingComponents[i] == ComponentType::CT_EMITTER)
		{
			Emitter * emitter = new Emitter();
	
			temp->AddComponent<Emitter>();

		}

	}

	if (path.DefaultObjectStyle == "Wall")
		temp->objectstyle = Objectstyle::Wall;
	if (path.DefaultObjectStyle == "Box")
		temp->objectstyle = Objectstyle::Box;
	if (path.DefaultObjectStyle == "Player")
		temp->objectstyle = Objectstyle::Player;
	if (path.DefaultObjectStyle == "Button")
		temp->objectstyle = Objectstyle::Button;
	if (path.DefaultObjectStyle == "Trigger180")
		temp->objectstyle = Objectstyle::Trigger180;
	if (path.DefaultObjectStyle == "Trigger90")
		temp->objectstyle = Objectstyle::Trigger90;
	
	if (path.DefaultObjectStyle == "Hazard")
		temp->objectstyle = Objectstyle::Hazard;
	if (path.DefaultObjectStyle == "Particle")
		temp->objectstyle = Objectstyle::Particle;
	temp->Initialize();

	return temp;
}




Object * ObjectFactory::CreateWall(const glm::vec3 & pos, const glm::vec3& scale)
{
	Object* Wall = CreateEmptyObject();
	Wall->objectstyle = Objectstyle::Wall;
//	Transform * transform = new Transform();
	Wall->GetComponent<Transform>()->SetPosition(pos);
	Wall->AddComponent<Transform>();
	Wall->GetComponent<Transform>()->SetScale(scale);

	Wall->GetComponent<Transform>()->rotation = glm::vec3(0, 0, 1);
	Wall->AddComponent<Body>();
	Wall->GetComponent<Body>()->pm_velocity = glm::vec3(0, 0, 0);
	Wall->GetComponent<Body>()->pm_mass = 0;
	//Wall->AddComponent(new Body(glm::vec3(0, 0, 0), 0));
	Wall->GetComponent<Body>()->gravityOn = true;
	//Todo: add sprite and body components

	Wall->AddComponent<Sprite>();
	Wall->GetComponent<Sprite>()->depth = 0.0f;
	//sprite->isFlipY = false;
	Wall->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("wall.png");
	//TOdo : load textrue and get texture id
	Wall->GetComponent<Sprite>()->m_TextureID = 3;
	Wall->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
	Wall->GetComponent<Sprite>()->isPerspective = true;

//	Wall->AddComponent(sprite);


	Wall->Initialize();
	BodyID++;
	return Wall;

}

Object *  ObjectFactory::GetPlayer(void)
{
	Object *temp = nullptr;
	for (auto Objects : FACTORY->ObjectIDMap)
	{
		if (Objects.second->HasComponent<PlayerController>() )
			return Objects.second;

	}
 	return temp;
}
unsigned int TE::ObjectFactory::GetAllObjectNumbers(void)
{
	unsigned int ObjectCount = 0;
	for (auto Objects : FACTORY->ObjectIDMap)
	{
		ObjectCount++;
	}
	return ObjectCount;
}



void ObjectFactory::CreateBoundary(void)
{
	CreateWall(glm::vec3(0, static_cast<float>(-APP->_screenHeight/2), 0), glm::vec3(static_cast<float>(APP->_screenWidth), 50.f, 0));
	CreateWall(glm::vec3(0, static_cast<float>(APP->_screenHeight/2), 0), glm::vec3(static_cast<float>(APP->_screenWidth), 50.f, 0));
	CreateWall(glm::vec3(static_cast<float>(-APP->_screenWidth/2), 0, 0), glm::vec3(50.f, static_cast<float>(APP->_screenHeight) - 50.f, 0));
	CreateWall(glm::vec3(static_cast<float>(APP->_screenWidth/2), 0, 0), glm::vec3(50.f, static_cast<float>(APP->_screenHeight) - 50.f, 0));
}
Object * TE::ObjectFactory::CreateHUD(const glm::vec3 & pos, const glm::vec3 & scale)
{
	Object* Hud = CreateEmptyObject();
	
	Hud->objectstyle = Objectstyle::Button;
//	Transform * transform = new Transform();
	Hud->AddComponent<Transform>();
	Hud->GetComponent<Transform>()->SetPosition(pos);
	
	
	Hud->GetComponent<Transform>()->SetScale(scale);
	Hud->GetComponent<Transform>()->rotation = glm::vec3(0, 0, 1);

	Hud->AddComponent<Sprite>();
	Hud->GetComponent<Sprite>()->isPerspective = false;
	//sprite->isFlipY = false;
	Hud->GetComponent<Sprite>()->depth = 1.0f;
	Hud->GetComponent<Sprite>()->m_TextureID = 5;
	Hud->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);

//	Hud->AddComponent(sprite);


	Hud->Initialize();
	return Hud;
}
Object * TE::ObjectFactory::LeftBoundary(void)
{
	std::vector<Object*> WallObject;
	for (auto walls : FACTORY->ObjectIDMap) {
		
		if (walls.second->objectstyle == Objectstyle::Wall)
		{
			WallObject.push_back(walls.second);
		}
	}
	Object * minObject = WallObject[0];
	for (int i = 1; i < WallObject.size(); i++)
	{
		if (minObject->GetComponent<Transform>()->position.x > WallObject[i]->GetComponent<Transform>()->position.x)
			minObject = WallObject[i];
	}
	return minObject;
}
Object * TE::ObjectFactory::RightBoundary(void)
{
	std::vector<Object*> WallObject;
	for (auto walls : FACTORY->ObjectIDMap) {

		if (walls.second->objectstyle == Objectstyle::Wall)
		{
			WallObject.push_back(walls.second);
		}
	}
	Object * maxObject = WallObject[0];
	for (int i = 1; i < WallObject.size(); i++)
	{
		if (maxObject->GetComponent<Transform>()->position.x < WallObject[i]->GetComponent<Transform>()->position.x)
			maxObject = WallObject[i];
	}
	return maxObject;
}
Object * TE::ObjectFactory::UpBoundary(void)
{
	std::vector<Object*> WallObject;
	for (auto walls : FACTORY->ObjectIDMap) {

		if (walls.second->objectstyle == Objectstyle::Wall)
		{
			WallObject.push_back(walls.second);
		}
	}
	Object * maxObject = WallObject[0];
	for (int i = 1; i < WallObject.size(); i++)
	{
		if (maxObject->GetComponent<Transform>()->position.y < WallObject[i]->GetComponent<Transform>()->position.y)
			maxObject = WallObject[i];
	}
	return maxObject;
}
Object * TE::ObjectFactory::DownBoundary(void)
{
	std::vector<Object*> WallObject;
	for (auto walls : FACTORY->ObjectIDMap) {

		if (walls.second->objectstyle == Objectstyle::Wall)
		{
			WallObject.push_back(walls.second);
		}
	}
	Object * minObject = WallObject[0];
	for (int i = 1; i < WallObject.size(); i++)
	{
		if (minObject->GetComponent<Transform>()->position.y > WallObject[i]->GetComponent<Transform>()->position.y)
			minObject = WallObject[i];
	}
	return minObject;
}