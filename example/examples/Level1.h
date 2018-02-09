/******************************************************************************/
/*!
\file   level1.h
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

All levels is running in this state
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "BaseStage.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Level1 :public BaseStage
{
public:
	Level1();
	~Level1();
	void Load(void);
	void Init(void);
	void Update(float dt);
	void Free(void);
	void Unload(void);


	void zoomintoPlayer();
	void CamMoveToPlayer(float dt);
	void lookAtMap();
	//void CamMoveToCenter();
	void ShakeCamera(float dt);
	void Setshakeduration(float duration) { shakeDuration = duration; }
	void FollowPlayer(glm::vec3 startCamPos, float dt);

	
	//bool increasing;
	bool CenterToPlayer;
	bool ZoomInToPlayer;
	bool XmovedCompleted;
	bool YmovedCompleted;



	bool moving;
	bool movingToCenter;

private:
	glm::vec3 cameraOriginPos;
	// how long the object should shake for
	float shakeDuration;
	// amplitude of the shake. A larger value shakes the camera harder.
	float shakeAmount = 3.0f;
	float decreaseFactor = 1.0f;

	float _camPaceSpeed;
	glm::vec3 _camPacedirction;
	glm::vec3 _camStartPosition;


private:
	int Level1Counter;
};