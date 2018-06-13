/******************************************************************************/
/*!
\file        CameraMovement.cpp
\author      HyunJun Yoo
\par         email: hyunjun306@gmail.com
\par         course: GAM250
\par		 ThumbUp Engine
\date        06/13/2018

This is implementation for CameraMovement
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "CameraMovement.h"
#include "Factory.h"
#include "Physics.h"
#include "StateManager.h"
#include <random>

float CameraPosz = 999.f;
namespace TE {
	CameraAction::CameraAction()
	{
		shakeDuration = 3.0f;
		decreaseFactor = 1.0f;
		shakeAmount = 3.0f;
		_camPaceSpeed = 10.f;
		iscam_interpol = false;
		IsCamMovHoz = false;
		IsCamMovVer = false;
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

	bool CameraAction::cam_move_interpolate(glm::vec2 destination, float dt)
	{
		glm::vec2 displacement = destination- glm::vec2(CAMERA->cameraPos);
		displacement  = glm::normalize(displacement);
		glm::vec3 scale(3, 3, 0);
		glm::vec2 _camPacedirction = -displacement;
		glm::vec3 collpos(destination, 0);
		if (PHYSICS->SimpleRectvsRectCollisionCheck(&CAMERA->cameraPos, &scale,
			&collpos,
			&scale))
		{
			CAMERA->cameraPos.x = destination.x;
			CAMERA->cameraPos.y = destination.y;
			CAMERA->cameraPos.z = CameraPosz;
			return true;
			//isCamToPlayer = false;
		}
		if (CAMERA->cameraPos.x == destination.x || CAMERA->cameraPos.y == destination.y)
			return true;
		if(STATEMANAGER->Loadtolevelname == "level6.json")
			CAMERA->cameraPos.x -= _camPacedirction.x * 300.f * dt;
		else {
			CAMERA->cameraPos.x -= _camPacedirction.x * 300.f * dt;
			CAMERA->cameraPos.y -= _camPacedirction.y * 300.f * dt;
		}
		return false;
	}

	int CameraAction::RotatingCam()
	{
		// when degree is 0 
		if (CAMERA->cameraUp.x == 0 && CAMERA->cameraUp.y == 1)
			return EN_0;
		// when degree is 90
		else if (CAMERA->cameraUp.x == 1 && CAMERA->cameraUp.y == 0)
			return EN_90;
		// when degree is 180
		else if (CAMERA->cameraUp.x == 0 && CAMERA->cameraUp.y == -1)
			return EN_180;
		// when degree is 270
		else if (CAMERA->cameraUp.x == -1 && CAMERA->cameraUp.y == 0)
			return EN_270;
		return false;
	}

	void CameraAction::ControlCamMovement(int type, float dt)
	{
		glm::vec3 rightblock = glm::vec3(0), leftblock = glm::vec3(0), upblock = glm::vec3(0), downblock = glm::vec3(0);
		if (FACTORY->GetPlayer())
		{
			glm::vec2 playerpos = glm::vec2(FACTORY->GetPlayer()->GetComponent<Transform>()->position);
			/*CAMERA->cameraPos.x = playerpos.x;
			CAMERA->cameraPos.y = playerpos.y;*/
			float leftboundary = 0, rightboundary = 0, upboundary = 0, downboundary = 0;
			glm::vec2 center(0);
			float width = 0, height = 0;
			// this is for when EN ==0 
			if (type == CameraRotation::EN_0)
			{		
				upblock = FACTORY->UpBoundary()->GetComponent<Transform>()->position;
				downblock = FACTORY->DownBoundary()->GetComponent<Transform>()->position;
				rightblock = FACTORY->RightBoundary()->GetComponent<Transform>()->position;
				leftblock = FACTORY->LeftBoundary()->GetComponent<Transform>()->position;
				width = abs(rightblock.x - leftblock.x), height = abs(upblock.y - downblock.y);
				center.x = leftblock.x + width * .5f;
				center.y = downblock.y + height * .5f;
			}
			else if(type == CameraRotation::EN_90)
			{
				upblock = FACTORY->RightBoundary()->GetComponent<Transform>()->position;
				downblock = FACTORY->LeftBoundary()->GetComponent<Transform>()->position;
				leftblock = FACTORY->UpBoundary()->GetComponent<Transform>()->position;
				rightblock = FACTORY->DownBoundary()->GetComponent<Transform>()->position;
				width = abs(rightblock.y - leftblock.y), height = abs(upblock.x - downblock.x);
				center.x = leftblock.y + width * .5f;
				center.y = downblock.x + height * .5f;
			}
			else if(type == CameraRotation::EN_180)
			{
				upblock = FACTORY->DownBoundary()->GetComponent<Transform>()->position;
				downblock = FACTORY->UpBoundary()->GetComponent<Transform>()->position;
				leftblock = FACTORY->RightBoundary()->GetComponent<Transform>()->position;
				rightblock = FACTORY->LeftBoundary()->GetComponent<Transform>()->position;

				width = std::abs(rightblock.x - leftblock.x), height = std::abs(upblock.y - downblock.y);
				center.x = leftblock.x + width * .5f;
				center.y = downblock.y + height * .5f;
				
			}
			else if(type == CameraRotation::EN_270)
			{
				upblock = FACTORY->LeftBoundary()->GetComponent<Transform>()->position;
				downblock = FACTORY->RightBoundary()->GetComponent<Transform>()->position;
				leftblock = FACTORY->DownBoundary()->GetComponent<Transform>()->position;
				rightblock = FACTORY->UpBoundary()->GetComponent<Transform>()->position;

				width = std::abs(rightblock.y - leftblock.y), height = std::abs(upblock.x - downblock.x);
				center.y = leftblock.x + width * .5f;
				center.x = downblock.y + height * .5f;
			}

				
			if (STATEMANAGER->Loadtolevelname == "level1.json")
			{
				width = FACTORY->DownBoundary()->GetComponent<Transform>()->scale.x;
				CAMERA->cameraPos.x = downblock.x;
				CAMERA->cameraPos.y = 0;
				leftboundary = downblock.x - width * .1f;
				rightboundary = downblock.x + width * .1f;

				if (playerpos.x < leftboundary)
					CAMERA->cameraPos.x = leftboundary;
				else if (playerpos.x > rightboundary)
					CAMERA->cameraPos.x = rightboundary;
				else
					CAMERA->cameraPos.x = playerpos.x;
			}
			else if (STATEMANAGER->Loadtolevelname == "level2.json" || STATEMANAGER->Loadtolevelname == "level3.json" ||
				STATEMANAGER->Loadtolevelname == "level5.json" ||
				STATEMANAGER->Loadtolevelname == "level8.json" || STATEMANAGER->Loadtolevelname == "level10.json"  )
			{
				CAMERA->cameraPos.x = 0;
				CAMERA->cameraPos.x = 0;
			}
			else if (STATEMANAGER->Loadtolevelname == "level4.json" || STATEMANAGER->Loadtolevelname == "level12.json")
			{
				if (type == CameraRotation::EN_0)
				{
					CAMERA->cameraPos.x = center.x;
					CAMERA->cameraPos.y = center.y;
				}
			}
			else if (STATEMANAGER->Loadtolevelname == "level6.json")
			{
				if (type == CameraRotation::EN_0)
				{
					center.x = leftblock.x + width * .5f;
					center.y = downblock.y + height * .5f;
					CAMERA->cameraPos.x = center.x;
					CAMERA->cameraPos.y = center.y;
				}
				else if (type == CameraRotation::EN_90)
				{
					upboundary = upblock.x - height * .2f;
					downboundary = downblock.x + height * .2f;
					center.x = leftblock.y + width * .5f;
					center.y = downblock.x + height * .5f;

					if (CAMERA->cameraPos.x != downboundary)
						iscam_interpol = false;
						 
					if(!iscam_interpol)
					iscam_interpol = cam_move_interpolate( glm::vec2(downboundary, center.y ), dt);
					else {
						if (playerpos.x < downboundary)
							CAMERA->cameraPos.x = downboundary;
						else if (playerpos.x > upboundary)
							CAMERA->cameraPos.x = upboundary;
						else CAMERA->cameraPos.x = playerpos.x;
						iscam_interpol = false;

					}
				}
				else if (type == CameraRotation::EN_270)
				{
					center.x = leftblock.y + width * .5f;
					center.y = downblock.x + height * .5f;
					upboundary = upblock.x + height * .2f;
					downboundary = downblock.x - height * .2f;
					if (!iscam_interpol)
						iscam_interpol = cam_move_interpolate(glm::vec2(downboundary, center.y), dt);
					else
					{
						if (playerpos.x > downboundary)
							CAMERA->cameraPos.x = downboundary;
					
						else CAMERA->cameraPos.x = playerpos.x;
						iscam_interpol = false;

					}
				}
			}

			else if(STATEMANAGER->Loadtolevelname == "level20.json" || STATEMANAGER->Loadtolevelname == "level16.json")
			{
				CAMERA->cameraPos.x = playerpos.x;
				CAMERA->cameraPos.y = playerpos.y;
			}
			else 
			{
				leftboundary = downblock.x - width * .1f;
				rightboundary = downblock.x + width * .1f;
				
				if (type == CameraRotation::EN_0)
				{
					if (playerpos.x < leftboundary)
						CAMERA->cameraPos.x = leftboundary;
					else if (playerpos.x > rightboundary)
					{
						CAMERA->cameraPos.x = rightboundary;
					}
					else
					{
						CAMERA->cameraPos.x = playerpos.x;
					}
				}
				else if (type == CameraRotation::EN_90)
				{
					upboundary = upblock.x - height * .35f;
					downboundary = downblock.x + height * .35f;
					/*else if (!iscam_interpol)
						iscam_interpol = cam_move_interpolate(glm::vec2(upboundary,center.y), dt);*/
					//float difference = abs(upboundary - upblock.x *.5f);
					if (playerpos.x < downboundary)
						CAMERA->cameraPos.x = downboundary;
					else if (playerpos.x < upboundary)
						CAMERA->cameraPos.x = playerpos.x;// +difference;
					iscam_interpol = true;
					
				}
				else if (type == CameraRotation::EN_180)
				{
					upboundary = upblock.x - height * .2f;
					downboundary = downblock.x + height * .2f;
					
						if (playerpos.x < leftboundary)
							CAMERA->cameraPos.x = leftboundary;
						else if (playerpos.x > rightboundary)
							CAMERA->cameraPos.x = rightboundary;
						else
							CAMERA->cameraPos.x = playerpos.x;
					
				}
				else if (type == CameraRotation::EN_270)
				{
					upboundary = upblock.x - height * .2f;

					downboundary = downblock.x - height * 0.3f;
					/*if (!iscam_interpol)
						iscam_interpol = cam_move_interpolate(glm::vec2(downboundary, center.y), dt);
					else
					{*/
						if (playerpos.x < upboundary)
							CAMERA->cameraPos.x = upboundary;
						else if (playerpos.x < downboundary)
							CAMERA->cameraPos.x = playerpos.x;
						iscam_interpol = true;
					//}
				}
			}
		}
	}





	void CameraAction::cameraSetting(CameraPosType camPosType)
	{
		switch (camPosType)
		{
		case EN_Default:
			CAMERA->cameraPos = glm::vec3(0, 0, CameraPosz);
			break;
		case EN_Splash:
			CAMERA->cameraPos = glm::vec3(0, 0, CameraPosz);
			break;
		case EN_Menu:
			CameraPosz = 999.f;
			CAMERA->cameraPos = glm::vec3(0, 0, CameraPosz);
			break;
		case EN_LevelSelect:
			CameraPosz = 999.f;
			CAMERA->cameraPos = glm::vec3(0, 0, CameraPosz);
			break;
		case EN_playerPos:
			CameraPosz = 999.f;
			glm::vec2(CAMERA->cameraPos) = glm::vec2(FACTORY->GetPlayer()->GetComponent<Transform>()->position);
			break;
		case EN_BOUNDARY:
			CameraPosz = 999.f;
			CAMERA->cameraPos = glm::vec3(0, 0, CameraPosz);
			break;
		case EN_Option:
			CameraPosz = 999.f;
			CAMERA->cameraPos = glm::vec3(0, 0, CameraPosz);
			break;
		case End:
			break;
		}
	}
	bool CameraAction::DisplayTheWholeMap() {
		if (!FACTORY->GetPlayer())
			return false;
		CAMERA->CenterOfCamera.x = FACTORY->LeftBoundary()->GetComponent<Transform>()->position.x + (FACTORY->RightBoundary()->GetComponent<Transform>()->position.x - FACTORY->LeftBoundary()->GetComponent<Transform>()->position.x) *.5f;
		CAMERA->CenterOfCamera.y = FACTORY->DownBoundary()->GetComponent<Transform>()->position.y + (FACTORY->UpBoundary()->GetComponent<Transform>()->position.y - FACTORY->DownBoundary()->GetComponent<Transform>()->position.y)*.5f;
		glm::vec2(CAMERA->cameraPos) = glm::vec2(CAMERA->CenterOfCamera);
		return true;
	}

	void CameraAction::moveCameraPos(glm::vec2 cameraPOS)
	{
		CAMERA->cameraPos += glm::vec3(cameraPOS, 0);
	}

	void CameraAction::Update(float dt)
	{
		if (STATEMANAGER->i_LevelSelect >= 1)
		{
			int degrees = RotatingCam();
			ControlCamMovement(degrees, dt);
			//move_cam(dt);

		}

	}
}