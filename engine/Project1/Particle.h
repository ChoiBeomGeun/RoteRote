//#pragma once
//
//#include "Component.h"
//#include "GameLogic.h"
//#include  <string>
//
//#include "glew.h"
//#include "SDL.h"
//#include "Vertex.h"
//#include <glm\glm.hpp>
//#include <vector>
//
//namespace TE {
//	class Particle : public Component
//	{
//	public:
//		Particle();
//
//		~Particle();
//		void update(float dt);
//
//		virtual void Initialize() override;
//		
//		
//
//		glm::vec3 position = glm::vec3(0.0f);
//		glm::vec3 velocity = glm::vec3(0.0f);
//
//		GLuint & texutre_load(std::string filepath);
//	private:
//		int findFreeParticle();
//		Color color;
//		float life = 0.0f;
//		float width = 0.0f;
//		float m_decayRate = 0.1f;
//		int m_maxParticles = 0;
//		int m_lastFreeParticle = 0;
//		GLuint m_textureID;
//		std::string m_TexutureDir;
//
//	};
//}