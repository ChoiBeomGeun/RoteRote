#include "ReplayLogic.h" 
#include "StateManager.h"
#include "Factory.h"
#include "Object.h"
#include "Input.h"
#include "SoundManager.h"
#include "LevelManager.h"
#include "Level1.h"
#include "Trigger.h"
#include "ParticleManager.h"
using namespace TE;
unsigned int index = 0;
void ButtonLogic(void);
void ReStartgame3(void);
void LevelSelect3(void);
void Backgame3(void);
void(*pausefunction3[3])(void) = { Backgame3,ReStartgame3,LevelSelect3 };
glm::vec3 Vec3Buttonscale3 = glm::vec3(.2f, .2f, 0);
int pauseindex3 = 0;
glm::vec3 Vec3buttonPostion3 = glm::vec3(.85f, .1f, 0);
unsigned int rlmove;
unsigned int rlselect;
unsigned int rlback;
unsigned int rlwin;
bool SettingItor = false;
Object * replayer ;
Object * oReBackbutton;
Object * oReRestartbutton;
Object * oReLevelbutton;
Object * ReeplayImage;
Object * ReeplayUI;
std::queue<ReplayerInfo> itor;
static bool IsButtonAvailable = true;

void ReStartgame3(void)
{
	//->LoadLevel(STATEMANAGER->Loadtolevelname);
	//STATEMANAGER->ReplayPosition.clear();
	//STATEMANAGER->b_IsReplay = false;
	//STATEMANAGER->b_Relplay = false;
	//STATEMANAGER->b_IsPauseFirst = true;
	//STATEMANAGER->Restart();
	while (!STATEMANAGER->Replayerinfo.empty())
		STATEMANAGER->Replayerinfo.pop();
	FreeReplayer();
	STATEMANAGER->Restart();
	PARTICLEMANAGER->Initialize();
	//PARTICLEMANAGER->GetEmitters()[0].Initialize();
}

void LevelSelect3(void)
{
	FreeReplayer();
	STATEMANAGER->b_IsReplay = false;
	STATEMANAGER->b_Relplay = false;
	STATEMANAGER->b_IsPauseFirst = true;
	STATEMANAGER->MoveState(3);
	while (!STATEMANAGER->Replayerinfo.empty())
		STATEMANAGER->Replayerinfo.pop();
	PARTICLEMANAGER->Initialize();
	//PARTICLEMANAGER->GetEmitters()[0].Initialize();
}
//Move next
void Backgame3(void)
{
	FreeReplayer();
	if (STATEMANAGER->i_LevelSelect == 13) {
		LevelSelect3();
		return;
	}


	CAMERA->cameraUp.x = 0;
	CAMERA->cameraUp.y = 1;

	std::string level = "level";
	level += std::to_string(++STATEMANAGER->i_LevelSelect);
	level += +".json";
	STATEMANAGER->Loadtolevelname = level;
	STATEMANAGER->b_IsReplay = false;
	STATEMANAGER->b_Relplay = false;
	STATEMANAGER->b_IsPauseFirst = true;
	STATEMANAGER->MoveState(4);
	PARTICLEMANAGER->Initialize();
	//PARTICLEMANAGER->GetEmitters()[0].Initialize();
}


void MakeReplayerUI(void) {
	SOUNDMANAGER->DeleteSounds();
	FACTORY->DestroyAllObjects();
	LEVELMANAGER->LoadLevel(STATEMANAGER->Loadtolevelname);
	oReBackbutton = FACTORY->CreateHUD(glm::vec3(Vec3buttonPostion3.x, Vec3buttonPostion3.y + .5f, Vec3buttonPostion3.z), Vec3Buttonscale3);
	oReBackbutton->GetComponent<Sprite>()->texture_load("nextlevel.png");
	oReRestartbutton = FACTORY->CreateHUD(glm::vec3(Vec3buttonPostion3.x, Vec3buttonPostion3.y, Vec3buttonPostion3.z), Vec3Buttonscale3);
	oReRestartbutton->GetComponent<Sprite>()->texture_load("retry.png");
	oReLevelbutton = FACTORY->CreateHUD(glm::vec3(Vec3buttonPostion3.x, Vec3buttonPostion3.y - .5f, Vec3buttonPostion3.z), Vec3Buttonscale3);
	oReLevelbutton->GetComponent<Sprite>()->texture_load("levelselect.png");
	ReeplayImage = FACTORY->CreateHUD(glm::vec3(0, 0, Vec3buttonPostion3.z), glm::vec3(2, 2, 0));
	ReeplayImage->GetComponent<Sprite>()->texture_load("replay.png");
	//IsButtonAvailable = true;
	replayer = FACTORY->GetPlayer();
//	STATEMANAGER->Replayerinfo.reverse();
//	itor = STATEMANAGER->Replayerinfo.;
	STATEMANAGER->ReplayInit = false;
	TRIGGERLOGIC->Free();
	TRIGGERLOGIC->Initialize();
	IsButtonAvailable = true;
	itor = STATEMANAGER->Replayerinfo;
	rlmove = SOUNDMANAGER->LoadSound("menumove.mp3");
	rlback = SOUNDMANAGER->LoadSound("menuselect.mp3");
	rlselect = SOUNDMANAGER->LoadSound("menuselect.mp3");
}

