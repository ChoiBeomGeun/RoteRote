/******************************************************************************/
/*!
\file   LevelSelect.cpp
\author Kyungook.Park
\par    email: qkrruddn6680@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/04/19

Level selection stage source file
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "StateLists.h"
#include "LevelSelect.h"
#include <stdio.h>
#include "Object.h"
#include "Factory.h"
#include "Input.h"
#include "StateManager.h"
#include "Engine.h"
#include "Graphics.h"
#include  "LevelManager.h"
#include  "Timer.h"
#include "Application.h"
#include "SoundManager.h"
#include <vector>
#include "InGameLogic.h"

const int LastLevelNumber = LevelList::quit;
using namespace TE;
Object * LockObject = nullptr;
struct StringCompareLevel {

	bool operator()(const std::string& a, const std::string& b)

		const {

		//return std::atoi(&a.at(5)) > std::atoi(&b.at(5));
		return std::atoi(&a.at(5)) > std::atoi(&b.at(5));
		//return true;
	}
};

LevelSelect::LevelSelect()
{

}

LevelSelect::~LevelSelect()
{

}

void SavingLevelInfo(void) {


	
}
void LoadingLevelInfo(void) {





}


/*
Load texture & Initialize a value of texture to Levelpng[i]
*/
void LevelSelect::Load()
{
	 lsPauseSound = SOUNDMANAGER->LoadSound("Menu.mp3");
	 lsMoveSound = SOUNDMANAGER->LoadSound("Menumove.mp3");
	 lsSelectSound = SOUNDMANAGER->LoadSound("Menuselect.mp3");

	for (auto it : ENGINE->mVsLevelnamelist)
	{
		if (*it.begin() == 'l')
		{
			INGAMELOGIC->vsLevelList.push_back(it);
		}
	}



	std::sort(INGAMELOGIC->vsLevelList.begin(), INGAMELOGIC->vsLevelList.end(), StringCompareLevel());
	std::reverse(INGAMELOGIC->vsLevelList.begin(), INGAMELOGIC->vsLevelList.end());

	INGAMELOGIC->vsLevelList.erase(INGAMELOGIC->vsLevelList.begin());



	LEVELMANAGER->LoadingLevelInfo();
	LEVELMANAGER->LoadLevel("selectlevel.json");

	for (int i = 1; i <= LevelList::quit + 1; ++i)
		Levelpng[i - 1] = FACTORY->ObjectIDMap[3]->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("level" + NumberToString(i) + ".png");

	LevelList = STATEMANAGER->i_LevelSelect - 1;
	FACTORY->ObjectIDMap[2]->GetComponent<Sprite>()->depth = 1;

	LockObject = FACTORY->CreateArchetype(ReadingArchetype("Button.json"));
	LockObject->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("LevelSelectionLock.png");
        LockObject->GetComponent<Transform>()->SetPosition(glm::vec3(0, -120, 0));


	glm::vec3 num_pos(-100.f, 180.f, 0);

	for (auto indexOfnumber = 0; indexOfnumber <= quit; ++indexOfnumber)
	{
            if ((indexOfnumber != 0) && !(indexOfnumber % 5))
            {
                num_pos.x = -100.f;
                num_pos.y -= 50.f;
            }
            else if(indexOfnumber != 0)
            {
                num_pos.x += 50.f;
            }
            
            FACTORY->ObjectIDMap[indexOfnumber + 4]->GetComponent<Transform>()->scale = glm::vec3(30, 30, 1);
            FACTORY->ObjectIDMap[indexOfnumber + 4]->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id(NumberToString(indexOfnumber + 1) + ".png");
            FACTORY->ObjectIDMap[indexOfnumber + 4]->GetComponent<Transform>()->SetPosition(num_pos);

            if (indexOfnumber == quit)
            {
                FACTORY->ObjectIDMap[indexOfnumber + 4]->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("attachbox.png");
                FACTORY->ObjectIDMap[indexOfnumber + 4]->GetComponent<Transform>()->SetPosition(glm::vec3(0, -300, 0));
            }
	}

}


void LevelSelect::Init()
{
    	FACTORY->ObjectIDMap[3]->GetComponent<Sprite>()->m_TextureID = Levelpng[LevelList];

		IsRotating = false;
		IsLeftPressed = false;
		IsRightPreesed = false;
		LvlSelectCam.cameraSetting(CameraPosType::EN_LevelSelect);

        FACTORY->ObjectIDMap[LevelList + 4]->GetComponent<Sprite>()->ChangeColor(255, 255, 0, 255);
}

