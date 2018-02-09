/*
Transform.h
BeomGeun Choi


*/

#pragma once
#include "Component.h"
//#include "vector3.h"
#include "ComponentType.h"
//#include "Mat44.h"
#include "TUMath.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Application.h"


namespace TE {
	class Transform : public Component {

	public:
		Transform() : Component(ComponentType::CT_TRANSFORM)
		{
			position = glm::vec3(0, 0, 0);
			rotation = glm::vec3(0, 0, 0);
			scale = glm::vec3(1, 1, 1);
			angle = 0.0f;
		}
		///~Transform();
	
		glm::vec3 GetPosition() { return position; }
		glm::vec3 GetRotation() { return rotation; }
		glm::vec3 GetScale() { return scale; }
		float GetRotationZ() { return angle; }

		void SetRotationZ(float zangle) { angle = TUMath::DegreeToRadian(zangle); }
		void SetPosition(const glm::vec3& pos) { position = pos; }
		void SetRotation(const glm::vec3& Rot) { rotation = Rot; }
		void SetScale(const glm::vec3& Scale) { scale = Scale; }
		
		float angle;
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

		// helper function
		/*glm::vec3 changeVec3PosToglm(const Vector3& pos);
		glm::vec3 changeVec3ScaleToglm(const Vector3& scale);
		glm::vec3 changeVec3RotateToglm(const Vector3& rotate);*/
	private:

	};




}