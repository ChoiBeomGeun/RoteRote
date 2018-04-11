#pragma once
#include "Camera.h"
#include "Object.h"

enum CameraPosType {
	EN_Default,
	EN_Splash,
	EN_Menu,
	EN_LevelSelect,
	EN_playerPos,
	EN_BOUNDARY,
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
		void FollowPlayer(glm::vec3 * cameraPos, glm::vec3 * cameraScale, float dt);
		bool isCamToPlayer;

	public: // camera rotating changing walls;
		int Rotating90DegreesCam();
		void Rotating180DegreesCam();
		void HorizontalLockCam(CameraRotation type);
		void VerticalLockCam(CameraRotation type);

	private:
		// how long the object should shake for
		float shakeDuration;
		// amplitude of the shake. A larger value shakes the camera harder.
		float shakeAmount;
		float decreaseFactor;
		float _camPaceSpeed;
		
		bool istest;


	};
}