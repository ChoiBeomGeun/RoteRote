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
			if (FACTORY->ObjectIDMap[gameObject->objID]->GetComponent(ComponentType::CT_BODY) != nullptr)
			{
				auto IDit2 = PHYSICS->m_Body.find(gameObject->body->BodyID);
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
			temp->AddComponent(new Body(glm::vec3(0, 0, 0), path.mass));
			temp->body->gravityOn = path.GravityOn;
			temp->body->restitution = 0;
		
			//'++;
		}
		if (path.HavingComponents[i] == ComponentType::CT_BUTTON)
		{
			Button * button = new Button();
			temp->AddComponent(button);
			temp->objectstyle = Objectstyle::Button;

		}
		if (path.HavingComponents[i] == ComponentType::CT_TRANSFORM)
		{
			Transform * transform = new Transform();
			transform->rotation = glm::vec3(0, 0, 1);
			transform->SetPosition(path.DefaultPosition);
			transform->SetScale(path.DefaultScale);
			transform->angle = path.rotation;
			temp->AddComponent(transform);




		}
		if (path.HavingComponents[i] == ComponentType::CT_SPRITE)
		{
			Sprite * sprite = new Sprite();
			sprite->depth = 0.0f;
			sprite->ChangeColor(255, 255, 255, 255);
			sprite->isPerspective = true;
			sprite->texture_load(path.DefaultTexture);
			
			temp->AddComponent(sprite);




		}
		if (path.HavingComponents[i] == ComponentType::CT_CONTROLLER)
		{
			//Controller * controller = new Controller();
			temp->objectstyle = Player;
			temp->AddComponent(new PlayerController());
			temp->body->gravityOn = true;
			temp->body->restitution = 0;
			//temp->AddComponent(controller);
		}

		if (path.HavingComponents[i] == ComponentType::CT_TRIGGER)
		{
			Trigger * trigger = new Trigger();
			if (path._180triggerornot)
				temp->objectstyle = Trigger180;
			else
				temp->objectstyle = Trigger90;

			temp->AddComponent(trigger);

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
	
	temp->Initialize();

	return temp;
}
Object * ObjectFactory::CreatePlayer(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& velocity, float invmass)
{
	Object* player = CreateEmptyObject();
	player->objectstyle = Objectstyle::Player;
	Transform * transform = new Transform();
	transform->SetPosition(pos);
	player->AddComponent(transform);
	player->transform->SetScale(scale);
	player->transform->rotation = glm::vec3(0, 0, 1);
	//Todo: add sprite and body components
	Sprite * sprite = new Sprite();

	//TOdo : load textrue and get texture id
	sprite->TextureId = 1;
	sprite->isPerspective = true;
	
	sprite->depth = 1.0f;
	
	//sprite->isFlipY = false;
	sprite->ChangeColor(255, 255, 255, 255);
	player->AddComponent(sprite);
	Animation * animation = new Animation();
	animation->setFlipX(false);
	animation->setTime(0.f);
	animation->isJumping = false;
	animation->setPressed(false);
	player->AddComponent(animation);

	
	player->AddComponent(new PlayerController());
	player->AddComponent(new Body(velocity, invmass));
	player->body->BodyID = BodyID;
	player->body->gravityOn = true;
	player->body->restitution = 0;
	player->Initialize();

	PHYSICS->m_Body[BodyID] = player->body;
	BodyID++;
	return player;
}

Object * ObjectFactory::CreateAsteroid(const glm::vec3 & pos, const glm::vec3& scale, const glm::vec3 & velocity, float invmass)
{
	Object* asteroid = CreateEmptyObject();
	asteroid->objectstyle = Objectstyle::Asteriod;
	Transform * transform = new Transform();
	transform->SetPosition(pos);
	asteroid->AddComponent(transform);
	asteroid->transform->SetScale(scale);
	asteroid->transform->rotation = glm::vec3(0, 0, 1);

	//Todo: add sprite and body components
	Sprite * sprite = new Sprite();

	//TOdo : load textrue and get texture id
	sprite->TextureId = 2;
	sprite->depth = 0.0f;
	sprite->ChangeColor(255, 255, 255, 255);
	sprite->isPerspective = true;
	//sprite->isFlipY = false;
	asteroid->AddComponent(sprite);

	asteroid->AddComponent(new Body(velocity, invmass));
	asteroid->body->gravityOn = true;
	asteroid->body->BodyID = BodyID;
	asteroid->Initialize();

	PHYSICS->m_Body[BodyID] = asteroid->body;

	BodyID++;
	return asteroid;

}

