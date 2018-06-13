/******************************************************************************/
/*!
\file       MapEditor.cpp
\author     KyungOok Park
\par        email: qkrruddn6680@gmail.com
\par        course: GAM250
\par		ThumbUp Engine
\date       12/12/2017
\brief
MapEditor state

Load Level and Edit it.
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include <iostream>

#include "MapEditor.h"
#include "InGameLogic.h"
#include "Input.h"
#include "Application.h"
#include "StateManager.h"
#include "Factory.h"
#include "Input.h"
#include "Engine.h"
#include "StateLists.h"
#include "Graphics.h"

using namespace TE;

bool EditorData::PlayerEditable = false;
bool EditorData::ClearZoneEditable = false;

int EditorData::Wall_Count = 0;
int EditorData::Box_Count = 0;
int EditorData::Trigger180_Count = 0;
int EditorData::Trigger90_Count = 0;

bool Input::IsMouseDown;
bool Input::IsMouseUp;

GameData gamedata;

MapEditor::MapEditor()
{

}

MapEditor::~MapEditor()
{
}

void MapEditor::Load(void)
{
	LEVELMANAGER->LoadLevel(STATEMANAGER->Loadtolevelname);
	for (auto i : FACTORY->ObjectIDMap)
		i.second->GetComponent<Transform>()->position.z = 130;
	EditorData::LoadGameData(gamedata);
}

void MapEditor::Init(void)
{
	CAMERA->IsCameraAttached = false;
	CAMERA->cameraPos.z = 999.f;
	OBJ_scale = glm::vec3(5.f, 5.f, 0);
	gamedata.mousePosAfter = glm::vec3(0, 0, 0);
	gamedata.mousePosBefore = glm::vec3(0, 0, 0);

	PHYSICS->IsMapEditorOn = true;

	FACTORY->GetPlayer()->GetComponent<Animation>()->setFrame(1.0f / 6);
	FACTORY->GetPlayer()->GetComponent<Animation>()->setTime(1.0f / 6);
}

void MapEditor::Update(float dt)
{
	dt = dt;
	if (Input::IsTriggered(SDL_SCANCODE_F7))
		STATEMANAGER->MoveState(StatesList::Level1);
	SDL_GetMouseState(&x, &y);
	mouse.x = static_cast<float>(x);
	mouse.y = static_cast<float>(y);
	mouse = APP->ConvertToWorld(mouse);
	mouse.x = (float)TUMath::GridRound(mouse.x);
	mouse.y = (float)TUMath::GridRound(mouse.y);

	if (Input::IsMouseDown)
		gamedata.mousePosBefore = mouse;

	if (Input::IsPressed(SDL_SCANCODE_R))
	{
		SDL_GetMouseState(&x, &y);
		mouse.x = static_cast<float>(x);
		mouse.y = static_cast<float>(y);
		mouse = APP->ConvertToWorld(mouse);
		mouse.x = (float)TUMath::GridRound(mouse.x);
		mouse.y = (float)TUMath::GridRound(mouse.y);
		EditorData::RotateObject(gamedata, mouse);
	}

	if (Input::IsPressed(SDL_SCANCODE_LCTRL))
	{
		SDL_GetMouseState(&x, &y);
		mouse.x = static_cast<float>(x);
		mouse.y = static_cast<float>(y);
		mouse = APP->ConvertToWorld(mouse);
		mouse.x = (float)TUMath::GridRound(mouse.x);
		mouse.y = (float)TUMath::GridRound(mouse.y);
		EditorData::ScaleObject(gamedata, mouse);
	}
	else if (Input::IsMouseTriggered(SDL_BUTTON_LEFT))
	{
		SDL_GetMouseState(&x, &y);
		mouse.x = static_cast<float>(x);
		mouse.y = static_cast<float>(y);
		mouse = APP->ConvertToWorld(mouse);
		mouse.x = (float)TUMath::GridRound(mouse.x);
		mouse.y = (float)TUMath::GridRound(mouse.y);
		EditorData::EditableObject(gamedata, mouse);
	}
	else if (Input::IsMouseTriggered(SDL_BUTTON_RIGHT))
	{
		SDL_GetMouseState(&x, &y);
		mouse.x = static_cast<float>(x);
		mouse.y = static_cast<float>(y);
		mouse = APP->ConvertToWorld(mouse);
		mouse.x = (float)TUMath::GridRound(mouse.x);
		mouse.y = (float)TUMath::GridRound(mouse.y);
		EditorData::UnEditObject(gamedata, mouse);
	}
	else
	{
		SDL_GetMouseState(&x, &y);
		mouse.x = static_cast<float>(x);
		mouse.y = static_cast<float>(y);
		mouse = APP->ConvertToWorld(mouse);
		mouse.x = (float)TUMath::GridRound(mouse.x);
		mouse.y = (float)TUMath::GridRound(mouse.y);
		EditorData::MoveObject(gamedata, mouse);
	}

	//if (Input::IsTriggered(SDL_SCANCODE_LEFT))
	//{
	//   EditorData::ScaleObject(gamedata, mouse);
	//}

	//if (Input::IsMouseDown)
	//{
	//   gamedata.mousePosBefore = mouse;

	//   printf("MouseDown\n");
	//   printf("%f , %f", gamedata.mousePosBefore.x, gamedata.mousePosBefore.y);
	//}

	//if (Input::IsMouseUp)
	//{
	//   gamedata.mousePosAfter = mouse;
	//   printf("Up\n");
	//   printf("%f , %f", gamedata.mousePosAfter.x, gamedata.mousePosAfter.y);
	//}




	//if (Input::IsTriggered(SDL_SCANCODE_M))
	//{
	//   GRAPHICS->drawGrid = true;
	//}
	//if (Input::IsTriggered(SDL_SCANCODE_G))
	//{
	//   GRAPHICS->drawGrid = false;
	//}
	if (Input::IsTriggered(SDL_SCANCODE_BACKSPACE))
	{
		EditorData::DeleteObject(gamedata);
	}

	if (Input::IsTriggered(SDL_SCANCODE_1))
	{
		SDL_GetMouseState(&x, &y);
		mouse.x = static_cast<float>(x);
		mouse.y = static_cast<float>(y);
		mouse = APP->ConvertToWorld(mouse);
		mouse.x = (float)TUMath::GridRound(mouse.x);
		mouse.y = (float)TUMath::GridRound(mouse.y);
		EditorData::AddBox(gamedata, mouse);
	}
	else if (Input::IsTriggered(SDL_SCANCODE_2))
	{
		SDL_GetMouseState(&x, &y);
		mouse.x = static_cast<float>(x);
		mouse.y = static_cast<float>(y);
		mouse = APP->ConvertToWorld(mouse);
		mouse.x = (float)TUMath::GridRound(mouse.x);
		mouse.y = (float)TUMath::GridRound(mouse.y);
		EditorData::AddWall(gamedata, mouse);
	}
	else if (Input::IsTriggered(SDL_SCANCODE_3))
	{
		SDL_GetMouseState(&x, &y);
		mouse.x = static_cast<float>(x);
		mouse.y = static_cast<float>(y);
		mouse = APP->ConvertToWorld(mouse);
		mouse.x = (float)TUMath::GridRound(mouse.x);
		mouse.y = (float)TUMath::GridRound(mouse.y);
		EditorData::AddTrigger180(gamedata, mouse);
	}
	else if (Input::IsTriggered(SDL_SCANCODE_4))
	{
		SDL_GetMouseState(&x, &y);
		mouse.x = static_cast<float>(x);
		mouse.y = static_cast<float>(y);
		mouse = APP->ConvertToWorld(mouse);
		mouse.x = (float)TUMath::GridRound(mouse.x);
		mouse.y = (float)TUMath::GridRound(mouse.y);
		EditorData::AddTrigger90(gamedata, mouse);
	}
	else if (Input::IsTriggered(SDL_SCANCODE_5))
	{
		SDL_GetMouseState(&x, &y);
		mouse.x = static_cast<float>(x);
		mouse.y = static_cast<float>(y);
		mouse = APP->ConvertToWorld(mouse);
		mouse.x = (float)TUMath::GridRound(mouse.x);
		mouse.y = (float)TUMath::GridRound(mouse.y);
		EditorData::AddButton(gamedata, mouse);
	}

	if (Input::IsPressed(SDL_SCANCODE_W))
	{
		CAMERA->cameraPos.y += 10.f;
	}
	if (Input::IsPressed(SDL_SCANCODE_A))
	{
		CAMERA->cameraPos.x -= 10.f;
	}
	if (Input::IsPressed(SDL_SCANCODE_S))
	{
		CAMERA->cameraPos.y -= 10.f;
	}
	if (Input::IsPressed(SDL_SCANCODE_D))
	{
		CAMERA->cameraPos.x += 10.f;
	}

}

void MapEditor::Free(void)
{
	gamedata.ClearData();
	PHYSICS->IsMapEditorOn = false;

	FACTORY->DestroyAllObjects();
}

void MapEditor::Unload(void)
{
}

