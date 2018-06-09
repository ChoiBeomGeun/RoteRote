/******************************************************************************/
/*!
\file   StateManager.cpp
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

State Manager source file in the engine system class
*/
/******************************************************************************/

#include "StateManager.h"
#include "DebugUtil.h"
#include  "Factory.h"

#include "SoundManager.h"
#include  "DebugUtil.h"
#include "Input.h"
#include "Timer.h"
#include "ParticleManager.h"
using namespace TE;

bool IsFadingOut = true;
bool IsFadingIn = true;
static float itransvalue = 0;
static float transvalue = 255;
void FadeIn(void)
{

	
	for(auto allobj:FACTORY->ObjectIDMap)
	{
		if (allobj.second->objectstyle == Objectstyle::BackGround)
			continue;
		allobj.second->GetComponent<Sprite>()->ChangeAvalue(itransvalue);
		//PARTICLEMANAGER->turn_on_particles(false);
		if (itransvalue == 255) {
			IsFadingIn = false;
		//	itransvalue = 255;
		
		}
	
	}
	itransvalue += 5;

}


void FadeOut(void)
{


	for (auto allobj : FACTORY->ObjectIDMap)
	{
		allobj.second->GetComponent<Sprite>()->ChangeAvalue(transvalue);

		if (transvalue == 0) {
			IsFadingOut = false;

			
		}

	}
	transvalue -= 5;

}
namespace TE {


	StateManager * STATEMANAGER = nullptr;


}
StateManager::StateManager()
{


	DEBUG_ASSERT(STATEMANAGER != nullptr, "STATEMANAGER already created! There can only be 1");
	STATEMANAGER = this;
	i_NumberOfstage = 0;


	i_CurrentStateNumber = State_1;




}
StateManager::~StateManager()
{
}
void StateManager::AddState(BaseStage* states)
{
	states->NumberOfStage = i_NumberOfstage;
	v_StatesLists.push_back(states);
	i_NumberOfstage++;
}

void StateManager::SetStartState(int StateNumber)
{

	DEBUG_ASSERT(StateNumber > i_NumberOfstage || StateNumber<0, "Out of State Number");

	i_CurrentStateNumber = StateNumber;
}

void StateManager::Initialize(void)
{


	i_CurrentStateNumber = 0;
	b_IsRestart = false;
	b_IsQuit = false;
	b_IsNext = false;
	b_IsFirst = true;
	b_IsGravityChanged = false;
	b_IsReplayFirst = true;
	//activestage = Statevec[0];
}