Object * ObjectFactory::CreateWall(const glm::vec3 & pos, const glm::vec3& scale)
{
	Object* Wall = CreateEmptyObject();
	Wall->objectstyle = Objectstyle::Wall;
	Transform * transform = new Transform();
	transform->SetPosition(pos);
	Wall->AddComponent(transform);
	Wall->transform->SetScale(scale);

	Wall->transform->rotation = glm::vec3(0, 0, 1);

	Wall->AddComponent(new Body(glm::vec3(0, 0, 0), 0));
	Wall->body->gravityOn = true;
	//Todo: add sprite and body components
	Sprite * sprite = new Sprite();
	sprite->depth = 0.0f;
	//sprite->isFlipY = false;
	sprite->texture_load("wall.png");
	//TOdo : load textrue and get texture id
	sprite->TextureId = 3;
	sprite->ChangeColor(255, 255, 255, 255);
	sprite->isPerspective = true;
	Wall->AddComponent(sprite);


	Wall->Initialize();
	BodyID++;
	return Wall;

}
Object * ObjectFactory::CreateTrigger(const glm::vec3 & pos, const glm::vec3& scale)
{
	Object* TriggerObject = CreateEmptyObject();

	Transform * transform = new Transform();
	Trigger * trigger = new Trigger();
	transform->SetPosition(pos);
	TriggerObject->AddComponent(transform);
	TriggerObject->AddComponent(trigger);
	TriggerObject->transform->SetScale(scale);

	TriggerObject->transform->rotation = glm::vec3(0, 0, 1);
	TriggerObject->AddComponent(new Body(glm::vec3(0, 0, 0), 0));
	TriggerObject->body->gravityOn = true;
	//Todo: add sprite and body components
	Sprite * sprite = new Sprite();
	sprite->depth = 0.0f;
	//sprite->isFlipY = false;
	sprite->texture_load("180trigger.png");
	//TOdo : load textrue and get texture id
	sprite->TextureId = 3;
	sprite->ChangeColor(255, 255, 255, 255);
	sprite->isPerspective = true;
	TriggerObject->AddComponent(sprite);


	TriggerObject->Initialize();
	BodyID++;
	return TriggerObject;

}
Object *  ObjectFactory::GetPlayer(void)
{
	Object *temp = nullptr;
	for (auto Objects : FACTORY->ObjectIDMap)
	{
		if (Objects.second->GetComponent(ComponentType::CT_CONTROLLER) != nullptr)
			temp = Objects.second;

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
Object * ObjectFactory::CreateButton(const glm::vec3 & pos, const glm::vec3& scale)
{
	Object* mbutton = CreateEmptyObject();

	Transform * transform = new Transform();
	Button * button = new Button();
	transform->position = pos;
	mbutton->AddComponent(transform);
	mbutton->AddComponent(button);
	mbutton->transform->scale = scale;
	mbutton->transform->rotation = glm::vec3(0, 0, 1);
	mbutton->objectstyle = Objectstyle::Button;
	//Todo: add sprite and body components
	Sprite * sprite = new Sprite();
	//sprite->isFlipY = false;
	//TOdo : load textrue and get texture id
	//sprite->TextureId = 4;
	sprite->depth = 0.0f;
	sprite->ChangeColor(255, 255, 255, 255);
	sprite->isPerspective = true;
	mbutton->AddComponent(sprite);


	mbutton->Initialize();

	return mbutton;

}

Object * ObjectFactory::CreateBox(const glm::vec3 & pos, const glm::vec3 & scale)
{
	Object* Box = CreateEmptyObject();
	Box->objectstyle = Objectstyle::Box;
	Transform * transform = new Transform();
	transform->SetPosition(pos);
	Box->AddComponent(transform);
	Box->transform->SetScale(scale);
	Box->transform->rotation = glm::vec3(0, 0, 1);
	Box->AddComponent(new Body(glm::vec3(0, 0, 0), 1));
	Box->body->gravityOn = false;
	Box->body->restitution = 0;
	//Todo: add sprite and body components
	Sprite * sprite = new Sprite();

	//TOdo : load textrue and get texture id
	//sprite->TextureId = 5;
	sprite->depth = 0.0f;
	//sprite->isFlipY = false;
	sprite->ChangeColor(255, 255, 255, 255);
	sprite->isPerspective = true;
	Box->AddComponent(sprite);


	Box->Initialize();
	BodyID++;
	return Box;
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
	Transform * transform = new Transform();
	transform->SetPosition(pos);
	Hud->AddComponent(transform);
	Hud->transform->SetScale(scale);
	Hud->transform->rotation = glm::vec3(0, 0, 1);

	//Todo: add sprite and body components
	Sprite * sprite = new Sprite();

	//TOdo : load textrue and get texture id
	sprite->isPerspective = false;
	//sprite->isFlipY = false;
	sprite->depth = 1.0f;
	sprite->TextureId = 5;
	sprite->ChangeColor(255, 255, 255, 255);
	Hud->AddComponent(sprite);


	Hud->Initialize();
	return Hud;
}
Object * TE::ObjectFactory::LeftBoundary(void)
{
	std::map<ObjectID, Object*>::iterator max;
	auto Objects = FACTORY->ObjectIDMap;
	auto min = FACTORY->ObjectIDMap.begin()->second->transform->position.x;
	auto test = std::find_if(FACTORY->ObjectIDMap.begin(), FACTORY->ObjectIDMap.end(),
		[&](std::pair<ObjectID, Object*> t)
	{
		if (t.second->objectstyle == Objectstyle::Wall)
		{
			if (t.second->transform->position.x < min)
				min = t.second->transform->position.x;
		}
		return min;
	}
	);
	return test->second;
}
Object * TE::ObjectFactory::RightBoundary(void)
{
	auto Objects = FACTORY->ObjectIDMap;
	auto max = FACTORY->ObjectIDMap.begin()->second->transform->position.x;
	auto test = std::find_if(FACTORY->ObjectIDMap.begin(), FACTORY->ObjectIDMap.end(),
		[&](std::pair<ObjectID, Object*> t)
	{
		if (t.second->objectstyle == Objectstyle::Wall)
		{
			if (t.second->transform->position.x > max)
				max = t.second->transform->position.x;
		}
		return max;
	});

	return test->second;
}
Object * TE::ObjectFactory::UpBoundary(void)
{
	auto Objects = FACTORY->ObjectIDMap;
	auto max = FACTORY->ObjectIDMap.begin()->second->transform->position.y;
	auto test = std::find_if(FACTORY->ObjectIDMap.begin(), FACTORY->ObjectIDMap.end(),
		[&](std::pair<ObjectID, Object*> t)
	{
		if (t.second->objectstyle == Objectstyle::Wall)
		{
			if (t.second->transform->position.y > max)
				max = t.second->transform->position.y;
		}
		return max;
	});

	return test->second;
}
Object * TE::ObjectFactory::DownBoundary(void)
{
	std::map<ObjectID, Object*>::iterator max;
	auto min = FACTORY->ObjectIDMap.begin()->second->transform->position.y;
	auto Objects = FACTORY->ObjectIDMap.begin();
	for (Objects; Objects != FACTORY->ObjectIDMap.end(); ++Objects)
	{
		if ((Objects)->second->objectstyle == Objectstyle::Wall)
		{
			if (Objects->second->transform->position.y < min)
			{
				max = Objects;
			}

		}


	}

	return max->second;
}