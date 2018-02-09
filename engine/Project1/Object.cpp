/******************************************************************************/
/*!
\file   Object.cpp
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

Base Object class source file
*/
/******************************************************************************/


#include "Object.h"
#include "DebugUtil.h"

using namespace TE;

Object::Object()
{
	//Set all componetnt

	transform = nullptr;
	controller = nullptr;
	sprite = nullptr;
	body = nullptr;
	button = nullptr;
	trigger = nullptr;
	animation = nullptr;
	//particle = nullptr;
}
Object::~Object()
{

	//Todo : delete ALL componenets
	if (transform)
		delete transform;
	if(sprite)
		delete sprite;
	if (body)	
          delete body;
	if (controller)
		delete controller;
	if (button)
		delete button;
	if (trigger)
		delete trigger;
	if (animation)
		delete animation;
	/*if (particle)
		delete particle;*/
}	





void Object::Initialize()
{
	
	//Todo : Init all components
	if (transform)
		transform->Initialize();
	if (controller)
		controller->Initialize();
	if (sprite)
		sprite->Initialize();
	if (body)
		body->Initialize();
	if (button)
		button->Initialize();
	if (trigger)
		trigger->Initialize();
	if (animation)
		animation->Initialize();
	/*if (particle)
		particle->Initialize();*/
}

bool Object::AddComponent(Component * pComponent)
{
	if(pComponent->GetType() > CT_INVALID && pComponent->GetType() < CT_NUMCOMPONENT)
		pComponent->SetOwner(this);
	switch (pComponent->GetType())
	{
	case CT_TRANSFORM: 
		transform = dynamic_cast<Transform*>(pComponent);
		return true;
	case CT_CONTROLLER: 
		controller = dynamic_cast<Controller*>(pComponent);
		return true;
	case CT_SPRITE:
		sprite = dynamic_cast<Sprite*>(pComponent);
		return true;
	case CT_ANIMATION:
		animation = dynamic_cast<Animation *>(pComponent);
		return true;
	/*case CT_PARTICLE:
		particle = dynamic_cast<Animation *>(pComponent);
		return true;*/
	case CT_BODY:
		body = dynamic_cast<Body*>(pComponent);
		return true;
	case CT_BUTTON:
		button = dynamic_cast<Button*>(pComponent);
		return true;
	case CT_TRIGGER:
		trigger = dynamic_cast<Trigger*>(pComponent);
		return true;
		default:
		break;
	}
	//Add Comoponnt and return true if sucess
	return false;// if failed 
}

//return nullptr if fails
Component * Object::GetComponent(ComponentType cType)
{
	switch (cType)
	{
	case CT_TRANSFORM: return transform;
	case CT_CONTROLLER: return controller;
	case CT_SPRITE: return sprite;
	case CT_BODY: return body;
	case CT_BUTTON: return button;
	case CT_TRIGGER: return trigger;
	case CT_ANIMATION: return animation;
	//case CT_PARTICLE: return particle;
	default:
		break;
	}
	return nullptr;
}

//Signal the object factory for delay destruction
void Object::Destroy() {




}