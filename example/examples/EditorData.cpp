/**
\file        EditorData.cpp
\author      KyungOok Park
\par         email: qkrruddn6680@gmail.com
\par         course: GAM200
\par       ThumbUp Engine
\date        12/12/2017
\brief

struct GameData, store Object.
class EditorData, function for edit object.
Add Object, Scale, Translate
Load Level and move to GameData to use it as a MapEditor.
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include "EditorData.h"
#include "Physics.h"
#include "Input.h"
using namespace TE;
bool PlayerEditable = false;
bool ClearZoneEditable = false;

void EditorData::InitPlayer(GameData & gamedata)
{
	gamedata.Player = FACTORY->CreatePlayer(glm::vec3(0, 0, 0), glm::vec3(10, 10, 0), glm::vec3(0, 0, 0), 1.f);
}

void EditorData::InitClearZone(GameData & gamedata)
{
	gamedata.ClearZone = FACTORY->CreateButton(glm::vec3(400, 400, 0), glm::vec3(10, 10, 0));
	gamedata.ClearZone->objectstyle = Objectstyle::Clearzone;
}

void EditorData::AddWall(GameData& gamedata, glm::vec3 mouse)
{
	std::pair<Object*, bool> temp;
	temp.second = false;
	temp.first = FACTORY->CreateArchetype(ReadingArchetype("Wall.json"));
	temp.first->transform->SetPosition(mouse);
	gamedata.Wall.insert(temp);
	++Wall_Count;
}

void EditorData::AddBox(GameData& gamedata, glm::vec3 mouse)
{
	std::pair<Object*, bool> temp;
	temp.second = false;
	temp.first = FACTORY->CreateArchetype(ReadingArchetype("Box.json"));
	temp.first->transform->SetPosition(mouse);
	gamedata.Box.insert(temp);
	++Box_Count;
}

void EditorData::AddTrigger180(GameData& gamedata, glm::vec3 mouse)
{
	std::pair<Object*, bool> temp;
	temp.second = false;
	temp.first = FACTORY->CreateArchetype(ReadingArchetype("Trigger180.json"));
	temp.first->transform->SetPosition(mouse);
	gamedata.Trigger180.insert(temp);
	++Trigger180_Count;
}

void EditorData::AddTrigger90(GameData& gamedata, glm::vec3 mouse)
{
	std::pair<Object*, bool> temp;
	temp.second = false;
	temp.first = FACTORY->CreateArchetype(ReadingArchetype("Trigger90.json"));
	temp.first->transform->SetPosition(mouse);
	gamedata.Trigger90.insert(temp);
	++Trigger90_Count;
}

void EditorData::AddButton(GameData & gamedata, glm::vec3 mouse)
{
	std::pair<Object*, bool> temp;
	temp.second = false;
	temp.first = FACTORY->CreateArchetype(ReadingArchetype("Button.json"));
	temp.first->transform->SetPosition(mouse);
	gamedata.Button.insert(temp);
}

void EditorData::MoveObject(GameData & gamedata, glm::vec3 mouse)
{
	mouse.z = 130.f;
	//mouse.z = 150.f;
	if (PlayerEditable)
		gamedata.Player->transform->position = mouse;

	if (ClearZoneEditable)
		gamedata.ClearZone->transform->position = mouse;


	auto box = gamedata.Box.begin();
	for (box; box != gamedata.Box.end(); ++box)
	{
		if (box->second)
			box->first->transform->position = mouse;
	}

	auto wall = gamedata.Wall.begin();
	for (wall; wall != gamedata.Wall.end(); ++wall)
	{
		if (wall->second)
			wall->first->transform->position = mouse;
	}

	auto trigger180 = gamedata.Trigger180.begin();
	for (trigger180; trigger180 != gamedata.Trigger180.end(); ++trigger180)
	{
		if (trigger180->second)
			trigger180->first->transform->position = mouse;
	}

	auto trigger90 = gamedata.Trigger90.begin();
	for (trigger90; trigger90 != gamedata.Trigger90.end(); ++trigger90)
	{
		if (trigger90->second)
			trigger90->first->transform->position = mouse;
	}

	auto button = gamedata.Button.begin();
	for (button; button != gamedata.Button.end(); ++button)
	{
		if (button->second)
			button->first->transform->position = mouse;
	}
}

void EditorData::EditableObject(GameData & gamedata, glm::vec3 mouse)
{
	if (!gamedata.IsEditable_Object_Exist)
	{
		if (Physics::MouseVsRect(mouse, gamedata.Player->transform))
		{
			PlayerEditable = true;
			gamedata.IsEditable_Object_Exist = true;
			return;
		}
	}
	if (!gamedata.IsEditable_Object_Exist)
	{
		if (Physics::MouseVsRect(mouse, gamedata.ClearZone->transform))
		{
			ClearZoneEditable = true;
			gamedata.IsEditable_Object_Exist = true;
			return;
		}
	}
	if (!gamedata.IsEditable_Object_Exist)
	{
		auto box = gamedata.Box.begin();
		for (box; box != gamedata.Box.end(); ++box)
		{
			if (Physics::MouseVsRect(mouse, box->first->transform))
			{
				box->second = true;
				gamedata.IsEditable_Object_Exist = true;
				return;
			}
		}
	}

	if (!gamedata.IsEditable_Object_Exist)
	{
		auto wall = gamedata.Wall.begin();
		for (wall; wall != gamedata.Wall.end(); ++wall)
		{
			if (Physics::MouseVsRect(mouse, wall->first->transform))
			{
				wall->second = true;
				gamedata.IsEditable_Object_Exist = true;
				return;
			}
		}
	}

	if (!gamedata.IsEditable_Object_Exist)
	{
		auto trigger180 = gamedata.Trigger180.begin();
		for (trigger180; trigger180 != gamedata.Trigger180.end(); ++trigger180)
		{
			if (Physics::MouseVsRect(mouse, trigger180->first->transform))
			{
				trigger180->second = true;
				gamedata.IsEditable_Object_Exist = true;
				return;
			}
		}
	}
	if (!gamedata.IsEditable_Object_Exist)
	{
		auto trigger90 = gamedata.Trigger90.begin();
		for (trigger90; trigger90 != gamedata.Trigger90.end(); ++trigger90)
		{
			if (Physics::MouseVsRect(mouse, trigger90->first->transform))
			{
				trigger90->second = true;
				gamedata.IsEditable_Object_Exist = true;
				return;
			}
		}
	}

	if (!gamedata.IsEditable_Object_Exist)
	{
		auto button = gamedata.Button.begin();
		for (button; button != gamedata.Button.end(); ++button)
		{
			if (Physics::MouseVsRect(mouse, button->first->transform))
			{
				button->second = true;
				gamedata.IsEditable_Object_Exist = true;
				return;
			}
		}
	}
}

void EditorData::UnEditObject(GameData & gamedata, glm::vec3 mouse)
{
	if (Physics::MouseVsRect(mouse, gamedata.Player->transform))
	{
		PlayerEditable = false;
		gamedata.IsEditable_Object_Exist = false;
	}

	if (Physics::MouseVsRect(mouse, gamedata.ClearZone->transform))
	{
		ClearZoneEditable = false;
		gamedata.IsEditable_Object_Exist = false;
	}

	auto box = gamedata.Box.begin();
	for (box; box != gamedata.Box.end(); ++box)
	{
		if (Physics::MouseVsRect(mouse, box->first->transform))
		{
			box->second = false;
			gamedata.IsEditable_Object_Exist = false;
		}
	}

	auto wall = gamedata.Wall.begin();
	for (wall; wall != gamedata.Wall.end(); ++wall)
	{
		if (Physics::MouseVsRect(mouse, wall->first->transform))
		{
			wall->second = false;
			gamedata.IsEditable_Object_Exist = false;
		}
	}

	auto trigger180 = gamedata.Trigger180.begin();
	for (trigger180; trigger180 != gamedata.Trigger180.end(); ++trigger180)
	{
		if (Physics::MouseVsRect(mouse, trigger180->first->transform))
		{
			trigger180->second = false;
			gamedata.IsEditable_Object_Exist = false;
		}
	}

	auto trigger90 = gamedata.Trigger90.begin();
	for (trigger90; trigger90 != gamedata.Trigger90.end(); ++trigger90)
	{
		if (Physics::MouseVsRect(mouse, trigger90->first->transform))
		{
			trigger90->second = false;
			gamedata.IsEditable_Object_Exist = false;
		}
	}

	auto button = gamedata.Button.begin();
	for (button; button != gamedata.Button.end(); ++button)
	{
		if (Physics::MouseVsRect(mouse, button->first->transform))
		{
			button->second = false;
			gamedata.IsEditable_Object_Exist = false;
		}
	}
}

void EditorData::ScaleObject(GameData & gamedata, glm::vec3 mouse)
{
	float xdis = 0;
	float ydis = 0;

	if (Input::IsMouseUp)
	{
		gamedata.mousePosAfter = mouse;

		xdis = (gamedata.mousePosBefore.x - gamedata.mousePosAfter.x);
		ydis = (gamedata.mousePosBefore.y - gamedata.mousePosAfter.y);

		auto box = gamedata.Box.begin();
		for (box; box != gamedata.Box.end(); ++box)
		{
			if (box->second)
			{
				if (abs(xdis) > abs(ydis))
				{
					box->first->transform->scale.x -= xdis;
				}
				else
				{
					box->first->transform->scale.y -= ydis;
				}
			}
		}

		auto wall = gamedata.Wall.begin();
		for (wall; wall != gamedata.Wall.end(); ++wall)
		{
			if (wall->second)
			{
				if (abs(xdis) > abs(ydis))
				{
					wall->first->transform->scale.x -= xdis;
				}
				else
					wall->first->transform->scale.y -= ydis;
			}
		}

		auto trigger180 = gamedata.Trigger180.begin();
		for (trigger180; trigger180 != gamedata.Trigger180.end(); ++trigger180)
		{
			if (trigger180->second)
			{
				if (abs(xdis) > abs(ydis))
				{
					trigger180->first->transform->scale.x -= xdis;
				}
				else
					trigger180->first->transform->scale.y -= ydis;
			}
		}

		auto trigger90 = gamedata.Trigger90.begin();
		for (trigger90; trigger90 != gamedata.Trigger90.end(); ++trigger90)
		{
			if (trigger90->second)
			{
				if (abs(xdis) > abs(ydis))
				{
					trigger90->first->transform->scale.x -= xdis;
				}
				else
					trigger90->first->transform->scale.y -= ydis;
			}
		}

		auto button = gamedata.Button.begin();
		for (button; button != gamedata.Button.end(); ++button)
		{
			if (button->second)
			{
				if (abs(xdis) > abs(ydis))
				{
					button->first->transform->scale.x -= xdis;
				}
				else
					button->first->transform->scale.y -= ydis;
			}
		}
	}


}

void EditorData::RotateObject(GameData & gamedata, glm::vec3 mouse)
{
	float xdis = 0;
	float ydis = 0;

	if (Input::IsMouseUp)
	{
		gamedata.mousePosAfter = mouse;

		xdis = (gamedata.mousePosBefore.x - gamedata.mousePosAfter.x);
		ydis = (gamedata.mousePosBefore.y - gamedata.mousePosAfter.y);

		auto box = gamedata.Box.begin();
		for (box; box != gamedata.Box.end(); ++box)
		{
			if (box->second)
			{
				if (abs(xdis) > abs(ydis))
				{
					box->first->transform->angle -= xdis;
				}
				else
				{
					box->first->transform->angle -= ydis;
				}
			}
		}

		auto wall = gamedata.Wall.begin();
		for (wall; wall != gamedata.Wall.end(); ++wall)
		{
			if (wall->second)
			{
				if (abs(xdis) > abs(ydis))
				{
					wall->first->transform->angle -= xdis;
				}
				else
					wall->first->transform->angle -= ydis;
			}
		}

		auto trigger180 = gamedata.Trigger180.begin();
		for (trigger180; trigger180 != gamedata.Trigger180.end(); ++trigger180)
		{
			if (trigger180->second)
			{
				if (abs(xdis) > abs(ydis))
				{
					trigger180->first->transform->angle -= xdis;
				}
				else
					trigger180->first->transform->angle -= ydis;
			}
		}

		auto trigger90 = gamedata.Trigger90.begin();
		for (trigger90; trigger90 != gamedata.Trigger90.end(); ++trigger90)
		{
			if (trigger90->second)
			{
				if (abs(xdis) > abs(ydis))
				{
					trigger90->first->transform->angle -= xdis;
				}
				else
					trigger90->first->transform->angle -= ydis;
			}
		}

		auto button = gamedata.Button.begin();
		for (button; button != gamedata.Button.end(); ++button)
		{
			if (button->second)
			{
				if (abs(xdis) > abs(ydis))
				{
					button->first->transform->angle -= xdis;
				}
				else
					button->first->transform->angle -= ydis;
			}
		}
	}
}

void EditorData::DeleteObject(GameData & gamedata)
{
	auto box = gamedata.Box.begin();
	for (box; box != gamedata.Box.end(); ++box)
	{
		if (box->second)
		{
			FACTORY->Destroy(box->first);
			gamedata.Box.erase(box);
			gamedata.IsEditable_Object_Exist = false;
			return;
		}
	}

	auto wall = gamedata.Wall.begin();
	for (wall; wall != gamedata.Wall.end(); ++wall)
	{
		if (wall->second)
		{
			FACTORY->Destroy(wall->first);
			gamedata.Wall.erase(wall);
			gamedata.IsEditable_Object_Exist = false;
			return;
		}
	}

	auto trigger180 = gamedata.Trigger180.begin();
	for (trigger180; trigger180 != gamedata.Trigger180.end(); ++trigger180)
	{
		if (trigger180->second)
		{
			FACTORY->Destroy(trigger180->first);
			gamedata.Trigger180.erase(trigger180);
			gamedata.IsEditable_Object_Exist = false;
			return;
		}
	}

	auto trigger90 = gamedata.Trigger90.begin();
	for (trigger90; trigger90 != gamedata.Trigger90.end(); ++trigger90)
	{
		if (trigger90->second)
		{
			FACTORY->Destroy(trigger90->first);
			gamedata.Trigger90.erase(trigger90);
			gamedata.IsEditable_Object_Exist = false;
			return;
		}
	}

	auto button = gamedata.Button.begin();
	for (button; button != gamedata.Button.end(); ++button)
	{
		if (button->second)
		{
			FACTORY->Destroy(button->first);
			//Object* toDelete = button->first;
			gamedata.Button.erase(button);
			gamedata.IsEditable_Object_Exist = false;
			return;
		}
	}
}

void EditorData::LoadGameData(GameData & gamedata)
{
	for (auto it : FACTORY->ObjectIDMap) {
		switch (it.second->objectstyle)
		{
		case Objectstyle::Box:
			gamedata.Box.insert(std::pair<Object*, bool>(it.second, false));
			break;
		case Objectstyle::Trigger180:
			gamedata.Trigger180.insert(std::pair<Object*, bool>(it.second, false));
			break;
		case Objectstyle::Trigger90:
			gamedata.Trigger90.insert(std::pair<Object*, bool>(it.second, false));
			break;
		case Objectstyle::Wall:
			gamedata.Wall.insert(std::pair<Object*, bool>(it.second, false));
			break;
		case Objectstyle::Button:
			gamedata.Button.insert(std::pair<Object*, bool>(it.second, false));
			break;
		case Objectstyle::Player:
			gamedata.Player = FACTORY->GetPlayer();
			PlayerEditable = false;
			break;
		case Objectstyle::Clearzone:
			gamedata.ClearZone = it.second;
			ClearZoneEditable = false;
			break;
		default:
			break;
		}
	}
}

void GameData::ClearData()
{

	Box.clear();
	Wall.clear();
	Trigger180.clear();
	Trigger90.clear();
	Button.clear();

	IsEditable_Object_Exist = false;
}