void StateManager::Update(float dt)
{

	if (forceLevelLoading)
	{
		this->v_StatesLists[i_CurrentStateNumber]->Free();
		this->v_StatesLists[i_CurrentStateNumber]->Load();
		this->v_StatesLists[i_CurrentStateNumber]->Init();
		forceLevelLoading = false;
	}


	if (b_IsFirst)
	{
		this->v_StatesLists[i_CurrentStateNumber]->Load();
		this->v_StatesLists[i_CurrentStateNumber]->Init();
		b_IsFirst = false;
	}
	if (b_IsRestart && !CAMERA->IsCameraShaking)
	{

		if (IsFadingOut) {
			FadeOut();
			return;
		}
		IsFadingOut = true;
		IsFadingIn = true;
		transvalue = 255;
		FACTORY->DestroyAllObjects();
		CAMERA->cameraUp.x = 0;
		CAMERA->cameraUp.y = 1;
		
			SOUNDMANAGER->DeleteSounds();

		this->v_StatesLists[i_CurrentStateNumber]->Free();
		this->v_StatesLists[i_CurrentStateNumber]->Init();
		b_IsRestart = false;
		b_IsPauseOn = false;
	}

	if (b_Replayrestart)
	{
		if (IsFadingOut) {
			FadeOut();
			return;
		}
		IsFadingOut = true;
		IsFadingIn = true;
		transvalue = 255;
		FACTORY->DestroyAllObjects();
		this->v_StatesLists[i_ReplayStageNumber]->Free();
		this->v_StatesLists[i_ReplayStageNumber]->Init();
		b_IsRestart = false;
		b_IsPauseOn = false;

	}

	if (b_IsNext)
	{
	
		if (IsFadingOut) {
			FadeOut();
			return;
		}
		IsFadingOut = true;
		IsFadingIn = true;
		transvalue = 255;
		FACTORY;
		SOUNDMANAGER->DeleteSounds();
		FACTORY->DestroyAllObjects();
		this->v_StatesLists[i_CurrentStateNumber]->Free();
		this->v_StatesLists[i_CurrentStateNumber]->Unload();
		i_CurrentStateNumber++;

		this->v_StatesLists[i_CurrentStateNumber]->Load();
		this->v_StatesLists[i_CurrentStateNumber]->Init();
		b_IsNext = false;
	}


	if (b_IsMoveState)
	{
		if (IsFadingOut) {
			FadeOut();
			return;
		}

		IsFadingOut = true;
		IsFadingIn = true;
		transvalue = 255;
		SOUNDMANAGER->DeleteSounds();
		FACTORY->DestroyAllObjects();

		this->v_StatesLists[i_CurrentStateNumber]->Free();
		this->v_StatesLists[i_CurrentStateNumber]->Unload();


		i_CurrentStateNumber = i_StageToMove;
		this->v_StatesLists[i_CurrentStateNumber]->Load();
		this->v_StatesLists[i_CurrentStateNumber]->Init();
		b_IsMoveState = false;
		b_Relplay = false;
	}

	if (b_IsPauseOn)
	{
		if (b_IsPauseFirst) {
			this->v_StatesLists[i_PauseStageNumber]->Load();
			this->v_StatesLists[i_PauseStageNumber]->Init();
			b_IsPauseFirst = false;
		}


	}
	if (b_Relplay)
	{


		if (b_IsReplayFirst) {


			this->v_StatesLists[i_ReplayStageNumber]->Load();
			this->v_StatesLists[i_ReplayStageNumber]->Init();

			b_IsReplayFirst = false;
		}


	}
	if (b_Relplay) {
		FACTORY;
		this->v_StatesLists[i_ReplayStageNumber]->Update(dt);

		if (IsFadingIn) {
			FadeIn();
			return;
		}

		itransvalue = 0;
	}
	if (!b_IsPauseOn && !b_Relplay) {

	
		
		FACTORY;
		this->v_StatesLists[i_CurrentStateNumber]->Update(dt);

		if (IsFadingIn) {
			FadeIn();
			return;
		}

		itransvalue = 0;
	}
	if (b_IsPauseOn) {
		FACTORY;
		this->v_StatesLists[i_PauseStageNumber]->Update(dt);

		if (IsFadingIn) {
			FadeIn();
			return;
		}

		itransvalue = 0;

	}



	if (!b_IsRotating && b_IsGameLevel && !b_IsReplay)
	{
		if (Input::IsTriggered(SDL_SCANCODE_ESCAPE) && APP->b_Lose == false)
			PauseReturn();
		if (APP->windowIsActive) {
			PauseReturn();
			APP->windowIsActive = false;
		}



	}






}

void StateManager::PauseReturn(void)
{


	if (b_IsPauseOn) {
		b_IsPauseOn = false;
		this->v_StatesLists[i_PauseStageNumber]->Free();
		this->v_StatesLists[i_PauseStageNumber]->Unload();
	}
	else
	{
		b_IsPauseOn = true;
		b_IsPauseFirst = true;
	}



}

void TE::StateManager::Replay(void)
{
}

void TE::StateManager::ReplayReturn(void)
{
	if (b_Relplay) {
		b_Relplay = false;
		this->v_StatesLists[i_ReplayStageNumber]->Free();
		this->v_StatesLists[i_ReplayStageNumber]->Unload();
	}
	else
	{
		b_Relplay = true;
		b_IsReplayFirst = true;
	}
}

void TE::StateManager::DestoryAllStates(void)
{
	for (unsigned i = 0; i <v_StatesLists.size(); ++i)
	{
		delete v_StatesLists[i];

	}
}




void StateManager::MoveState(int StateNumber)
{

	i_StageToMove = StateNumber;
	b_IsMoveState = true;
}

void StateManager::RestartReplay(void)
{
	b_Replayrestart = true;
}


void StateManager::Quit(void)
{
	b_IsQuit = true;
}

void StateManager::Restart(void)
{

	b_IsRestart = true;
}
void StateManager::Pause(void)
{

	b_IsPauseOn = true;
}
void StateManager::Next()
{
	b_IsNext = true;
}


