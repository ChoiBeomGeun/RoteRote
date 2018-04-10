#include "GameLogicSystem.h"
#include "InGameLogic.h"
#include "Object.h"
#include "Camera.h"
#include "CameraMovement.h"

#pragma once

namespace TE {
	class CameraLogic : public GameLogicSystem
	{
	public:
		// Initialize the system
		void Initialize() override;
		// All systems are updated every frames
		void Update(float dt) override;
		void Free(void) override;

	private:
		CameraAction _cameraLogicCam;
	};
	extern CameraLogic * CAMERALOGIC;




}
