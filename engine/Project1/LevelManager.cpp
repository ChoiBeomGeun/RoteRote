/******************************************************************************/
/*!
\file   LevelManager.cpp
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

Using jsoncpp, loading and saving the level
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "LevelManager.h"
#include "json/json.h"
#include "json/JsonClass.h"
#include "Factory.h"
#include "Object.h"
#include "DebugUtil.h"
#include  "StateManager.h"
#include  "stdlib.h"
#include "camera.h"
#include "Particles.h"
#include "..\..\example\examples\PlayerController.h"
#include "ParticleManager.h"
#define JSON_FILE "jsonFile.json"
#define JSON_FILE_WRITE "jsonFileWrite.json"
using namespace TE;
using namespace  std;

namespace TE {


	LevelManager * LEVELMANAGER = nullptr;


}


void LevelManager::LoadLevel(std::string  path)
{
	char * Userinfo;
	size_t len = path.size();
	_dupenv_s(&Userinfo, &len, "USERPROFILE");
	


	
	std::string saveLevel = path;
#ifdef _DEBUG
	path = ".\\levels.\\" + path;
#else
	path = Userinfo;
	path += "/Documents/RoteRote/levels/" + saveLevel;
#endif
	free(Userinfo);

	//FACTORY->DestroyAllObjects();

	STATEMANAGER->Loadtolevelname = saveLevel;
	Jsonclass file;
	std::string object = "Object";
	
	//	char *path = (char*)JSON_FILE;
	file.ReadFile(path);


	for (int i = 1; i< file.mRoot.get("NumberOfObjects", false).asInt() + 1; i++)
	{
		std::string Objectstyle = file.mRoot.get(object + to_string(i), false).get("ObjectType", false).asString();


		Object *tempObject = FACTORY->CreateEmptyObject();
	//	int objectID = file.mRoot.get(object + to_string(i), false).get("ObjectID", false).asInt();
 		

		bool GravityOn = file.mRoot.get(object + to_string(i), false).get("GravityOn", false).asBool();
		float Xpos = file.mRoot.get(object + to_string(i), false).get("Position", false).get("x", false).asFloat();
		float Ypos = file.mRoot.get(object + to_string(i), false).get("Position", false).get("y", false).asFloat();
//		float Zpos = file.mRoot.get(object + to_string(i), false).get("Position", false).get("z", false).asFloat();
		float Rotation = file.mRoot.get(object + to_string(i), false).get("Rotation", false).asFloat();
		float Xscale = file.mRoot.get(object + to_string(i), false).get("Scale", false).get("x", false).asFloat();
		float Yscale = file.mRoot.get(object + to_string(i), false).get("Scale", false).get("y", false).asFloat();
		std::string textureDir = file.mRoot.get(object + to_string(i), false).get("TextureDir", false).asString();
		CAMERA->cameraPos.x = file.mRoot.get("DefalutCamera", false).get("EYE", false).get("x", false).asFloat();
		CAMERA->cameraPos.y = file.mRoot.get("DefalutCamera", false).get("EYE", false).get("y", false).asFloat();
		CAMERA->cameraPos.z = file.mRoot.get("DefalutCamera", false).get("EYE", false).get("z", false).asFloat();
		CAMERA->cameraTarget.x = file.mRoot.get("DefalutCamera", false).get("TARGET", false).get("x", false).asFloat();
		CAMERA->cameraTarget.y = file.mRoot.get("DefalutCamera", false).get("TARGET", false).get("y", false).asFloat();
		CAMERA->cameraTarget.z = file.mRoot.get("DefalutCamera", false).get("TARGET", false).get("z", false).asFloat();
		CAMERA->cameraUp.x = file.mRoot.get("DefalutCamera", false).get("UP", false).get("x", false).asFloat();
		CAMERA->cameraUp.y = file.mRoot.get("DefalutCamera", false).get("UP", false).get("y", false).asFloat();
		CAMERA->cameraUp.z = file.mRoot.get("DefalutCamera", false).get("UP", false).get("z", false).asFloat();
		float mass = file.mRoot.get(object + to_string(i), false).get("Mass", false).asFloat();
		
		STATEMANAGER->v_StatesLists[STATEMANAGER->i_CurrentStateNumber]->WorldSizeX = file.mRoot.get("WorldSize", false).get("x", false).asInt();
		STATEMANAGER->v_StatesLists[STATEMANAGER->i_CurrentStateNumber]->WorldSizeY = file.mRoot.get("WorldSize", false).get("y", false).asInt();
		
		
		for (unsigned int indexC = 0; ;indexC++) {
			if (file.mRoot[object + to_string(i)]["Components"][indexC].asString() == "TRANSFORM") {
			//	Transform * transform = new Transform();
				tempObject->AddComponent<Transform>();
			//	tempObject->GetComponent<Transform>()->position.x
				tempObject->GetComponent<Transform>()->position.x = Xpos;
				tempObject->GetComponent<Transform>()->position.y = Ypos;
				tempObject->GetComponent<Transform>()->scale.x = Xscale;
				tempObject->GetComponent<Transform>()->scale.y = Yscale;
				tempObject->GetComponent<Transform>()->angle = Rotation;
				tempObject->GetComponent<Transform>()->rotation = glm::vec3(0, 0, 1);
			
			//	tempObject->AddComponent(transform);
			}
			if (file.mRoot[object + to_string(i)]["Components"][indexC].asString() == "SPRITE") {
			//	Sprite * sprite = new Sprite();
				tempObject->AddComponent<Sprite>();
				tempObject->GetComponent<Sprite>()->depth = 0.0f;
				tempObject->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
				tempObject->GetComponent<Sprite>()->isPerspective = true;
				tempObject->GetComponent<Sprite>()->texture_load(textureDir);
			//	tempObject->AddComponent(sprite);
				
				
			}
			if (file.mRoot[object + to_string(i)]["Components"][indexC].asString() == "CONTROLLER") {
				tempObject->AddComponent<PlayerController>();
			}
			if (file.mRoot[object + to_string(i)]["Components"][indexC].asString() == "BODY") {
		//		Body * body = new Body(glm::vec3(0, 0, 0), mass);
 				tempObject->AddComponent<Body>();
				tempObject->GetComponent<Body>()->gravityOn = GravityOn;
				tempObject->GetComponent<Body>()->pm_invmass = mass;
				tempObject->GetComponent<Body>()->restitution = 0;
			}
	
			if (file.mRoot[object + to_string(i)]["Components"][indexC].asString() == "BUTTON") {
				tempObject->objectstyle = Objectstyle::Button;
			

			}
			if (file.mRoot[object + to_string(i)]["Components"][indexC].asString() == "ANIMATION") {
				//Animation * GetComponent<Animation>()= new Animation();

				tempObject->AddComponent<Animation>();
				tempObject->GetComponent<Animation>()->setFlipX(false);
				tempObject->GetComponent<Animation>()->setTime(0.f);
				tempObject->GetComponent<Animation>()->isJumping = false;
				tempObject->GetComponent<Animation>()->setPressed(false);

			}
			if (file.mRoot[object + to_string(i)]["Components"][indexC].asString() == "TRIGGER") {
	//			Trigger * trigger = new Trigger();

				tempObject->AddComponent<Trigger>();
				if (Objectstyle == "Trigger180") {
					tempObject->GetComponent<Trigger>()->TriggerType = "180";
					tempObject->objectstyle = Objectstyle::Trigger180;

				}

				if (Objectstyle == "Trigger90") {
					tempObject->GetComponent<Trigger>()->TriggerType = "90";
					tempObject->objectstyle = Objectstyle::Trigger90;
				}
			}

			if (file.mRoot[object + to_string(i)]["Components"][indexC].asString() == "PARTICLE") {

				if (STATEMANAGER->Loadtolevelname == "level1.json" || STATEMANAGER->Loadtolevelname == "level2.json" || STATEMANAGER->Loadtolevelname == "level3.json"
					|| STATEMANAGER->Loadtolevelname == "level4.json" || STATEMANAGER->Loadtolevelname == "level5.json" || STATEMANAGER->Loadtolevelname == "leve6.json"
					|| STATEMANAGER->Loadtolevelname == "level7.json" || STATEMANAGER->Loadtolevelname == "level8.json" || STATEMANAGER->Loadtolevelname == "level9.json"
					|| STATEMANAGER->Loadtolevelname == "level10.json" || STATEMANAGER->Loadtolevelname == "level11.json" || STATEMANAGER->Loadtolevelname == "level12.json")
				{
					tempObject = PARTICLEMANAGER->LoadEmitter(tempObject,"particle1.json");

					/*tempObject->AddComponent<Emitter>();
					tempObject->GetComponent<Sprite>()->texture_load("test.png");
					tempObject->GetComponent<Transform>()->position = tempObject->GetComponent<Transform>()->position;
					tempObject->GetComponent<Emitter>()->SetEmitter(tempObject->GetComponent<Transform>()->position, glm::vec3(0.0f), 0, 400, 20.f, ET_TRAIL);
					tempObject->GetComponent<Emitter>()->SetTexture(tempObject->GetComponent<Sprite>()->TextureId);
					tempObject->GetComponent<Emitter>()->CreateParticle();
					tempObject->GetComponent<Transform>()->scale = glm::vec3(10.0f);

					PARTICLEMANAGER->AddEmitter(tempObject->GetComponent<Emitter>());*/

					
				}
				else if (STATEMANAGER->Loadtolevelname == "Menu.json")
				{
					tempObject->AddComponent<Emitter>();
					tempObject->GetComponent<Sprite>()->texture_load("test.png");
					tempObject->GetComponent<Transform>()->position = tempObject->GetComponent<Transform>()->position;
					tempObject->GetComponent<Emitter>()->SetEmitter(tempObject->GetComponent<Transform>()->position, glm::vec3(0.0f), 0, 400, 20.f, ET_EXPLOSION);
					tempObject->GetComponent<Emitter>()->SetTexture(tempObject->GetComponent<Sprite>()->TextureId);
					tempObject->GetComponent<Emitter>()->CreateParticle();
					tempObject->GetComponent<Transform>()->scale = glm::vec3(10.0f);

					PARTICLEMANAGER->AddEmitter(tempObject->GetComponent<Emitter>());
				}
				
			}
			if (file.mRoot[object + to_string(i)]["Components"][indexC].asString() == "")
				break;
		}
		
		if (Objectstyle == "Player")
		{
			//tempObject = FACTORY->CreatePlayer(glm::vec3(Xpos, Ypos, Zpos), glm::vec3(Xscale, Yscale, 0), glm::vec3(0, 0, 0), mass);
			tempObject->objectstyle = Objectstyle::Player;
  			//tempObject = FACTORY->CreateArchetype(ReadingArchetype("Player.json"));
		}
		else if (Objectstyle == "Asteriod")
		{
			//tempObject = FACTORY->CreateAsteroid(glm::vec3(Xpos, Ypos, Zpos), glm::vec3(Xscale, Yscale, 0), glm::vec3(0, 0, 0), mass);
			tempObject->objectstyle = Objectstyle::Asteriod;
		}
		else if (Objectstyle == "Wall")
		{
			//tempObject = FACTORY->CreateWall(glm::vec3(Xpos, Ypos, Zpos), glm::vec3(Xscale, Yscale, 0));
			tempObject->objectstyle = Objectstyle::Wall;
		}
		else if (Objectstyle == "Button")
		{

			if(tempObject->GetComponent<Sprite>() !=NULL)
			tempObject->GetComponent<Sprite>()->depth = -1;
			//tempObject = FACTORY->CreateButton(glm::vec3(Xpos, Ypos, Zpos), glm::vec3(Xscale, Yscale, 0));
			tempObject->objectstyle = Objectstyle::Button;
		}
		else if (Objectstyle == "Box")
		{
			//tempObject = FACTORY->CreateBox(glm::vec3(Xpos, Ypos, Zpos), glm::vec3(Xscale, Yscale, 0));
			tempObject->objectstyle = Objectstyle::Box;
		}
		else if (Objectstyle == "Trigger90")
		{
			//tempObject = FACTORY->CreateTrigger(glm::vec3(Xpos, Ypos, Zpos), glm::vec3(Xscale, Yscale, 0));
			tempObject->objectstyle = Objectstyle::Trigger90;
		}
		else if (Objectstyle == "Trigger180")
		{
			//tempObject = FACTORY->CreateTrigger(glm::vec3(Xpos, Ypos, Zpos), glm::vec3(Xscale, Yscale, 0));
			tempObject->objectstyle = Objectstyle::Trigger180;
		}
		else if (Objectstyle == "Clearzone")
		{
			//tempObject = FACTORY->CreateButton(glm::vec3(Xpos, Ypos, Zpos), glm::vec3(Xscale, Yscale, 0));
			tempObject->objectstyle = Objectstyle::Clearzone;
		}
		else if (Objectstyle == "Particle")
		{
			tempObject->objectstyle = Objectstyle::Particle;
		}
		else
			DEBUG_ASSERT(true, "Invaild Object Style");

		if (tempObject->GetComponent<Sprite>())
		{
			FACTORY->ObjectIDMap[tempObject->objID]->GetComponent<Sprite>()->mTexutureDir = (const_cast<char*>(textureDir.c_str()));
			tempObject->GetComponent<Sprite>()->texture_load(const_cast<char*>(textureDir.c_str()));
		}
	
		tempObject->Initialize();
 	}

  }

