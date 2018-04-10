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
namespace TE
{
	class CameraAction
	{
	public:
		
		CameraAction();
		~CameraAction();
		void ShakeCamera(float dt);
		void Setshakeduration(float duration) { shakeDuration = duration;}

		glm::vec3 cameraOriginPos;
		void FollowPlayer(glm::vec3 * cameraPos, glm::vec3 * cameraScale, float dt);
		bool isCamToPlayer;
		void cameraSetting(CameraPosType camPosType);
		bool DisplayTheWholeMap();
		void moveCameraPos(glm::vec2 cameraPOS);
		void Update(float dt);
		void Rotating90DegreesCam();
		void Rotating180DegreesCam();


	private:
		// how long the object should shake for
		float shakeDuration;
		// amplitude of the shake. A larger value shakes the camera harder.
		float shakeAmount;
		float decreaseFactor;
		float _camPaceSpeed;
		


	};

}