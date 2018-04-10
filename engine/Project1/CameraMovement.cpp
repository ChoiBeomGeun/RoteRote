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

	void CameraAction::cameraSetting(glm::vec3 cameraPOS)
	{
		CAMERA->cameraPos = cameraPOS;		
		CAMERA->lookat(CAMERA->cameraPos, CAMERA->cameraTarget, CAMERA->cameraUp);
		std::cout << "cameraPos.x " << CAMERA->cameraPos.x << " .y " << CAMERA->cameraPos.y << ". z " << CAMERA->cameraPos.z << std::endl;
		std::cout << "cameraTarget.x " << CAMERA->cameraTarget.x << " .y " << CAMERA->cameraTarget.y << "/z " << CAMERA->cameraTarget.z << std::endl;
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