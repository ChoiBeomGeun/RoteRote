/******************************************************************************/
/*!
\file   Object.cpp
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

Base Object class header file
Contains definition for the object class which each and every
entities in the game engine will be built upon.
*/
/******************************************************************************/





#pragma once
#include <vector>
#include "Component.h"
#include "Transform.h"
#include "ComponentType.h"
#include "Controller.h"
#include "Sprite.h"
#include "Body.h"
#include "Button.h"
#include "ObjectStyle.h"
#include "Camera.h"
#include  "TriggerC.h"
#include "Animation.h"
#include "Particle.h"


namespace TE {
	typedef unsigned int ObjectID;
	
	class Object
	{

	public:
		//Todo : friend class to ObjectFactory 
		friend class ObjectFactory;
		Object();
		~Object();
		//Init the object
		void Initialize();

		// Properly destroy the object by delay destruction
		void Destroy();

		bool AddComponent(Component * pComponent);
	Component* GetComponent(ComponentType cType);
	ObjectID objID;	
	Objectstyle objectstyle;
		//std::vector<Component*> pCompList;
		//ALL different types of pointers to different components
		//but set to null ptr if component doesn't exist.
		//Transform* transform
		//Sprite * sprite
		//Body * body
		//Logic * logic
		Sprite * sprite;
		Animation * animation;
		Transform * transform;
		Controller * controller;
		Body * body;
		Button * button;
		Trigger * trigger;
		/*Particle * particle;*/

		char * objName = "default";
	};

}