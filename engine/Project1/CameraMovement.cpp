#include "CameraMovement.h"
#include "Factory.h"
#include "Physics.h"
#include "StateManager.h"


#include <random>
#include <iostream>


namespace TE {
	CameraAction::CameraAction()
	{
		shakeDuration = 3.0f;
		decreaseFactor = 1.0f;
		shakeAmount = 3.0f;
		isCamToPlayer = false;
		_camPaceSpeed = 10.f;
		istest = true;
	}
	CameraAction::~CameraAction()
	{
	
	}
	void CameraAction::ShakeCamera(float dt)
	{
		if (shakeDuration > 0)
		{
			std::random_device rd;

			std::mt19937 gen(rd());
			std::uniform_real_distribution<> dis(-10.0, 10.0);
			CAMERA->cameraPos = cameraOriginPos + glm::vec3(dis(gen) * shakeAmount, dis(gen) * shakeAmount, 1);
			std::cout << "Camera.x : " << CAMERA->cameraPos.x << '\n';
			std::cout << "Camera.y : " << CAMERA->cameraPos.y << '\n';

			CAMERA->cameraPos.z = 999.f;
			shakeDuration -= dt * decreaseFactor;
		}
		else
		{

			CAMERA->IsCameraShaking = false;
			shakeDuration = 0.f;
			CAMERA->cameraPos = cameraOriginPos;
		}
	}

	void CameraAction::FollowPlayer(glm::vec3 * cameraPos, glm::vec3 * cameraScale, float dt)
	{
		glm::vec3 displacement = FACTORY->GetPlayer()->GetComponent<Transform>()->position - CAMERA->cameraPos;
		cameraOriginPos = glm::vec3();
		//	auto distanceFromStart = glm::length(displacement);

		glm::vec3 _camPacedirction = -displacement;

		//_camPacedirction = glm::normalize(_camPacedirction);

		//	cameraObj->GetComponent<Transform>()->position -=
		CAMERA->cameraPos -= _camPacedirction  * _camPaceSpeed * dt;
		CAMERA->cameraPos.z = 999.f;

		*cameraPos = CAMERA->cameraPos;

		if (PHYSICS->SimpleRectvsRectCollisionCheck(cameraPos,cameraScale,
			&FACTORY->GetPlayer()->GetComponent<Transform>()->position,
			&FACTORY->GetPlayer()->GetComponent<Transform>()->scale))
		{
			CAMERA->cameraPos = FACTORY->GetPlayer()->GetComponent<Transform>()->position;
			CAMERA->cameraPos.z = 999.f;
			CAMERA->isCentered = false;
			isCamToPlayer = false;
		}
	}

	int CameraAction::Rotating90DegreesCam()
	{
		// when degree is 0 
		if (CAMERA->cameraUp.x == 0 && CAMERA->cameraUp.y >= 1)
		{
			if (FACTORY->LeftBoundary()->HasComponent<Transform>())
			{
				CAMERA->CenterOfCamera.x = FACTORY->LeftBoundary()->GetComponent<Transform>()->position.x + (FACTORY->RightBoundary()->GetComponent<Transform>()->position.x - FACTORY->LeftBoundary()->GetComponent<Transform>()->position.x) *.5f;
				CAMERA->CenterOfCamera.y = FACTORY->DownBoundary()->GetComponent<Transform>()->position.y + (FACTORY->UpBoundary()->GetComponent<Transform>()->position.y - FACTORY->DownBoundary()->GetComponent<Transform>()->position.y)*.5f;
			}
			return EN_0;
		}
		// when degree is 90
		else if (CAMERA->cameraUp.x == 1 && CAMERA->cameraUp.y >= 0)
		{
			//CAMERA->CenterOfCamera.x = FACTORY->LeftBoundary()->GetComponent<Transform>()->position.x + (FACTORY->RightBoundary()->GetComponent<Transform>()->position.x - FACTORY->LeftBoundary()->GetComponent<Transform>()->position.x) *.5f;
			
			// Left boundary has become as Up boundry , right boundary -> down boundary
			CAMERA->CenterOfCamera.x = FACTORY->UpBoundary()->GetComponent<Transform>()->position.x + (FACTORY->DownBoundary()->GetComponent<Transform>()->position.x - FACTORY->UpBoundary()->GetComponent<Transform>()->position.x) *.5f;
			
			//CAMERA->CenterOfCamera.y = FACTORY->DownBoundary()->GetComponent<Transform>()->position.y + (FACTORY->UpBoundary()->GetComponent<Transform>()->position.y - FACTORY->DownBoundary()->GetComponent<Transform>()->position.y)*.5f;
			// changed;
			CAMERA->CenterOfCamera.y = FACTORY->LeftBoundary()->GetComponent<Transform>()->position.y + (FACTORY->RightBoundary()->GetComponent<Transform>()->position.y - FACTORY->LeftBoundary()->GetComponent<Transform>()->position.y)*.5f;
						
			return EN_90;

		}
		// when degree is 180
		else if (CAMERA->cameraUp.x == 0 && CAMERA->cameraUp.y == -1)
		{
			return EN_180;

		}
		// when degree is 270
		else if (CAMERA->cameraUp.x == -1 && CAMERA->cameraUp.y == 0)
		{
			return EN_270;

		}
	}