void SetReplayer(void) {

	
	
	replayer->GetComponent<Transform>()->SetPosition(itor.front().Pos);
	replayer->GetComponent<Animation>()->setFrame(itor.front().aniframe);
	replayer->GetComponent<Animation>()->setTime(itor.front().anitime);
	replayer->GetComponent<Animation>()->setFlipX(itor.front().mouseinfo);
	
	//PARTICLEMANAGER->GetEmitters()[0].Initialize();
	itor.pop();
	if (itor.size() <=0) {
		
		//itor = STATEMANAGER->Replayerinfo.begin();
		FACTORY->DestroyAllObjects();
		LEVELMANAGER->LoadLevel(STATEMANAGER->Loadtolevelname);
		TRIGGERLOGIC->Initialize();
		oReBackbutton = FACTORY->CreateHUD(glm::vec3(Vec3buttonPostion3.x, Vec3buttonPostion3.y + .5f, Vec3buttonPostion3.z), Vec3Buttonscale3);
		oReBackbutton->GetComponent<Sprite>()->texture_load("nextlevel.png");
		oReRestartbutton = FACTORY->CreateHUD(glm::vec3(Vec3buttonPostion3.x, Vec3buttonPostion3.y, Vec3buttonPostion3.z), Vec3Buttonscale3);
		oReRestartbutton->GetComponent<Sprite>()->texture_load("retry.png");
		oReLevelbutton = FACTORY->CreateHUD(glm::vec3(Vec3buttonPostion3.x, Vec3buttonPostion3.y - .5f, Vec3buttonPostion3.z), Vec3Buttonscale3);
		oReLevelbutton->GetComponent<Sprite>()->texture_load("levelselect.png");
		ReeplayImage = FACTORY->CreateHUD(glm::vec3(0, 0, Vec3buttonPostion3.z), glm::vec3(2, 2, 0));
		ReeplayImage->GetComponent<Sprite>()->texture_load("replay.png");
		replayer = FACTORY->GetPlayer();
		CAMERA->cameraUp.x = 0;
		CAMERA->cameraUp.y = 1;
		PHYSICS->gravityScale = -20.f;
		PHYSICS->GravityType = Gravity::y_Minus;
		PHYSICS->gravity = glm::vec3(0, PHYSICS->gravityScale, 0);
		itor = STATEMANAGER->Replayerinfo;
		//PARTICLEMANAGER->Initialize();
	}


	if (IsButtonAvailable)
		ButtonLogic();
	else
		return;

}

void ButtonLogic(void)
{
	if (Input::IsTriggered(SDL_SCANCODE_DOWN))
		if (pauseindex3 <= 3)
		{
			SOUNDMANAGER->PlaySounds(rlmove, false);
			pauseindex3++;


			if (pauseindex3 == 3)
				pauseindex3 = 0;
		}

	if (Input::IsTriggered(SDL_SCANCODE_UP))
		if (pauseindex3 >= 0)
		{
			SOUNDMANAGER->PlaySounds(rlmove, false);
			pauseindex3--;

			if (pauseindex3 == -1)
			{

				pauseindex3 = 2;
			}
		}


	if ((Input::IsTriggered(SDL_SCANCODE_SPACE) || (Input::IsTriggered(SDL_SCANCODE_RETURN)))
		&& (!STATEMANAGER->b_IsRotating)
		)
	{
		SOUNDMANAGER->PlaySounds(rlselect, false);
		pausefunction3[pauseindex3]();
		return;
	}






	if (oReBackbutton&& oReLevelbutton && oReRestartbutton) {
		switch (pauseindex3)
		{
		case 1:
			oReRestartbutton->GetComponent<Sprite>()->ChangeColor(255, 255, 0, 255);
			oReBackbutton->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
			oReLevelbutton->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
			break;
		case 0:
			oReBackbutton->GetComponent<Sprite>()->ChangeColor(255, 255, 0, 255);
			oReRestartbutton->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
			oReLevelbutton->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
			break;
		case 2:
			oReLevelbutton->GetComponent<Sprite>()->ChangeColor(255, 255, 0, 255);
			oReRestartbutton->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
			oReBackbutton->GetComponent<Sprite>()->ChangeColor(255, 255, 255, 255);
			break;
		}
	}


}

void FreeReplayer(void) {


	FACTORY->Destroy(oReBackbutton);
	FACTORY->Destroy(oReLevelbutton);
	FACTORY->Destroy(oReRestartbutton);
	FACTORY->Destroy(ReeplayImage);
	STATEMANAGER->b_IsReplay = false;
	IsButtonAvailable = false;
	STATEMANAGER->b_IsReplayStart = true;
	//FACTORY->GetPlayer()->transform->SetPosition(STATEMANAGER->InitplayerPos) ;
	//STATEMANAGER->Replayerinfo.clea
	STATEMANAGER->b_Relplay = false;
	STATEMANAGER->ReplayInit = true;
	STATEMANAGER->b_IsGameLevel = true;
//	STATEMANAGER->Replayerinfo.clear();
}