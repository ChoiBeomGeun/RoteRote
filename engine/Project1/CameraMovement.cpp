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

	void CameraAction::ControlCamMovement(int type, float dt)
	{
		
		glm::vec3 rightblock = glm::vec3(0), leftblock = glm::vec3(0), upblock = glm::vec3(0), downblock = glm::vec3(0);
		if (FACTORY->GetPlayer())
		{
			glm::vec3 playerpos = FACTORY->GetPlayer()->GetComponent<Transform>()->position;
			if (type == CameraRotation::EN_0)
			{
				upblock = FACTORY->UpBoundary()->GetComponent<Transform>()->position;
				downblock = FACTORY->DownBoundary()->GetComponent<Transform>()->position;
				rightblock = FACTORY->RightBoundary()->GetComponent<Transform>()->position;
				leftblock = FACTORY->LeftBoundary()->GetComponent<Transform>()->position;
			

				float width = rightblock.x - leftblock.x, height = upblock.x - downblock.x;
				if (STATEMANAGER->Loadtolevelname == "level1.json")
				{
					width = FACTORY->DownBoundary()->GetComponent<Transform>()->scale.x;
					CAMERA->cameraPos.x = downblock.x;
					CAMERA->cameraPos.y =0;

					float leftboundary, rightboundary;
					leftboundary = downblock.x - APP->_screenWidth*.25f;
					rightboundary = downblock.x + APP->_screenWidth *.25f;

					if (playerpos.x < leftboundary)
						CAMERA->cameraPos.x = leftboundary;
					else if (playerpos.x > rightboundary)
						CAMERA->cameraPos.x = rightboundary;
					else
						CAMERA->cameraPos.x = playerpos.x;
				}
				else if (STATEMANAGER->Loadtolevelname == "level8.json" || STATEMANAGER->Loadtolevelname == "level10.json")
				{
					CAMERA->cameraPos = glm::vec3(0, 0, 999);
				}
				// 1. if map is smaller than screen
				else if (width < APP->_screenWidth && height < APP->_screenHeight)
				{
					CAMERA->cameraPos.x = downblock.x;
					CAMERA->cameraPos.y = leftblock.y;
				}
				// 2. if width is bigger than screen
				else if (width > APP->_screenWidth)
				{
					float leftboundary, rightboundary;
					leftboundary = downblock.x - APP->_screenWidth*.25f;
					rightboundary = downblock.x + APP->_screenWidth *.25f;

					if (playerpos.x < leftboundary)
						CAMERA->cameraPos.x = leftboundary;
					else if (playerpos.x > rightboundary)
						CAMERA->cameraPos.x = rightboundary;
					else
						CAMERA->cameraPos.x = playerpos.x;
				}
				// 3. if height is bigger than screen
				else if (height > APP->_screenHeight)
				{
					float upboundary, downboundary;
					upboundary = upblock.x - APP->_screenHeight*.25f;
					downboundary = downblock.x + APP->_screenHeight *.25f;

					if (playerpos.y < upboundary)
						CAMERA->cameraPos.y = upboundary;
					else if (playerpos.y > downboundary)
						CAMERA->cameraPos.y = downboundary;
					else
						CAMERA->cameraPos.y = playerpos.y;
				}
				// 4 when map is both bigger than screen.
				else {

				}
			}
			else if (type == CameraRotation::EN_90)
			{
				upblock = FACTORY->RightBoundary()->GetComponent<Transform>()->position;
				downblock = FACTORY->LeftBoundary()->GetComponent<Transform>()->position;
				leftblock = FACTORY->UpBoundary()->GetComponent<Transform>()->position;
				rightblock = FACTORY->DownBoundary()->GetComponent<Transform>()->position;
				float width = std::abs(rightblock.y - leftblock.y), height = std::abs(upblock.x - downblock.x);
				glm::vec2 center = glm::vec2(width *.5f,height *.5f );
				// 1. if map is smaller than screen
				if (width < APP->_screenWidth && height < APP->_screenHeight)
				{
					// do nothing
				}
				// 2. if width is bigger than screen
				else if (width > APP->_screenWidth)
				{
					
				}
				// 3. if height is bigger than screen
				else if (height > APP->_screenHeight)
				{
					float downlimit = downblock.x + APP->_screenHeight*.25f, uplimit = upblock.x - APP->_screenHeight *.25f;
					if (CAMERA->cameraPos.x < center.y)
					{
						if (CAMERA->cameraPos.x > downlimit)
							CAMERA->cameraPos.x -= 500 * dt;
						if (CAMERA->cameraPos.x <= downlimit)
							CAMERA->cameraPos.x = downlimit;
					}
					else if (CAMERA->cameraPos.x > center.y)
					{
						if (CAMERA->cameraPos.x < uplimit)
							CAMERA->cameraPos.x += 5 * dt;
						if (CAMERA->cameraPos.x >= uplimit)
							CAMERA->cameraPos.x = downlimit;
					}

				}
				// 4 when map is both bigger than screen.
			}
			else if (type == CameraRotation::EN_180)
			{

			}
			else if (type == CameraRotation::EN_270)
			{
				upblock = FACTORY->LeftBoundary()->GetComponent<Transform>()->position;
				downblock = FACTORY->RightBoundary()->GetComponent<Transform>()->position;
				leftblock = FACTORY->DownBoundary()->GetComponent<Transform>()->position;
				rightblock = FACTORY->UpBoundary()->GetComponent<Transform>()->position;
				float width = std::abs(rightblock.y - leftblock.y), height = std::abs(upblock.x - downblock.x);
				glm::vec2 center = glm::vec2(width *.5f, height *.5f);
				// 1. if map is smaller than screen
				if (width < APP->_screenWidth && height < APP->_screenHeight)
				{
					CAMERA->cameraPos = glm::vec3(center, 999);
				}
				// 2. if width is bigger than screen
				else if (width > APP->_screenWidth)
				{

				}
				// 3. if height is bigger than screen
				else if (height > APP->_screenHeight)
				{
					float downlimit = downblock.x - APP->_screenHeight*.25f, uplimit = upblock.x + APP->_screenHeight *.25f;
					if (playerpos.x<= center.x)
					{
						if (playerpos.x < uplimit)
							CAMERA->cameraPos.x = uplimit;
						else if (playerpos.x >= uplimit)
							CAMERA->cameraPos.x = playerpos.x;
						
					}
					else if (playerpos.x > center.x)
					{
						if (CAMERA->cameraPos.x >= downlimit)
							CAMERA->cameraPos.x = downlimit;
						else if (CAMERA->cameraPos.x < downlimit)
							CAMERA->cameraPos.x = playerpos.x;
						
					}
				}
				// 4 when map is both bigger than screen.
			}


			//	glm::vec3 test = CAMERA->viewport_geometry();
			//	if (type == CameraRotation::EN_0)
			//	{
			//		if (IsCamMovVer)
			//			IsCamMovVer = !IsCamMovVer;

			//		// this is current block of map 
			//		upblock = FACTORY->UpBoundary()->GetComponent<Transform>()->position;
			//		downblock = FACTORY->DownBoundary()->GetComponent<Transform>()->position;
			//		rightblock = FACTORY->RightBoundary()->GetComponent<Transform>()->position;
			//		leftblock = FACTORY->LeftBoundary()->GetComponent<Transform>()->position;
			//		if (((upblock.y - downblock.y) < APP->_screenHeight) && ((rightblock.x - leftblock.x) < APP->_screenWidth)) // when wall boundaries are smaller than camera size
			//		{
			//			if (STATEMANAGER->Loadtolevelname == "level1.json")
			//			{
			//				CAMERA->CenterOfCamera.x = downblock.x;
			//				CAMERA->CenterOfCamera.y = downblock.y + APP->_screenHeight*.5f - FACTORY->GetPlayer()->GetComponent<Transform>()->scale.y * 4;


			//				CAMERA->cameraPos.x = CAMERA->CenterOfCamera.x;
			//				CAMERA->cameraPos.y = CAMERA->CenterOfCamera.y;

			//				float LeftXLimit, RightXLimit;
			//				LeftXLimit = downblock.x - APP->_screenWidth *.25f;
			//				RightXLimit = downblock.x + APP->_screenWidth *.25f;

			//				if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x <= LeftXLimit)
			//				{
			//					CAMERA->cameraPos.x = LeftXLimit;
			//				}
			//				else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x >= RightXLimit)
			//				{
			//					CAMERA->cameraPos.x = RightXLimit;
			//				}
			//				else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x > LeftXLimit || FACTORY->GetPlayer()->GetComponent<Transform>()->position.x < RightXLimit)
			//				{
			//					CAMERA->cameraPos.x = FACTORY->GetPlayer()->GetComponent<Transform>()->position.x;
			//				}
			//			}
			//			else {

			//				CAMERA->CenterOfCamera.x = leftblock.x + (std::abs(leftblock.x - rightblock.x)*.5f);
			//				CAMERA->CenterOfCamera.y = downblock.y + (std::abs(upblock.y - downblock.y)*.5f);

			//				CAMERA->cameraPos.x = CAMERA->CenterOfCamera.x;
			//				CAMERA->cameraPos.y = CAMERA->CenterOfCamera.y;
			//			}
			//		}
			//		else
			//		{
			//			CAMERA->CenterOfCamera.x = leftblock.x + (std::abs(leftblock.x - rightblock.x)*.5f);
			//			CAMERA->CenterOfCamera.y = downblock.y + (std::abs(upblock.y - downblock.y)*.5f);

			//			// To do: seprate sections of map 
			//			float UpYLimit, DownYLimit, LeftXLimit, RightXLimit;

			//			UpYLimit = upblock.y - std::abs(upblock.y - CAMERA->CenterOfCamera.y)*.5f;
			//			DownYLimit = downblock.y + std::abs(CAMERA->CenterOfCamera.y - downblock.y)*.5f;
			//			LeftXLimit = leftblock.x + std::abs(CAMERA->CenterOfCamera.x - leftblock.x)*.5f;
			//			RightXLimit = rightblock.x - std::abs(rightblock.x - CAMERA->CenterOfCamera.x)*.5f;


			//			if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x <= LeftXLimit)         // when player is on the left.
			//				CAMERA->cameraPos.x = LeftXLimit;
			//			else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x >= RightXLimit)   // when player is on the right
			//				CAMERA->cameraPos.x = RightXLimit;
			//			else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x > LeftXLimit || FACTORY->GetPlayer()->GetComponent<Transform>()->position.x < RightXLimit)
			//				CAMERA->cameraPos.x = FACTORY->GetPlayer()->GetComponent<Transform>()->position.x;

			//			if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.y <= DownYLimit)         // when player is on the left.
			//				CAMERA->cameraPos.y = DownYLimit;
			//			else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.y >= UpYLimit)   // when player is on the right
			//				CAMERA->cameraPos.y = UpYLimit;
			//			else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.y > DownYLimit || FACTORY->GetPlayer()->GetComponent<Transform>()->position.y < UpYLimit)
			//				CAMERA->cameraPos.y = FACTORY->GetPlayer()->GetComponent<Transform>()->position.y;

			//		}


			//	}
			//	else if (type == CameraRotation::EN_90)
			//	{

			//		// this is current block of map 
			//		upblock = FACTORY->RightBoundary()->GetComponent<Transform>()->position;
			//		downblock = FACTORY->LeftBoundary()->GetComponent<Transform>()->position;
			//		leftblock = FACTORY->UpBoundary()->GetComponent<Transform>()->position;
			//		rightblock = FACTORY->DownBoundary()->GetComponent<Transform>()->position;

			//		CAMERA->CenterOfCamera.x = downblock.x + ((upblock.x - downblock.x)*.5f);
			//		CAMERA->CenterOfCamera.y = leftblock.y + ((rightblock.y - leftblock.y)*.5f);
			//		if ((upblock.y - downblock.y) < APP->_screenHeight && (rightblock.x - leftblock.x) < APP->_screenWidth) // when wall boundaries are smaller than camera size
			//		{
			//			float UpYLimit, DownYLimit;
			//			UpYLimit = (upblock.x - std::abs((upblock.x - CAMERA->CenterOfCamera.x))*.5f);
			//			DownYLimit = (downblock.x + std::abs(CAMERA->CenterOfCamera.x - downblock.x)*.5f);

			//			if (abs(upblock.x - downblock.x)> APP->_screenHeight) //when vertical different is bigger than screen _height size.
			//			{

			//				if (!IsCamMovVer)
			//				{
			//					if (STATEMANAGER->Loadtolevelname == "level6.json")
			//					{
			//						if (CAMERA->cameraPos.x > DownYLimit)
			//							CAMERA->cameraPos.x -= 5.f;
			//						else if (CAMERA->cameraPos.x <= DownYLimit)
			//							IsCamMovVer = !IsCamMovVer;
			//					}
			//					else
			//					{
			//						if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x >= UpYLimit)
			//							CAMERA->cameraPos.x = UpYLimit;
			//						else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x <= DownYLimit)
			//							CAMERA->cameraPos.x = DownYLimit;
			//						else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x < UpYLimit || FACTORY->GetPlayer()->GetComponent<Transform>()->position.x > DownYLimit)
			//							CAMERA->cameraPos.x = FACTORY->GetPlayer()->GetComponent<Transform>()->position.x;
			//					}
			//				}

			//				else {
			//					if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x >= UpYLimit)
			//						CAMERA->cameraPos.x = UpYLimit;
			//					else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x <= DownYLimit)
			//						CAMERA->cameraPos.x = DownYLimit;
			//					else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x < UpYLimit || FACTORY->GetPlayer()->GetComponent<Transform>()->position.x > DownYLimit)
			//						CAMERA->cameraPos.x = FACTORY->GetPlayer()->GetComponent<Transform>()->position.x;
			//				}
			//			}
			//			else // have fix camera pos at the center.
			//			{
			//				CAMERA->cameraPos.x = CAMERA->CenterOfCamera.x;
			//				CAMERA->cameraPos.y = CAMERA->CenterOfCamera.y;
			//			}

			//		}
			//		else
			//		{
			//			CAMERA->CenterOfCamera.x = downblock.x + ((upblock.x - downblock.x)*.5f);
			//			CAMERA->CenterOfCamera.y = leftblock.y + ((rightblock.y - leftblock.y)*.5f);

			//			float UpYLimit, DownYLimit;// , LeftXLimit, RightXLimit;
			//			// 90 degrees rotated x becomes Y 
			//			// only this applies to 90 degrees, 
			//			UpYLimit = (upblock.x - std::abs((upblock.x - CAMERA->CenterOfCamera.x))*.5f);
			//			DownYLimit = (downblock.x + std::abs(CAMERA->CenterOfCamera.x - downblock.x)*.5f);


			//			if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x >= UpYLimit)
			//				CAMERA->cameraPos.x = UpYLimit;
			//			else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x <= DownYLimit)
			//				CAMERA->cameraPos.x = DownYLimit;
			//			else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x < UpYLimit || FACTORY->GetPlayer()->GetComponent<Transform>()->position.x > DownYLimit)
			//				CAMERA->cameraPos.x = FACTORY->GetPlayer()->GetComponent<Transform>()->position.x;
			//		}
			//	}
			//	else if (type == CameraRotation::EN_180)
			//	{
			//		if (IsCamMovVer)
			//			IsCamMovVer = !IsCamMovVer;

			//		// this is current block of map 
			//		upblock = FACTORY->DownBoundary()->GetComponent<Transform>()->position;
			//		downblock = FACTORY->UpBoundary()->GetComponent<Transform>()->position;
			//		rightblock = FACTORY->LeftBoundary()->GetComponent<Transform>()->position;
			//		leftblock = FACTORY->RightBoundary()->GetComponent<Transform>()->position;

			//		if ((upblock.y - downblock.y) < APP->_screenHeight && (rightblock.x - leftblock.x) < APP->_screenWidth) // when wall boundaries are smaller than camera size
			//		{
			//				
			//		}
			//		else
			//		{
			//			CAMERA->CenterOfCamera.x = leftblock.x - (std::abs(leftblock.x - rightblock.x)*.5f);
			//			CAMERA->CenterOfCamera.y = downblock.y - (std::abs(upblock.y - downblock.y)*.5f);

			//			// To do: seprate sections of map 
			//			float UpYLimit, DownYLimit, LeftXLimit, RightXLimit;

			//			UpYLimit = upblock.y + std::abs(upblock.y - CAMERA->CenterOfCamera.y)*.5f;
			//			DownYLimit = downblock.y - std::abs(CAMERA->CenterOfCamera.y - downblock.y)*.5f;
			//			LeftXLimit = leftblock.x - std::abs(CAMERA->CenterOfCamera.x - leftblock.x)*.5f;
			//			RightXLimit = rightblock.x + std::abs(rightblock.x - CAMERA->CenterOfCamera.x)*.5f;

			//			if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x <= RightXLimit)         // when player is on the left.
			//				CAMERA->cameraPos.x = RightXLimit;
			//			else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x >= LeftXLimit)   // when player is on the right
			//				CAMERA->cameraPos.x = LeftXLimit;
			//			else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x < LeftXLimit || FACTORY->GetPlayer()->GetComponent<Transform>()->position.x > RightXLimit)
			//				CAMERA->cameraPos.x = FACTORY->GetPlayer()->GetComponent<Transform>()->position.x;

			//			if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.y <= UpYLimit)         // when player is on the left.
			//				CAMERA->cameraPos.y = UpYLimit;
			//			else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.y >= DownYLimit)   // when player is on the right
			//				CAMERA->cameraPos.y = DownYLimit;
			//			else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.y < DownYLimit || FACTORY->GetPlayer()->GetComponent<Transform>()->position.y > UpYLimit)
			//				CAMERA->cameraPos.y = FACTORY->GetPlayer()->GetComponent<Transform>()->position.y;
			//		}

			//	}
			//	else if (type == CameraRotation::EN_270)
			//	{
			//		// this is current block of map 
			//		upblock = FACTORY->LeftBoundary()->GetComponent<Transform>()->position;
			//		downblock = FACTORY->RightBoundary()->GetComponent<Transform>()->position;
			//		leftblock = FACTORY->DownBoundary()->GetComponent<Transform>()->position;
			//		rightblock = FACTORY->RightBoundary()->GetComponent<Transform>()->position;
			//		float UpYLimit, DownYLimit;
			//		// 270 degrees rotated x becomes Y 
			//		// only this applies to 270 degrees, 
			//		CAMERA->CenterOfCamera.x = downblock.x + ((upblock.x - downblock.x)*.5f);
			//		CAMERA->CenterOfCamera.y = rightblock.x + ((leftblock.x - rightblock.x)*.5f);
			//		UpYLimit = upblock.x + std::abs(upblock.x - CAMERA->CenterOfCamera.x)*.5f;
			//		DownYLimit = downblock.x - std::abs(CAMERA->CenterOfCamera.x - downblock.x)*.5f;


			//		if ((upblock.y - downblock.y) < APP->_screenHeight && (rightblock.x - leftblock.x) < APP->_screenWidth) // when wall boundaries are smaller than camera size
			//		{
			//			if (abs(upblock.x - downblock.x) > APP->_screenHeight)
			//			{
			//				if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x <= UpYLimit)
			//					CAMERA->cameraPos.x = UpYLimit;
			//				else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x >= DownYLimit)
			//					CAMERA->cameraPos.x = DownYLimit;
			//				else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x > UpYLimit || FACTORY->GetPlayer()->GetComponent<Transform>()->position.x < DownYLimit)
			//					CAMERA->cameraPos.x = FACTORY->GetPlayer()->GetComponent<Transform>()->position.x;
			//			}

			//		}
			//		else
			//		{
			//			CAMERA->CenterOfCamera.x = downblock.x + ((upblock.x - downblock.x)*.5f);
			//			CAMERA->CenterOfCamera.y = rightblock.y + ((leftblock.y - rightblock.y)*.5f);
			//			// 90 degrees rotated x becomes Y 
			//			// only this applies to 90 degrees, 
			//			UpYLimit = (upblock.x - std::abs((upblock.x - CAMERA->CenterOfCamera.x))*.5f);
			//			DownYLimit = (downblock.x + std::abs(CAMERA->CenterOfCamera.x - downblock.x)*.5f);

			//			if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x <= UpYLimit)
			//				CAMERA->cameraPos.x = UpYLimit;
			//			else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x >= DownYLimit)
			//				CAMERA->cameraPos.x = DownYLimit;
			//			else if (FACTORY->GetPlayer()->GetComponent<Transform>()->position.x > UpYLimit || FACTORY->GetPlayer()->GetComponent<Transform>()->position.x < DownYLimit)
			//				CAMERA->cameraPos.x = FACTORY->GetPlayer()->GetComponent<Transform>()->position.x;
			//		}
			//	}
			//}
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