	void CameraAction::Rotating180DegreesCam()
	{
		// when degree is 0 
		if (CAMERA->cameraUp.x == 0 && CAMERA->cameraUp.y >= 1)
		{

		}
		// when degree is 90
		else if (CAMERA->cameraUp.x == 1 && CAMERA->cameraUp.y >= 0)
		{

		}
		// when degree is 180
		else if (CAMERA->cameraUp.x == 0 && CAMERA->cameraUp.y == -1)
		{

		}
		// when degree is 270
		else if (CAMERA->cameraUp.x == -1 && CAMERA->cameraUp.y == 0)
		{

		}
	}

	void CameraAction::HorizontalLockCam(CameraRotation type)
	{
		if (type == CameraRotation::EN_90)
		{
			glm::vec3 rightblock, leftblock, upblock, downblock;
			// this is current block of map 
			upblock = FACTORY->RightBoundary()->GetComponent<Transform>()->position;
			downblock = FACTORY->LeftBoundary()->GetComponent<Transform>()->position;
			leftblock = FACTORY->UpBoundary()->GetComponent<Transform>()->position;
			rightblock = FACTORY->DownBoundary()->GetComponent<Transform>()->position;
	
			CAMERA->CenterOfCamera.y = leftblock.y + ((rightblock.y - leftblock.y)*.5f);
			CAMERA->CenterOfCamera.x = downblock.x + ((upblock.x - downblock.x)*.5f);

			float UpYLimit, DownYLimit;
			// 90 degrees rotated x becomes Y 
			// only this applies to 90 degrees, 
			UpYLimit = (upblock.x - std::abs((upblock.x - CAMERA->CenterOfCamera.x))*.5f);
			DownYLimit = (downblock.x + std::abs(CAMERA->CenterOfCamera.x - downblock.x)*.5f);
			CAMERA->cameraPos.y = CAMERA->CenterOfCamera.y;

			if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x >= UpYLimit)
				CAMERA->cameraPos.x = UpYLimit;
			else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x <= DownYLimit)
				CAMERA->cameraPos.x = DownYLimit;
			else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x < UpYLimit || FACTORY->GetPlayer()->GetComponent<Transform>()->position.x > DownYLimit)
				CAMERA->cameraPos.x = FACTORY->GetPlayer()->GetComponent<Transform>()->position.x;
			CAMERA->cameraPos.z = 999.f;
		}
		else if (type == CameraRotation::EN_270)
		{
			glm::vec3 rightblock, leftblock, upblock, downblock;
			// this is current block of map 
			upblock = FACTORY->LeftBoundary()->GetComponent<Transform>()->position;
			downblock = FACTORY->RightBoundary()->GetComponent<Transform>()->position;
			leftblock = FACTORY->DownBoundary()->GetComponent<Transform>()->position;
			rightblock = FACTORY->UpBoundary()->GetComponent<Transform>()->position;

			CAMERA->CenterOfCamera.y = leftblock.y + ((rightblock.y - leftblock.y)*.5f);
			CAMERA->CenterOfCamera.x = downblock.x + ((upblock.x - downblock.x)*.5f);

			float UpYLimit, DownYLimit;
			// 90 degrees rotated x becomes Y 
			// only this applies to 90 degrees, 
			UpYLimit = (upblock.x + std::abs((upblock.x - CAMERA->CenterOfCamera.x))*.5f);
			DownYLimit = (downblock.x - std::abs(CAMERA->CenterOfCamera.x - downblock.x)*.5f);
			CAMERA->cameraPos.y = CAMERA->CenterOfCamera.y;

			std::cout << "UPYLIMIT. x " << UpYLimit << std::endl;
			std::cout << "DOWNYLIMIT. x " << DownYLimit << std::endl;
			if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x <= UpYLimit)
				CAMERA->cameraPos.x = UpYLimit;
			else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x >= DownYLimit)
				CAMERA->cameraPos.x = DownYLimit;
			else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x > UpYLimit || FACTORY->GetPlayer()->GetComponent<Transform>()->position.x < DownYLimit)
				CAMERA->cameraPos.x = FACTORY->GetPlayer()->GetComponent<Transform>()->position.x;
			CAMERA->cameraPos.z = 999.f;
		}
		/*
		if (istest)
		{
			CAMERA->cameraPos = glm::vec3(CAMERA->cameraPos.y, CAMERA->CenterOfCamera.x, 999.f);
			istest = false;
		}*/
		/*float cameraOriginalPos = CAMERA->cameraPos.x;
		DEBUG_ASSERT(CAMERA->cameraPos.x != cameraOriginalPos, "CAMERA X SHOULD BE FIXED");*/
	}

	void CameraAction::VerticalLockCam(CameraRotation type)
	{
		float cameraOriginalPos = CAMERA->cameraPos.y;
		DEBUG_ASSERT(CAMERA->cameraPos.y != cameraOriginalPos, "CAMERA Y SHOULD BE FIXED");
	}

	void CameraAction::cameraSetting(CameraPosType camPosType)
	{
		switch (camPosType)
		{
		case EN_Default:
			CAMERA->cameraPos = glm::vec3(0, 0, 999.f);
			break;
		case EN_Splash:
			CAMERA->cameraPos = glm::vec3(0, 0, 999.f);
			break;
		case EN_Menu:
			CAMERA->cameraPos = glm::vec3(0, 0, 999.f);
			break;
		case EN_LevelSelect:
			CAMERA->cameraPos = glm::vec3(0, 0, 999.f);
			break;
		case EN_playerPos:
			CAMERA->cameraPos = glm::vec3(glm::vec3(FACTORY->GetPlayer()->GetComponent<Transform>()->position.x , FACTORY->GetPlayer()->GetComponent<Transform>()->position.y ,0));
			break;
		case EN_BOUNDARY:
			CAMERA->cameraPos = glm::vec3(glm::vec3(CAMERA->CenterOfCamera.x, CAMERA->CenterOfCamera.y, CAMERA->cameraPos.z));
		case End:
			break;
		}	
	}
	bool CameraAction::DisplayTheWholeMap() {
		if (!FACTORY->GetPlayer())
			return false;
		CAMERA->CenterOfCamera.x = FACTORY->LeftBoundary()->GetComponent<Transform>()->position.x + (FACTORY->RightBoundary()->GetComponent<Transform>()->position.x - FACTORY->LeftBoundary()->GetComponent<Transform>()->position.x) *.5f;
		CAMERA->CenterOfCamera.y = FACTORY->DownBoundary()->GetComponent<Transform>()->position.y + (FACTORY->UpBoundary()->GetComponent<Transform>()->position.y - FACTORY->DownBoundary()->GetComponent<Transform>()->position.y)*.5f;
		CAMERA->cameraPos = glm::vec3(glm::vec2(CAMERA->CenterOfCamera.x, CAMERA->CenterOfCamera.y), CAMERA->cameraPos.z);
		return true;
	}

	void CameraAction::moveCameraPos(glm::vec2 cameraPOS)
	{
		CAMERA->cameraPos += glm::vec3(cameraPOS, 0);
	}

	void CameraAction::Update(float dt)
	{
		if (STATEMANAGER->i_LevelSelect == 4)
		{
			switch (Rotating90DegreesCam())
			{
			case EN_0:
				break;
			case EN_90:
				HorizontalLockCam(CameraRotation::EN_90);
				break;
			case EN_180:
				break;
			case EN_270:
				HorizontalLockCam(CameraRotation::EN_270);
				break;
			default: break;

			}
		}
		CAMERA->lookat(CAMERA->cameraPos, CAMERA->cameraTarget, CAMERA->cameraUp);
	}

	/*void Level1::lookAtMap()
	{
		CAMERA->CenterOfCamera.x = FACTORY->LeftBoundary()->GetComponent<Transform>()->position.x + (FACTORY->RightBoundary()->GetComponent<Transform>()->position.x - FACTORY->LeftBoundary()->GetComponent<Transform>()->position.x) *.5f;
		CAMERA->CenterOfCamera.y = FACTORY->DownBoundary()->GetComponent<Transform>()->position.y + (FACTORY->UpBoundary()->GetComponent<Transform>()->position.y - FACTORY->DownBoundary()->GetComponent<Transform>()->position.y)*.5f;
		std::cout << "CenterOfX: " << CAMERA->CenterOfCamera.x << '\n';
		std::cout << "CenterOfY: " << CAMERA->CenterOfCamera.y << '\n';

		CAMERA->cameraPos = glm::vec3(glm::vec2(CAMERA->CenterOfCamera.x, CAMERA->CenterOfCamera.y), CAMERA->cameraPos.z);
		CAMERA->cameraPos.z = 999.f;
	}*/


}