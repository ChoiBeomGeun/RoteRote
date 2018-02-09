/**
\file        EditorData.h
\author      KyungOok Park
\par         email: qkrruddn6680@gmail.com
\par         course: GAM200
\par   ThumbUp Engine
\date        12/12/2017
\brief
struct GameData, store Object.
class EditorData, function for edit object.
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <Factory.h>
#include <vector>
namespace TE
{
	struct GameData
	{
		Object* Player;
		Object* ClearZone;

		std::map<Object*, bool> Wall;
		std::map<Object*, bool> Box;
		std::map<Object*, bool> Trigger180;
		std::map<Object*, bool> Trigger90;
		std::map<Object*, bool> Button;


		glm::vec3 mousePosBefore;
		glm::vec3 mousePosAfter;

		bool IsEditable_Object_Exist = false;

		void ClearData();
	};
	class EditorData : GameData
	{
	public:
		static void InitPlayer(GameData &gamedata);
		static void InitClearZone(GameData &gamedata);
		static void AddWall(GameData& gamedata, glm::vec3 mouse);
		static void AddBox(GameData& gamedata, glm::vec3 mouse);
		static void AddTrigger180(GameData& gamedata, glm::vec3 mouse);
		static void AddTrigger90(GameData& gamedata, glm::vec3 mouse);
		static void AddButton(GameData& gamedata, glm::vec3 mouse);

		static void MoveObject(GameData & gamedata, glm::vec3 mouse);
		static void EditableObject(GameData& gamedata, glm::vec3 mouse);
		static void UnEditObject(GameData& gamedata, glm::vec3 mouse);
		static void ScaleObject(GameData& gamedata, glm::vec3 mouse);
		static void RotateObject(GameData& gamedata, glm::vec3 mouse);

		static void DeleteObject(GameData& gamedata);
		static void LoadGameData(GameData & gamedata);

	private:
		static int Wall_Count;
		static int Box_Count;
		static int Trigger180_Count;
		static int Trigger90_Count;

		static bool PlayerEditable;
		static bool ClearZoneEditable;

	};
}