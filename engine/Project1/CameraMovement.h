/******************************************************************************/
/*!
\file   CameraMovement.h
\author HyunJun Yoo
\par    email: hyunjun306@gmail.com
\par    course: GAM250
\par	ThumbUp Engine
\date   06/13/2018

This is interface for CameraMovement
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once
#include "Camera.h"
#include "Object.h"
#include "Input.h"

enum CameraPosType {
	EN_Default,
	EN_Splash,
	EN_Menu,
	EN_LevelSelect,
	EN_playerPos,
	EN_BOUNDARY,
	EN_Option,
	End,
};

enum CameraRotation
{
	EN_0 =0,
	EN_90,
	EN_180,
	EN_270,
	EN_END
};
namespace TE
{
	class CameraAction
	{
	public:
		// Cont Dest
		CameraAction();
		~CameraAction();

	public: // shaking camera
		void ShakeCamera(float dt);
		void Setshakeduration(float duration) { shakeDuration = duration; }
		glm::vec3 cameraOriginPos;

	public: // setting the camera
		void cameraSetting(CameraPosType camPosType);
		void Update(float dt);
		void moveCameraPos(glm::vec2 cameraPOS);

	public: // camera movement
		bool DisplayTheWholeMap();
		bool cam_move_interpolate(glm::vec2 destination, float dt);
		

	public: // camera rotating changing walls;
		int RotatingCam();
		void ControlCamMovement(int type, float dt);
		bool IsCamMovHoz, IsCamMovVer;
	private:
		bool iscam_interpol;
		// how long the object should shake for
		float shakeDuration;
		// amplitude of the shake. A larger value shakes the camera harder.
		float shakeAmount;
		float decreaseFactor;
		float _camPaceSpeed;
	};
}