void LevelManager::SaveLevel(std::string  path)
{
	char * Userinfo;
	size_t len = path.size();
	_dupenv_s(&Userinfo, &len, "USERPROFILE");
	
	std::string saveLevel = path;
#ifdef _DEBUG
	path = ".\\levels.\\" + path;
#else
	path = Userinfo;
	path += "/Documents/RoteRote/levels/" + saveLevel;
#endif
	free(Userinfo);
	Jsonclass file;
	Json::Value root;
	std::string object = "Object";


	root["Level"] = 1;
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
		it.second->GetComponent<Transform>()->position.z = 0;
		static unsigned index = 0;

		if (it.second->GetComponent<Transform>() != nullptr) {
			root[object + to_string(i)]["Components"][index] = "TRANSFORM";
			index++;
		}
		if (it.second->GetComponent<Sprite>() != nullptr) {
			root[object + to_string(i)]["Components"][index] = "SPRITE";
			index++;
		}
		if (it.second->GetComponent<Body>() != nullptr) {
			root[object + to_string(i)]["Components"][index] = "BODY";

			if(it.second->objectstyle == Objectstyle::Box)
				it.second->GetComponent<Body>()->gravityOn = false;
			root[object + to_string(i)]["GravityOn"] = it.second->GetComponent<Body>()->gravityOn;
			index++;
		}
		if (it.second->GetComponent<Trigger>() != nullptr) {
			root[object + to_string(i)]["Components"][index] = "TRIGGER";
			root[object + to_string(i)]["TriggerType"] = it.second->GetComponent<Trigger>()->TriggerType;
			index++;
		}
		if (it.second->GetComponent<Button>() != nullptr) {
			root[object + to_string(i)]["Components"][index] = "BUTTON";
			index++;
		}
		if (it.second->GetComponent<PlayerController>() != nullptr) {
			root[object + to_string(i)]["Components"][index] = "CONTROLLER";
			index++;
		}
		if (it.second->GetComponent<Animation>() != nullptr) {
			root[object + to_string(i)]["Components"][index] = "ANIMATION";
			index++;
		}
		if (it.second->GetComponent<Emitter>() != nullptr) {
			root[object + to_string(i)]["Components"][index] = "PARTICLE";
			if(it.second->GetComponent<Emitter>()->type == EmitterType::ET_TRAIL)
				root[object + to_string(i)]["ParticleType"] = "TRAIL";
			if (it.second->GetComponent<Emitter>()->type == EmitterType::ET_EXPLOSION)
				root[object + to_string(i)]["ParticleType"] = "ET_EXPLOSION";
			index++;
		}
		index = 0;

		switch (it.second->objectstyle)
		{
		case Objectstyle::Player:
			root[object + to_string(i)]["ObjectType"] = "Player";
			break;
		case Objectstyle::Asteriod:
			root[object + to_string(i)]["ObjectType"] = "Asteriod";
			break;
		case Objectstyle::Wall:
			root[object + to_string(i)]["ObjectType"] = "Wall";
			break;
		case Objectstyle::Button:
			root[object + to_string(i)]["ObjectType"] = "Button";
			break;
		case Objectstyle::Box:
			root[object + to_string(i)]["ObjectType"] = "Box";
			break;
		case Objectstyle::Trigger90:
			root[object + to_string(i)]["ObjectType"] = "Trigger90";
			break;
		case Objectstyle::Trigger180:
			root[object + to_string(i)]["ObjectType"] = "Trigger180";
			break;
		case Objectstyle::Clearzone:
			root[object + to_string(i)]["ObjectType"] = "Clearzone";
			break;
		case Objectstyle::Particle:
			root[object + to_string(i)]["ObjectType"] = "Particle";
			break;
			/*case Objectstyle::Camera:
			root[object + to_string(i)]["ObjectType"] = "Camera";
			break;*/
		default:
			DEBUG_ASSERT(true, "Invaild Object Style");


		}
		root[object + to_string(i)]["ObjectID"] = i;
		root[object + to_string(i)]["Position"]["x"] = it.second->GetComponent<Transform>()->position.x;
		root[object + to_string(i)]["Position"]["y"] = it.second->GetComponent<Transform>()->position.y;
		root[object + to_string(i)]["Position"]["z"] = it.second->GetComponent<Transform>()->position.z;
		root[object + to_string(i)]["Scale"]["x"] = it.second->GetComponent<Transform>()->scale.x;
		root[object + to_string(i)]["Scale"]["y"] = it.second->GetComponent<Transform>()->scale.y;
		if (FACTORY->ObjectIDMap[it.first]->GetComponent<Body>() != nullptr)
			root[object + to_string(i)]["Mass"] = it.second->GetComponent<Body>()->pm_mass;
		root[object + to_string(i)]["Rotation"] = it.second->GetComponent<Transform>()->angle;


		root[object + to_string(i)]["TextureDir"] = FACTORY->ObjectIDMap[it.first]->GetComponent<Sprite>()->mTexutureDir;
		i++;
	}


	file.WriteFile(path, root);


}



