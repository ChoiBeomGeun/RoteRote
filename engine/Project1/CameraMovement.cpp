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
		//glm::vec3 cameraOriginPos = CAMERA->cameraPos;
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

			//std::cout << "dis(gen): " << dis(gen) << '\n';
			//std::cout << "camerapos.x : " << glm::vec3(dis(gen) * shakeAmount, dis(gen) * shakeAmount, 1).x <<'\n';
			//std::cout << "camerapos.y : " << glm::vec3(dis(gen) * shakeAmount, dis(gen) * shakeAmount, 1).y << '\n';
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

}