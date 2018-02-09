/******************************************************************************/
/*!
\file   ImguiGraphics.cpp
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

Imgui drawing source file
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "ImguiGraphics.h"
#include "StateManager.h"
#include "SoundManager.h"
bool IsImguiOn = false;
using namespace TE;
char** cppLevelitems;
char** cppTextureitems;
char** cppSounditems;
char** cppArchetypesditems;
char** cppComponentitems = new char*[6];

glm::vec3 * points;
static int j = -1;
int sizeofobject = 0;
void SaveingLevel(std::string path, int WorldX, int WorldY);
void CheckSize(const int mapSizeX, const int mapSizeY);
struct ObjectInfo {
	bool selected = false;
	char Name = 'D';


};
static ObjectInfo  *selected;


void ImGuiUpdate(void) {
	cppComponentitems[0] = "Transform";
	cppComponentitems[1] = "Controller";
	cppComponentitems[2] = "Sprite";
	cppComponentitems[3] = "Body";
	cppComponentitems[4] = "Button";
	cppComponentitems[5] = "Trigger";

	ImGui_ImplSdlGL3_NewFrame(APP->pWnd);
	if (Input::IsTriggered(SDL_SCANCODE_F5))
	{
		IsImguiOn = true;


	}
	if (IsImguiOn)
	{
		static int vec4f[2] = { 800, 600 };
		static int vec4i[4] = { 1, 5, 100, 255 };


		if (ImGui::CollapsingHeader("Map Editor"))
		{
			static int item = 0;
			static bool IsFirst = true;
			const char* items[] = { "Wall", "Player", "Clearzone", "Box","Trigger180","Trigger90" };
			const char itemsi[] = { 'W', 'P', 'C', 'B','R','G' };
			static char str0[128] = "Level~";
			ImGui::Combo("Combo", &item, items, sizeof(items) / sizeof(items[0]));
			ImGui::InputInt2("Map Size X,Y", vec4f);
			ImGui::InputText("Input File Name", str0, IM_ARRAYSIZE(str0));
			if (IsFirst)
			{
				selected = new ObjectInfo[(vec4f[0] / 100) * (vec4f[1] / 100)];
				IsFirst = false;
			}
			if (ImGui::Button("Change the Size"))
			{
				delete selected;
				selected = new ObjectInfo[(vec4f[0] / 100) * (vec4f[1] / 100)];
			}
			CheckSize(vec4f[0], vec4f[1]);
			for (int i = 0; i < (vec4f[0] / 100) * (vec4f[1] / 100); i++)
			{

				ImGui::PushID(i);

				if (ImGui::Selectable(&(selected[i].Name), &selected[i].selected, 0, ImVec2(10, 10)))
				{

					selected[i].Name = (itemsi[item]);
				}
				if ((i % (vec4f[0] / 100)) < ((vec4f[0] / 100) - 1))ImGui::SameLine();
				ImGui::PopID();
			}


			if (ImGui::Button("SaveLevel"))
			{
				SaveingLevel(str0, vec4f[0], vec4f[1]);
			}
		}

		if (ImGui::CollapsingHeader("SoundManager"))
		{
			static int t = -1;
			cppSounditems = new char*[ENGINE->mVsSoundnamelist.size() + 1];
			for (unsigned int i = 0; i < ENGINE->mVsSoundnamelist.size(); i++)
			{
				cppSounditems[i] = const_cast<char*>(ENGINE->mVsSoundnamelist[i].c_str());

			}
			ImGui::Combo("Sounds", &t, cppSounditems, (int)ENGINE->mVsSoundnamelist.size());
			if (ImGui::Button("Play"))
			{
				SOUNDID temp = SOUNDMANAGER->LoadSound(cppSounditems[t]);
				SOUNDMANAGER->PlaySounds(temp, false);

			}
			if (ImGui::Button("Mute"))
				SOUNDMANAGER->PauseAllSound();
			if (ImGui::Button("Unmute"))
				SOUNDMANAGER->ResumeAllSound();



		}
		std::string test = "Object";

		if (ImGui::CollapsingHeader("CameraManager"))
		{
			ImGui::SliderFloat("CAMERAEYEx", &CAMERA->cameraPos.x, -5000, 5000);
			ImGui::SliderFloat("CAMERAEYEy", &CAMERA->cameraPos.y, -5000, 5000);
			ImGui::SliderFloat("CAMERAEYEz", &CAMERA->cameraPos.z, 0, 999);

			ImGui::SliderFloat("CAMERATRAGETx", &CAMERA->cameraTarget.x, -50, 50);
			ImGui::SliderFloat("CAMERATRAGETy", &CAMERA->cameraTarget.y, -50, 50);
			ImGui::SliderFloat("CAMERATRAGETz", &CAMERA->cameraTarget.z, -50, 50);

			ImGui::SliderFloat("CAMERAUPx", &CAMERA->cameraUp.x, -50, 50);
			ImGui::SliderFloat("CAMERAUPy", &CAMERA->cameraUp.y, -50, 50);
			ImGui::SliderFloat("CAMERAUPz", &CAMERA->cameraUp.z, -50, 50);

		}

		if (ImGui::CollapsingHeader("ObjectManager"))
		{
			ImGui::Indent();
			std::string ObjectStyle;
			for (auto i : FACTORY->ObjectIDMap)
			{
				switch (i.second->objectstyle)
				{
				case Objectstyle::Clearzone:
					ObjectStyle = "Clearzone";
					break;
				case Objectstyle::Trigger180:
					ObjectStyle = "Trigger180";
					break;
				case Objectstyle::Trigger90:
					ObjectStyle = "Trigger90";
					break;
				case Objectstyle::Button:
					ObjectStyle = "Button";
					break;
				case Objectstyle::Player:
					ObjectStyle = "Player";
					break;
				case Objectstyle::Wall:
					ObjectStyle = "Wall";
					break;
				case Objectstyle::Box:
					ObjectStyle = "Box";
					break;
				}
				std::string Object = "Object" + std::to_string(i.second->objID) + " - ";
				std::string ObjectScale = "Scale";
				std::string ObjectPosition = "Position";
				std::string ObjectVelocity = "Velocity";
				std::string ObjectRotation = "Rotation";
				std::string ObjectMass = "Mass";

				Object += ObjectStyle;

				ObjectScale += std::to_string(i.second->objID);
				ObjectPosition += std::to_string(i.second->objID);
				if (ImGui::CollapsingHeader(Object.c_str()))
				{
					if (i.second->objectstyle != Objectstyle::Player && 
						i.second->objectstyle != Objectstyle::Clearzone) {
						if (ImGui::Button(("Delete" + Object).c_str()))
						{
							FACTORY->Destroy(i.second);

						}
					}
					// Transform Component
					if (FACTORY->ObjectIDMap[i.first]->GetComponent(ComponentType::CT_TRANSFORM) != nullptr)
					{
						if (ImGui::TreeNode(("Transform " + std::to_string(i.second->objID)).c_str()))
						{

							ImGui::SliderFloat((ObjectScale + "x").c_str(), &FACTORY->ObjectIDMap[i.first]->transform->scale.x, 0, (float)APP->_screenWidth);
							ImGui::SliderFloat((ObjectScale + "y").c_str(), &FACTORY->ObjectIDMap[i.first]->transform->scale.y, 0, (float)APP->_screenHeight);
							ImGui::SliderFloat((ObjectScale + "z").c_str(), &FACTORY->ObjectIDMap[i.first]->transform->scale.z, 0, 1);
							ImGui::SliderFloat((ObjectPosition + "x").c_str(), &FACTORY->ObjectIDMap[i.first]->transform->position.x, -1000.f, 1000.f);
							ImGui::SliderFloat((ObjectPosition + "y").c_str(), &FACTORY->ObjectIDMap[i.first]->transform->position.y, -1000.f, 1000.f);
							ImGui::SliderFloat((ObjectPosition + "z").c_str(), &FACTORY->ObjectIDMap[i.first]->transform->position.z, -1000.f, 1000.f);
							ImGui::SliderFloat((ObjectRotation + "z").c_str(), &FACTORY->ObjectIDMap[i.first]->transform->angle, 0, TUMath::PI);

							ImGui::TreePop();
						}
					}
					if (FACTORY->ObjectIDMap[i.first]->GetComponent(ComponentType::CT_BODY) != nullptr)
					{

						if (ImGui::TreeNode(("Body " + std::to_string(i.second->objID)).c_str()))
						{
							ImGui::SliderFloat((ObjectVelocity + "x").c_str(), &FACTORY->ObjectIDMap[i.first]->body->pm_velocity.x, -1000.f, 1000.f);
							ImGui::SliderFloat((ObjectVelocity + "y").c_str(), &FACTORY->ObjectIDMap[i.first]->body->pm_velocity.y, -1000.f, 1000.f);
							ImGui::SliderFloat(ObjectMass.c_str(), &FACTORY->ObjectIDMap[i.first]->body->pm_mass, 0, (float)APP->_screenHeight);
							ImGui::TreePop();
						}
					}
					if (FACTORY->ObjectIDMap[i.first]->GetComponent(ComponentType::CT_SPRITE) != nullptr)
					{

						if (ImGui::TreeNode(("Sprite " + std::to_string(i.second->objID)).c_str()))
						{
							int arraysize2 = 0;
							cppTextureitems = new char*[ENGINE->mVsTexturenamelist.size() + 1];
							for (unsigned int j = 0; j < ENGINE->mVsTexturenamelist.size(); j++)
							{
								cppTextureitems[j] = const_cast<char*>(ENGINE->mVsTexturenamelist[j].c_str());
								arraysize2 = j;
							}

							ImGui::Combo("Levels", &j, cppTextureitems, (int)ENGINE->mVsTexturenamelist.size());
							if (ImGui::Button("Load"))
							{
								i.second->sprite->texture_load(cppTextureitems[j]);

							}
							ImGui::ColorEdit4("Color", FACTORY->ObjectIDMap[i.second->objID]->sprite->Color, ImGuiColorEditFlags_Uint8);
							ImGui::SliderFloat("Depth", &FACTORY->ObjectIDMap[i.second->objID]->sprite->depth, 0, 1);

							ImGui::TreePop();
						}
					}
				}
			}
			static int t = -1;
			cppArchetypesditems = new char*[ENGINE->mVsArchtypenamelist.size() + 1];
			for (unsigned int i = 0; i < ENGINE->mVsArchtypenamelist.size(); i++)
			{
				cppArchetypesditems[i] = const_cast<char*>(ENGINE->mVsArchtypenamelist[i].c_str());

			}

			ImGui::Combo("Archtypes", &t, cppArchetypesditems,(int) ENGINE->mVsArchtypenamelist.size());


			if (ImGui::Button("Make Archtypes"))
				FACTORY->CreateArchetype(ReadingArchetype(cppArchetypesditems[t]));

		}

		cppLevelitems = new char*[ENGINE->mVsLevelnamelist.size() + 1];
		for (unsigned int i = 0; i < ENGINE->mVsLevelnamelist.size(); i++)
		{
			cppLevelitems[i] = const_cast<char*>(ENGINE->mVsLevelnamelist[i].c_str());

		}
		static int t = -1;

		if (ImGui::CollapsingHeader("Levels"))
		{
			ImGui::Combo("Levels", &t, cppLevelitems, (int)ENGINE->mVsLevelnamelist.size());
			if (ImGui::Button("Save"))
				LEVELMANAGER->SaveLevel(cppLevelitems[t]);
			ImGui::SameLine();
			if (ImGui::Button("Load")) {
				STATEMANAGER->forceLevelLoading = true;
				LEVELMANAGER->LoadLevel(cppLevelitems[t]);
			}
		}
		static float position[3] = { 0,0,0 };
		static float scale[3] = { 0,0,0 };
		static float rotation =  0 ;
		static int t2 = 1;
		static int t3 = 1;
		static bool GravityOn = false;
		static float Mass = 0.f;
		static bool Componentlist[6]{ false };
		static bool triggerType180ornot = false;
		if (ImGui::CollapsingHeader("Archetype Editor"))
		{
			if (ImGui::TreeNode("Making Archetype"))
			{
				static char  name[128] = "Default";
				static char  Style[128] = "Default";
				ImGui::InputText("Name", name, IM_ARRAYSIZE(name));
				ImGui::InputText("ObjectStyle", Style, IM_ARRAYSIZE(Style));
				if (Componentlist[0]) {
					ImGui::InputFloat3("Position", position, 1);
					ImGui::InputFloat3("Scale", scale, 1);
					ImGui::InputFloat("Rotation", &rotation, 1);
				}
				int arraysize2 = 0;
			
				cppTextureitems = new char*[ENGINE->mVsTexturenamelist.size() + 1];
				for (unsigned int j = 0; j<ENGINE->mVsTexturenamelist.size(); j++)
				{
					cppTextureitems[j] = const_cast<char*>(ENGINE->mVsTexturenamelist[j].c_str());
					arraysize2 = j;
				}
				if (Componentlist[2])
				ImGui::Combo("Textures", &t2, cppTextureitems,(int) ENGINE->mVsTexturenamelist.size());

				if (Componentlist[3]) {

					ImGui::InputFloat("Mass", &Mass, -1);
					ImGui::Checkbox("GravityOn", &GravityOn);

				}

				if (Componentlist[5]) {
					ImGui::Checkbox("180AngleOrNot", &triggerType180ornot);
					

				}
				for (int i = 0; i < 6; i++) {
					if (Componentlist[i]) {
						ImGui::Text(cppComponentitems[i]);

					}


				}


				ImGui::Combo("ComponentAdd", &t3, cppComponentitems, 6);
				if (!Componentlist[t3]) {
					if (ImGui::Button("Add"))
						Componentlist[t3] = true;

				}
				else
				{

					if (ImGui::Button("Delete"))
						Componentlist[t3] = false;

				}

				if (ImGui::Button("Save"))
				{
					Archetype temp;
					temp.ArchetypeName = name;
					temp.DefaultObjectStyle = name;
					temp.DefaultPosition = glm::vec3{ position[0],position[1],position[2] };
					temp.DefaultScale = glm::vec3{ scale[0],scale[1],scale[2] };
					temp.DefaultTexture = cppTextureitems[t2];
					temp.GravityOn = GravityOn;
					temp.mass = Mass;
					temp.rotation = rotation;
					temp._180triggerornot = triggerType180ornot;
				
					for (int i = 0; i < 6; i++) {
						if (Componentlist[i])
							temp.HavingComponents.push_back((TE::ComponentType)(i + 1));
					}

					SavingArchtype(temp);
					ENGINE->Filenameloading();
				}


				ImGui::TreePop();
			}


		}

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Object Number : %d", FACTORY->GetAllObjectNumbers());

	}




	ImGui::Render();

}










void SaveingLevel(std::string path, int WorldX, int WorldY)
{
	using namespace std;
	path = ".\\levels.\\" + path + ".json";
	Jsonclass file;
	Json::Value root;
	std::string object = "Object";


	root["Level"] = 1;
	root["WorldSize"]["x"] = WorldX;
	root["WorldSize"]["y"] = WorldY;
	root["DefalutCamera"]["EYE"]["x"] = CAMERA->cameraPos.x;
	root["DefalutCamera"]["EYE"]["y"] = CAMERA->cameraPos.y;
	root["DefalutCamera"]["EYE"]["z"] = CAMERA->cameraPos.z;
	root["DefalutCamera"]["TARGET"]["x"] = CAMERA->cameraTarget.x;
	root["DefalutCamera"]["TARGET"]["y"] = CAMERA->cameraTarget.y;
	root["DefalutCamera"]["TARGET"]["z"] = CAMERA->cameraTarget.z;
	root["DefalutCamera"]["UP"]["x"] = CAMERA->cameraUp.x;
	root["DefalutCamera"]["UP"]["y"] = CAMERA->cameraUp.y;
	root["DefalutCamera"]["UP"]["z"] = CAMERA->cameraUp.z;
	int  i = 1;
	int j = 0;
	for (int q = 0; q < (WorldX / 100) * (WorldY / 100); q++)
	{
		if (selected[q].selected)
		{
			if (selected[q].Name == 'P')
				root[object + to_string(i)]["ObjectType"] = "Player";
			if (selected[q].Name == 'W')
				root[object + to_string(i)]["ObjectType"] = "Wall";
			if (selected[q].Name == 'B')
				root[object + to_string(i)]["ObjectType"] = "Box";
			if (selected[q].Name == 'R')
				root[object + to_string(i)]["ObjectType"] = "Trigger180";
			if (selected[q].Name == 'G')
				root[object + to_string(i)]["ObjectType"] = "Trigger90";
			if (selected[q].Name == 'C')
				root[object + to_string(i)]["ObjectType"] = "Clearzone";

			root[object + to_string(i)]["ObjectID"] = i;
			root[object + to_string(i)]["Position"]["x"] = points[j].x;
			root[object + to_string(i)]["Position"]["y"] = points[j].y;
			if (selected[q].Name == 'P') {
				root[object + to_string(i)]["Scale"]["x"] = 70;
				root[object + to_string(i)]["Scale"]["y"] = 120;
			}
			if (selected[q].Name == 'W') {
				root[object + to_string(i)]["Scale"]["x"] = 100;
				root[object + to_string(i)]["Scale"]["y"] = 100;
			}
			if (selected[q].Name == 'B') {
				root[object + to_string(i)]["Scale"]["x"] = 100;
				root[object + to_string(i)]["Scale"]["y"] = 100;
			}
			if (selected[q].Name == 'R') {
				root[object + to_string(i)]["Scale"]["x"] = 50;
				root[object + to_string(i)]["Scale"]["y"] = 50;
			}
			if (selected[q].Name == 'G') {
				root[object + to_string(i)]["Scale"]["x"] = 50;
				root[object + to_string(i)]["Scale"]["y"] = 50;
			}
			if (selected[q].Name == 'C') {
				root[object + to_string(i)]["Scale"]["x"] = 70;
				root[object + to_string(i)]["Scale"]["y"] = 70;
			}
			if (selected[q].Name == 'W')
				root[object + to_string(i)]["Mass"] = 0.f;
			if (selected[q].Name == 'P')
				root[object + to_string(i)]["Mass"] = 1.f;
			if (selected[q].Name == 'B')
				root[object + to_string(i)]["Mass"] = 0.f;

			root[object + to_string(i)]["Rotation"] = 0.f;


			root[object + to_string(i)]["TextureDir"] = "wall.png";
			i++;
			j++;
		}
		else
		{
			j++;
		}


	}
	root["NumberOfObjects"] = i - 1;
	file.WriteFile(path, root);
}

void CheckSize(const int mapSizeX, const int mapSizeY)
{

	int ArraySize = (mapSizeX / 100) * (mapSizeY / 100);
	points = new glm::vec3[ArraySize];
	int defalut_x = -(mapSizeX / 2) + 50;
	int defalut_y = (mapSizeY / 2) - 50;
	int Scale = 100;
	for (int i = 0, j = 0, t = 0; i < ArraySize; i++, j++)
	{
		if (j == (mapSizeX / 100)) {
			j = 0;
			t++;
		}
		points[i].x = static_cast<float>((defalut_x + Scale *j) - 400);
		points[i].y = static_cast<float>((defalut_y - Scale * t) + 300);
		//points[i].x /= 2;
		//points[i].y /= 2;
	}

	sizeofobject = ArraySize;



}

void ImguiFree(void)
{
	delete points;
	delete selected;
	delete cppTextureitems;
	delete cppLevelitems;
	delete cppComponentitems;
	delete cppSounditems;
	delete cppArchetypesditems;
	
}