void LevelSelect::Update(float dt)
{

	std::vector<std::pair<std::string, bool>> test = STATEMANAGER->vsLevelListandclear;
	if (STATEMANAGER->vsLevelListandclear[LevelList].second != true)
		LockObject->GetComponent<Transform>()->SetScale(glm::vec3(0, -120, 0));
	else
		LockObject->GetComponent<Transform>()->SetScale(glm::vec3(300, 300, 0));

	if (IsConfirmationOn) {
		if (Input::IsTriggered(SDL_SCANCODE_Y))
			ENGINE->Quit();
		if (Input::IsTriggered(SDL_SCANCODE_N))
		{
			IsConfirmationOn = false;
			FACTORY->Destroy(obj_confirmation);
			return;
		}
	}
	if (IsConfirmationOn)
		return;

	if (std::abs(FACTORY->ObjectIDMap[2]->GetComponent<Transform>()->angle == 360.f))
	{
		FACTORY->ObjectIDMap[3]->GetComponent<Transform>()->angle = 0.f;

		FACTORY->ObjectIDMap[2]->GetComponent<Transform>()->angle = 0.f;
	}


	LvlSelectCam.Update(dt);
	if (!IsRotating)	{
		if (Input::IsPressed(SDL_SCANCODE_RIGHT)) {
                        
                        FACTORY->ObjectIDMap[LevelList + 4]->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);

			if (LevelList == LevelList::quit)
				LevelList = LevelList::level1;
			else
				++LevelList;

			selectAngle = FACTORY->ObjectIDMap[2]->GetComponent<Transform>()->angle;

                        FACTORY->ObjectIDMap[LevelList + 4]->GetComponent<Sprite>()->ChangeColor(255, 255, 0, 255);

			IsRotating = true;
			IsLeftPressed = true;
			IsRightPreesed = false;


			SOUNDMANAGER->PlaySounds(lsMoveSound, false);
		}
		if (Input::IsPressed(SDL_SCANCODE_LEFT)) {

                        FACTORY->ObjectIDMap[LevelList + 4]->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);

			if (LevelList == LevelList::level1)
				LevelList = LevelList::quit;
			else
				--LevelList;

			selectAngle = FACTORY->ObjectIDMap[2]->GetComponent<Transform>()->angle;

            FACTORY->ObjectIDMap[LevelList + 4]->GetComponent<Sprite>()->ChangeColor(255, 255, 0, 255);

			IsRotating = true;
			IsRightPreesed = true;
			IsLeftPressed = false;
			SOUNDMANAGER->PlaySounds(lsMoveSound, false);
		}
		

		if (Input::IsTriggered(SDL_SCANCODE_SPACE) || Input::IsTriggered(SDL_SCANCODE_RETURN)) {
			if (STATEMANAGER->vsLevelListandclear[LevelList].second != true) {
				if (LevelList != LevelList::quit)
				{
					STATEMANAGER->i_LevelSelect = LevelList + 1;
					std::string levelnumber = NumberToString(STATEMANAGER->i_LevelSelect);
					STATEMANAGER->Loadtolevelname = "level" + levelnumber + ".json";
					STATEMANAGER->MoveState(StatesList::Level1);
					SOUNDMANAGER->PlaySounds(lsSelectSound, false);
				}
				else {
					STATEMANAGER->MoveState(StatesList::Menu);
					SOUNDMANAGER->PlaySounds(lsSelectSound, false);
				}
			}

			
		}

		if (Input::IsTriggered(SDL_SCANCODE_ESCAPE)) {
			STATEMANAGER->MoveState(StatesList::Menu);
			SOUNDMANAGER->PlaySounds(lsSelectSound, false);
		}
	}

	if (IsRotating)
	{
		if (IsLeftPressed)
		{
			FACTORY->ObjectIDMap[2]->GetComponent<Transform>()->angle -= 300 * dt;
			FACTORY->ObjectIDMap[3]->GetComponent<Transform>()->angle -= 300 * dt;
			FACTORY->ObjectIDMap[3]->GetComponent<Transform>()->scale -= 600 * dt;
		}
		else
		{
			FACTORY->ObjectIDMap[2]->GetComponent<Transform>()->angle += 300 * dt;
			FACTORY->ObjectIDMap[3]->GetComponent<Transform>()->angle += 300 * dt;
			FACTORY->ObjectIDMap[3]->GetComponent<Transform>()->scale -= 600 * dt;
		}

		Rotation();
	}

}

void LevelSelect::Free(void)
{

}


void LevelSelect::Unload()
{
}

void LevelSelect::Rotation(void)
{
	if (IsRotating)		{
		if (FACTORY->ObjectIDMap[2]->GetComponent<Transform>()->angle - selectAngle > 0) {
			if (std::abs(FACTORY->ObjectIDMap[2]->GetComponent<Transform>()->angle - selectAngle) > 90) {
				FACTORY->ObjectIDMap[2]->GetComponent<Transform>()->angle = LevelList * 90.f;
				FACTORY->ObjectIDMap[3]->GetComponent<Transform>()->angle = 0.f;
				FACTORY->ObjectIDMap[3]->GetComponent<Transform>()->scale = glm::vec3(150, 150, 1);
				IsRotating = false;
				FACTORY->ObjectIDMap[3]->GetComponent<Sprite>()->m_TextureID = Levelpng[LevelList];
			}

		}
		else if (FACTORY->ObjectIDMap[2]->GetComponent<Transform>()->angle - selectAngle < 0) {
			if (std::abs(FACTORY->ObjectIDMap[2]->GetComponent<Transform>()->angle - selectAngle) > 90) {
				FACTORY->ObjectIDMap[2]->GetComponent<Transform>()->angle = -LevelList * 90.f;
				FACTORY->ObjectIDMap[3]->GetComponent<Transform>()->angle = 0.f;
				FACTORY->ObjectIDMap[3]->GetComponent<Transform>()->scale = glm::vec3(150, 150, 1);
				IsRotating = false;
				FACTORY->ObjectIDMap[3]->GetComponent<Sprite>()->m_TextureID = Levelpng[LevelList];
			}

		}

	}

}
