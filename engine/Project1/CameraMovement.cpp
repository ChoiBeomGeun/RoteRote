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

	void CameraAction::FollowPlayer(glm::vec3 * cameraPos, glm::vec3 * cameraScale, float dt)
	{
		glm::vec2 displacement = glm::vec2(FACTORY->GetPlayer()->GetComponent<Transform>()->position) - glm::vec2(CAMERA->cameraPos);
		cameraOriginPos = glm::vec3();
		//   auto distanceFromStart = glm::length(displacement);
		glm::vec2 _camPacedirction = -displacement;
		//_camPacedirction = glm::normalize(_camPacedirction);
		glm::vec2(CAMERA->cameraPos) -= _camPacedirction * _camPaceSpeed * dt;


		*cameraPos = CAMERA->cameraPos;

		if (PHYSICS->SimpleRectvsRectCollisionCheck(cameraPos, cameraScale,
			&FACTORY->GetPlayer()->GetComponent<Transform>()->position,
			&FACTORY->GetPlayer()->GetComponent<Transform>()->scale))
		{
			CAMERA->cameraPos = FACTORY->GetPlayer()->GetComponent<Transform>()->position;
			CAMERA->cameraPos.z = CameraPosz;
			CAMERA->isCentered = false;
			isCamToPlayer = false;
		}
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
		return false;
	}

	void CameraAction::ControlCamMovement(int type)
	{
		glm::vec3 rightblock = glm::vec3(0), leftblock = glm::vec3(0), upblock = glm::vec3(0), downblock = glm::vec3(0);
		if (FACTORY->GetPlayer())
		{
			glm::vec3 test = CAMERA->viewport_geometry();
			if (type == CameraRotation::EN_0)
			{
				if (IsCamMovVer)
					IsCamMovVer = !IsCamMovVer;

				// this is current block of map 
				upblock = FACTORY->UpBoundary()->GetComponent<Transform>()->position;
				downblock = FACTORY->DownBoundary()->GetComponent<Transform>()->position;
				rightblock = FACTORY->RightBoundary()->GetComponent<Transform>()->position;
				leftblock = FACTORY->LeftBoundary()->GetComponent<Transform>()->position;
				if (((upblock.y - downblock.y) < APP->_screenHeight) && ((rightblock.x - leftblock.x) < APP->_screenWidth)) // when wall boundaries are smaller than camera size
				{
					if (STATEMANAGER->Loadtolevelname == "level1.json")
					{
						CAMERA->CenterOfCamera.x = downblock.x;
						CAMERA->CenterOfCamera.y = downblock.y + APP->_screenHeight*.5f - FACTORY->GetPlayer()->GetComponent<Transform>()->scale.y * 4;


						CAMERA->cameraPos.x = CAMERA->CenterOfCamera.x;
						CAMERA->cameraPos.y = CAMERA->CenterOfCamera.y;

						float LeftXLimit, RightXLimit;
						LeftXLimit = downblock.x - APP->_screenWidth *.25f;
						RightXLimit = downblock.x + APP->_screenWidth *.25f;

						if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x <= LeftXLimit)
						{
							CAMERA->cameraPos.x = LeftXLimit;
						}
						else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x >= RightXLimit)
						{
							CAMERA->cameraPos.x = RightXLimit;
						}
						else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x > LeftXLimit || FACTORY->GetPlayer()->GetComponent<Transform>()->position.x < RightXLimit)
						{
							CAMERA->cameraPos.x = FACTORY->GetPlayer()->GetComponent<Transform>()->position.x;
						}
					}
					else {

						CAMERA->CenterOfCamera.x = leftblock.x + (std::abs(leftblock.x - rightblock.x)*.5f);
						CAMERA->CenterOfCamera.y = downblock.y + (std::abs(upblock.y - downblock.y)*.5f);

						CAMERA->cameraPos.x = CAMERA->CenterOfCamera.x;
						CAMERA->cameraPos.y = CAMERA->CenterOfCamera.y;
					}
				}
				else
				{
					CAMERA->CenterOfCamera.x = leftblock.x + (std::abs(leftblock.x - rightblock.x)*.5f);
					CAMERA->CenterOfCamera.y = downblock.y + (std::abs(upblock.y - downblock.y)*.5f);

					// To do: seprate sections of map 
					float UpYLimit, DownYLimit, LeftXLimit, RightXLimit;

					UpYLimit = upblock.y - std::abs(upblock.y - CAMERA->CenterOfCamera.y)*.5f;
					DownYLimit = downblock.y + std::abs(CAMERA->CenterOfCamera.y - downblock.y)*.5f;
					LeftXLimit = leftblock.x + std::abs(CAMERA->CenterOfCamera.x - leftblock.x)*.5f;
					RightXLimit = rightblock.x - std::abs(rightblock.x - CAMERA->CenterOfCamera.x)*.5f;


					if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x <= LeftXLimit)         // when player is on the left.
						CAMERA->cameraPos.x = LeftXLimit;
					else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x >= RightXLimit)   // when player is on the right
						CAMERA->cameraPos.x = RightXLimit;
					else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x > LeftXLimit || FACTORY->GetPlayer()->GetComponent<Transform>()->position.x < RightXLimit)
						CAMERA->cameraPos.x = FACTORY->GetPlayer()->GetComponent<Transform>()->position.x;

					if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.y <= DownYLimit)         // when player is on the left.
						CAMERA->cameraPos.y = DownYLimit;
					else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.y >= UpYLimit)   // when player is on the right
						CAMERA->cameraPos.y = UpYLimit;
					else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.y > DownYLimit || FACTORY->GetPlayer()->GetComponent<Transform>()->position.y < UpYLimit)
						CAMERA->cameraPos.y = FACTORY->GetPlayer()->GetComponent<Transform>()->position.y;

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
				if ((upblock.y - downblock.y) < APP->_screenHeight && (rightblock.x - leftblock.x) < APP->_screenWidth) // when wall boundaries are smaller than camera size
				{
					float UpYLimit, DownYLimit;
					UpYLimit = (upblock.x - std::abs((upblock.x - CAMERA->CenterOfCamera.x))*.5f);
					DownYLimit = (downblock.x + std::abs(CAMERA->CenterOfCamera.x - downblock.x)*.5f);

					if (abs(upblock.x - downblock.x)> APP->_screenHeight) //when vertical different is bigger than screen _height size.
					{

						if (!IsCamMovVer)
						{
							if (STATEMANAGER->Loadtolevelname == "level6.json")
							{
								if (CAMERA->cameraPos.x > DownYLimit)
									CAMERA->cameraPos.x -= 5.f;
								else if (CAMERA->cameraPos.x <= DownYLimit)
									IsCamMovVer = !IsCamMovVer;
							}
							else
							{
								if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x >= UpYLimit)
									CAMERA->cameraPos.x = UpYLimit;
								else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x <= DownYLimit)
									CAMERA->cameraPos.x = DownYLimit;
								else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x < UpYLimit || FACTORY->GetPlayer()->GetComponent<Transform>()->position.x > DownYLimit)
									CAMERA->cameraPos.x = FACTORY->GetPlayer()->GetComponent<Transform>()->position.x;
							}
						}

						else {
							if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x >= UpYLimit)
								CAMERA->cameraPos.x = UpYLimit;
							else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x <= DownYLimit)
								CAMERA->cameraPos.x = DownYLimit;
							else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x < UpYLimit || FACTORY->GetPlayer()->GetComponent<Transform>()->position.x > DownYLimit)
								CAMERA->cameraPos.x = FACTORY->GetPlayer()->GetComponent<Transform>()->position.x;
						}
					}
					else // have fix camera pos at the center.
					{
						CAMERA->cameraPos.x = CAMERA->CenterOfCamera.x;
						CAMERA->cameraPos.y = CAMERA->CenterOfCamera.y;
					}

				}
				else
				{
					CAMERA->CenterOfCamera.x = downblock.x + ((upblock.x - downblock.x)*.5f);
					CAMERA->CenterOfCamera.y = leftblock.y + ((rightblock.y - leftblock.y)*.5f);

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
				}
			}
			else if (type == CameraRotation::EN_180)
			{
				if (IsCamMovVer)
					IsCamMovVer = !IsCamMovVer;

				// this is current block of map 
				upblock = FACTORY->DownBoundary()->GetComponent<Transform>()->position;
				downblock = FACTORY->UpBoundary()->GetComponent<Transform>()->position;
				rightblock = FACTORY->LeftBoundary()->GetComponent<Transform>()->position;
				leftblock = FACTORY->RightBoundary()->GetComponent<Transform>()->position;

				if ((upblock.y - downblock.y) < APP->_screenHeight && (rightblock.x - leftblock.x) < APP->_screenWidth) // when wall boundaries are smaller than camera size
				{

				}
				else
				{
					CAMERA->CenterOfCamera.x = leftblock.x - (std::abs(leftblock.x - rightblock.x)*.5f);
					CAMERA->CenterOfCamera.y = downblock.y - (std::abs(upblock.y - downblock.y)*.5f);

					// To do: seprate sections of map 
					float UpYLimit, DownYLimit, LeftXLimit, RightXLimit;

					UpYLimit = upblock.y + std::abs(upblock.y - CAMERA->CenterOfCamera.y)*.5f;
					DownYLimit = downblock.y - std::abs(CAMERA->CenterOfCamera.y - downblock.y)*.5f;
					LeftXLimit = leftblock.x - std::abs(CAMERA->CenterOfCamera.x - leftblock.x)*.5f;
					RightXLimit = rightblock.x + std::abs(rightblock.x - CAMERA->CenterOfCamera.x)*.5f;

					if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x <= RightXLimit)         // when player is on the left.
						CAMERA->cameraPos.x = RightXLimit;
					else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x >= LeftXLimit)   // when player is on the right
						CAMERA->cameraPos.x = LeftXLimit;
					else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x < LeftXLimit || FACTORY->GetPlayer()->GetComponent<Transform>()->position.x > RightXLimit)
						CAMERA->cameraPos.x = FACTORY->GetPlayer()->GetComponent<Transform>()->position.x;

					if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.y <= UpYLimit)         // when player is on the left.
						CAMERA->cameraPos.y = UpYLimit;
					else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.y >= DownYLimit)   // when player is on the right
						CAMERA->cameraPos.y = DownYLimit;
					else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.y < DownYLimit || FACTORY->GetPlayer()->GetComponent<Transform>()->position.y > UpYLimit)
						CAMERA->cameraPos.y = FACTORY->GetPlayer()->GetComponent<Transform>()->position.y;
				}

			}
			else if (type == CameraRotation::EN_270)
			{
				// this is current block of map 
				upblock = FACTORY->LeftBoundary()->GetComponent<Transform>()->position;
				downblock = FACTORY->RightBoundary()->GetComponent<Transform>()->position;
				leftblock = FACTORY->DownBoundary()->GetComponent<Transform>()->position;
				rightblock = FACTORY->RightBoundary()->GetComponent<Transform>()->position;
				float UpYLimit, DownYLimit;
				// 270 degrees rotated x becomes Y 
				// only this applies to 270 degrees, 
				CAMERA->CenterOfCamera.x = downblock.x + ((upblock.x - downblock.x)*.5f);
				CAMERA->CenterOfCamera.y = rightblock.x + ((leftblock.x - rightblock.x)*.5f);
				UpYLimit = upblock.x + std::abs(upblock.x - CAMERA->CenterOfCamera.x)*.5f;
				DownYLimit = downblock.x - std::abs(CAMERA->CenterOfCamera.x - downblock.x)*.5f;


				if ((upblock.y - downblock.y) < APP->_screenHeight && (rightblock.x - leftblock.x) < APP->_screenWidth) // when wall boundaries are smaller than camera size
				{
					if (abs(upblock.x - downblock.x) > APP->_screenHeight)
					{
						if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x <= UpYLimit)
							CAMERA->cameraPos.x = UpYLimit;
						else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x >= DownYLimit)
							CAMERA->cameraPos.x = DownYLimit;
						else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x > UpYLimit || FACTORY->GetPlayer()->GetComponent<Transform>()->position.x < DownYLimit)
							CAMERA->cameraPos.x = FACTORY->GetPlayer()->GetComponent<Transform>()->position.x;
					}

				}
				else
				{
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
				}
			}
		}
	}

	void CameraAction::move_cam(float dt)
	{
		/*if (IsCamMovHoz)
		{
		CAMERA->cameraPos.x = interpolate((CAMERA->cameraPos.x), FACTORY->GetPlayer()->GetComponent<Transform>()->position.x, .5f);
		IsCamMovHoz = false;
		}
		else if (IsCamMovVer)
		CAMERA->cameraPos.y = FACTORY->GetPlayer()->GetComponent<Transform>()->position.y;
		else if(IsCamMovHoz && IsCamMovVer)
		{
		glm::vec2(CAMERA->cameraPos) = glm::vec2(FACTORY->GetPlayer()->GetComponent<Transform>()->position);
		}*/
	}

	//float CameraAction::interpolate(float  start, float dest, float dt)
	//{
	//   /*start = abs(start);
	//   dest = abs(dest);*/
	//   /*while (1)
	//   {*/
	//   
	//   //float displacement = FACTORY->GetPlayer()->GetComponent<Transform>()->position.x - CAMERA->cameraPos.x;
	//   //cameraOriginPos = glm::vec3();
	//   ////   auto distanceFromStart = glm::length(displacement);
	//   //float _camPacedirction = -glm::normalize(displacement);
	//   ////_camPacedirction = glm::normalize(_camPacedirction);
	//   //CAMERA->cameraPos.x -= _camPacedirction * _camPaceSpeed * dt;
	//   //CAMERA->cameraPos.z = 999.f;


	//   return  start + dt*(dest-start);
	//      /*if (start >= dest)
	//         break;
	//   }*/
	//}

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
			CameraPosz = 500.f;
			CAMERA->cameraPos = glm::vec3(0, 0, CameraPosz);
			break;
		case EN_LevelSelect:
			CameraPosz = 500.f;
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
			CameraPosz = 675.f;
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
			ControlCamMovement(degrees);
			//move_cam(dt);

		}

	}
}