#include "CameraMovement.h"
#include "Factory.h"
#include "Physics.h"
#include "StateManager.h"


#include <random>
#include <iostream>

float CameraPosz = 999.f;
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

	void CameraAction::defaultCameraSetting()
	{
		CAMERA->cameraPos = glm::vec3(0, 0, 999.f);
	}

	int CameraAction::RotatingCam()
	{
		// when degree is 0 
		if (CAMERA->cameraUp.x == 0 && CAMERA->cameraUp.y == 1)
		{
			return EN_0;
		}
		// when degree is 90
		else if (CAMERA->cameraUp.x == 1 && CAMERA->cameraUp.y == 0)
		{
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
	
	void CameraAction::ControlCamMovement(int type)
	{
		glm::vec3 rightblock = glm::vec3(0), leftblock = glm::vec3(0), upblock = glm::vec3(0), downblock = glm::vec3(0);
		if (FACTORY->GetPlayer())
		{
			if (type == CameraRotation::EN_0)
			{
				// this is current block of map 
				upblock = FACTORY->UpBoundary()->GetComponent<Transform>()->position;
				downblock = FACTORY->DownBoundary()->GetComponent<Transform>()->position;
				rightblock = FACTORY->RightBoundary()->GetComponent<Transform>()->position;
				leftblock = FACTORY->LeftBoundary()->GetComponent<Transform>()->position;


				if (STATEMANAGER->Loadtolevelname == "level1.json")
				{
					CAMERA->CenterOfCamera.x = downblock.x;
					CAMERA->CenterOfCamera.y = downblock.y + APP->_screenHeight*.25f;

					CAMERA->cameraPos = glm::vec3(CAMERA->CenterOfCamera, CameraPosz);
					float LeftXLimit, RightXLimit;
					LeftXLimit = downblock.x - APP->_screenWidth*.25f;
					RightXLimit = downblock.x + APP->_screenWidth *.25f;
					
					if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x <= LeftXLimit)
						CAMERA->cameraPos.x = LeftXLimit;
					else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x >= RightXLimit)
						CAMERA->cameraPos.x = RightXLimit;
					else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x > LeftXLimit || FACTORY->GetPlayer()->GetComponent<Transform>()->position.x < RightXLimit)
						CAMERA->cameraPos.x = FACTORY->GetPlayer()->GetComponent<Transform>()->position.x;
					
					CAMERA->cameraPos.z = CameraPosz;
				}
				else {
					CAMERA->CenterOfCamera.x = leftblock.x + (std::abs(leftblock.x - rightblock.x)*.5f);
					CAMERA->CenterOfCamera.y = downblock.y + (std::abs(upblock.y - downblock.y)*.5f);

					CAMERA->cameraPos = glm::vec3(CAMERA->CenterOfCamera, CameraPosz);
					// To do: seprate sections of map 
					float UpYLimit, DownYLimit, LeftXLimit, RightXLimit;

					UpYLimit = upblock.y - std::abs(upblock.y - CAMERA->CenterOfCamera.y)*.5f;
					DownYLimit = downblock.y + std::abs(CAMERA->CenterOfCamera.y - downblock.y)*.5f;
					LeftXLimit = leftblock.x + std::abs(CAMERA->CenterOfCamera.x - leftblock.x)*.5f;
					RightXLimit = rightblock.x - std::abs(rightblock.x - CAMERA->CenterOfCamera.x)*.5f;


					if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x <= LeftXLimit)			// when player is on the left.
						CAMERA->cameraPos.x = LeftXLimit;
					else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x >= RightXLimit)	// when player is on the right
						CAMERA->cameraPos.x = RightXLimit;
					else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x > LeftXLimit || FACTORY->GetPlayer()->GetComponent<Transform>()->position.x < RightXLimit)
						CAMERA->cameraPos.x = FACTORY->GetPlayer()->GetComponent<Transform>()->position.x;

					if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.y <= DownYLimit)			// when player is on the left.
						CAMERA->cameraPos.y = DownYLimit;
					else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.y >= UpYLimit)	// when player is on the right
						CAMERA->cameraPos.y = UpYLimit;
					else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.y > DownYLimit || FACTORY->GetPlayer()->GetComponent<Transform>()->position.y < UpYLimit)
						CAMERA->cameraPos.y = FACTORY->GetPlayer()->GetComponent<Transform>()->position.y;


					CAMERA->cameraPos.z = CameraPosz;
				}

				

			}
			else if (type == CameraRotation::EN_90)
			{

				// this is current block of map 
				upblock = FACTORY->RightBoundary()->GetComponent<Transform>()->position;
				downblock = FACTORY->LeftBoundary()->GetComponent<Transform>()->position;
				leftblock = FACTORY->UpBoundary()->GetComponent<Transform>()->position;
				rightblock = FACTORY->DownBoundary()->GetComponent<Transform>()->position;

				CAMERA->CenterOfCamera.x = downblock.x + ((upblock.x - downblock.x)*.5f);
				CAMERA->CenterOfCamera.y = leftblock.y + ((rightblock.y - leftblock.y)*.5f);

				CAMERA->cameraPos = glm::vec3(CAMERA->CenterOfCamera, CameraPosz);

				float UpYLimit, DownYLimit, LeftXLimit, RightXLimit;
				// 90 degrees rotated x becomes Y 
				// only this applies to 90 degrees, 
				UpYLimit = (upblock.x - std::abs((upblock.x - CAMERA->CenterOfCamera.x))*.5f);
				DownYLimit = (downblock.x + std::abs(CAMERA->CenterOfCamera.x - downblock.x)*.5f);


				if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x >= UpYLimit)
					CAMERA->cameraPos.x = UpYLimit;
				else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x <= DownYLimit)
					CAMERA->cameraPos.x = DownYLimit;
				else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x < UpYLimit || FACTORY->GetPlayer()->GetComponent<Transform>()->position.x > DownYLimit)
					CAMERA->cameraPos.x = FACTORY->GetPlayer()->GetComponent<Transform>()->position.x;
				CAMERA->cameraPos.z = CameraPosz;
			}
			else if (type == CameraRotation::EN_180)
			{
				// this is current block of map 
				upblock = FACTORY->DownBoundary()->GetComponent<Transform>()->position;
				downblock = FACTORY->UpBoundary()->GetComponent<Transform>()->position;
				rightblock = FACTORY->LeftBoundary()->GetComponent<Transform>()->position;
				leftblock = FACTORY->RightBoundary()->GetComponent<Transform>()->position;

				CAMERA->CenterOfCamera.x = leftblock.x - (std::abs(leftblock.x - rightblock.x)*.5f);
				CAMERA->CenterOfCamera.y = downblock.y - (std::abs(upblock.y - downblock.y)*.5f);

				CAMERA->cameraPos = glm::vec3(CAMERA->CenterOfCamera, CameraPosz);

				// To do: seprate sections of map 
				float UpYLimit, DownYLimit, LeftXLimit, RightXLimit;

				UpYLimit = upblock.y - std::abs(upblock.y - CAMERA->CenterOfCamera.y)*.5f;
				DownYLimit = downblock.y + std::abs(CAMERA->CenterOfCamera.y - downblock.y)*.5f;
				LeftXLimit = leftblock.x + std::abs(CAMERA->CenterOfCamera.x - leftblock.x)*.5f;
				RightXLimit = rightblock.x - std::abs(rightblock.x - CAMERA->CenterOfCamera.x)*.5f;


				if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x <= RightXLimit)			// when player is on the left.
					CAMERA->cameraPos.x = RightXLimit;
				else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x >= LeftXLimit)	// when player is on the right
					CAMERA->cameraPos.x = LeftXLimit;
				else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x < LeftXLimit || FACTORY->GetPlayer()->GetComponent<Transform>()->position.x > RightXLimit)
					CAMERA->cameraPos.x = FACTORY->GetPlayer()->GetComponent<Transform>()->position.x;

				if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.y <= UpYLimit)			// when player is on the left.
					CAMERA->cameraPos.y = UpYLimit;
				else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.y >= DownYLimit)	// when player is on the right
					CAMERA->cameraPos.y = DownYLimit;
				else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.y < DownYLimit || FACTORY->GetPlayer()->GetComponent<Transform>()->position.y > UpYLimit)
					CAMERA->cameraPos.y = FACTORY->GetPlayer()->GetComponent<Transform>()->position.y;

				CAMERA->cameraPos.z = CameraPosz;

			}
			else if (type == CameraRotation::EN_270)
			{
				// this is current block of map 
				upblock = FACTORY->LeftBoundary()->GetComponent<Transform>()->position;
				downblock = FACTORY->RightBoundary()->GetComponent<Transform>()->position;
				leftblock = FACTORY->DownBoundary()->GetComponent<Transform>()->position;
				rightblock = FACTORY->RightBoundary()->GetComponent<Transform>()->position;

				CAMERA->CenterOfCamera.x = downblock.x + ((upblock.x - downblock.x)*.5f);
				CAMERA->CenterOfCamera.y = rightblock.y + ((leftblock.y - rightblock.y)*.5f);

				float UpYLimit, DownYLimit;
				// 90 degrees rotated x becomes Y 
				// only this applies to 90 degrees, 
				UpYLimit = (upblock.x - std::abs((upblock.x - CAMERA->CenterOfCamera.x))*.5f);
				DownYLimit = (downblock.x + std::abs(CAMERA->CenterOfCamera.x - downblock.x)*.5f);

				if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x <= UpYLimit)
					CAMERA->cameraPos.x = UpYLimit;
				else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x >= DownYLimit)
					CAMERA->cameraPos.x = DownYLimit;
				else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x > UpYLimit || FACTORY->GetPlayer()->GetComponent<Transform>()->position.x < DownYLimit)
					CAMERA->cameraPos.x = FACTORY->GetPlayer()->GetComponent<Transform>()->position.x;
				CAMERA->cameraPos.z = CameraPosz;
			}
		}
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
		if (STATEMANAGER->i_LevelSelect >=1)
		{
			int degrees = RotatingCam();
			ControlCamMovement(degrees);
			/*std::cout << "CAMERA->cameraCENTER.x : " << CAMERA->CenterOfCamera.x << std::endl;
			std::cout << "CAMERA->cameraCENTER.y : " << CAMERA->CenterOfCamera.y << std::endl;
			std::cout << "CAMERA->cameraPos.x: " << CAMERA->cameraPos.x << std::endl;
			std::cout << "CAMERA->cameraPos.y: " << CAMERA->cameraPos.y << std::endl;*/
		}
		CAMERA->lookat(CAMERA->cameraPos, CAMERA->cameraTarget, CAMERA->cameraUp);
	}
}