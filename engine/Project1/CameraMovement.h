#pragma once
#include "Camera.h"
#include "Object.h"


namespace TE
{
	class CameraAction
	{
	public:
		CameraAction();
		~CameraAction();
		void ShakeCamera(float dt);
		void Setshakeduration(float duration) { shakeDuration = duration; }

		glm::vec3 cameraOriginPos;
		void FollowPlayer(glm::vec3 * cameraPos, glm::vec3 * cameraScale, float dt);
		bool isCamToPlayer;
		void cameraSetting(glm::vec3 cameraPOS);
		bool DisplayTheWholeMap();
		void moveCameraPos(glm::vec2 cameraPOS);
		void Update(float dt);
	private:
		// how long the object should shake for
		float shakeDuration;
		// amplitude of the shake. A larger value shakes the camera harder.
		float shakeAmount;
		float decreaseFactor;
		float _camPaceSpeed;
		


	};

}