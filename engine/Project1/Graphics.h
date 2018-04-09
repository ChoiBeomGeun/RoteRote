#pragma once

/**
\file        Graphics.h
\author      HyunJun Yoo
\par         email: hyunjun306@gmail.com
\par         course: CS225
\date        12/11/2017
\brief

GLSLProgram header file.
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include <SDL.h>

#include "System.h"
#include "Input.h"
#include "Application.h"
#include "DebugUtil.h"
#include "Factory.h"

#include "Transform.h"
#include "Sprite.h"

#include "LevelManager.h"
#include "ParticleManager.h"
#include "Engine.h"

#include "Camera.h"
#include "GLSLProgram.h"
#include "Vertex.h"
#include "Particles.h"
#include "Animation.h"

#include <vector>
#include <map>

#include <glew.h>

#include <fstream>//file stream open file
#include <sstream>// string stream file->string

#include  "imgui.h"
#include  "imgui_impl_sdl_gl3.h"

#include "json/json.h"
#include "imgui_internal.h"
#include "ImguiGraphics.h"
#include "json/JsonClass.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace TE {

	enum UNIFORMLOC {
		TEXTURE,
		SAMPLER,
		COLOR,
		MODEL,
		VIEW,
		PROJ,
		DRAWINGSTATUS,
		HUDMODEL,
		ISANIMATION,
		FLIPX,
		ISJUMPING,
		ANIMATIONX,
		TIME,
		END
	};
	enum PARTICLELOC
	{
		PARTICLEMODEL,
		PARTICLEVIEW,
		PARTICLEPROJ,
		PTEXTURE,
		PSAMPLER,
		PCOLOR,
		PSTATS,
		PEND
	};

	//forward declartion of Sprite Component
	class Sprite;
	class Animation;
	class Emitter;
	class Graphics : public Systems
	{
	public:
		Graphics();
		//Init the system
		void Initialize(void) override;


		//void ChangeScale(void);
		//All systems are updated every frame
		void Update(float dt) override;
		void framebuffer_size_callback(SDL_Window * window, int width, int height);


		~Graphics();

		std::vector<Sprite*> SpriteList;
		std::vector<Animation *> AnimationList;
		std::map<std::string, GLuint> m_textureMap;
		//std::vector<Emitter *> EmitterList;

		void initbasicShader();
		void initparticleShader();
		void drawbasic_attributes();
		void drawparticle_attributes();

		void drawPerspective(std::vector<Sprite*>::iterator iter);
		void drawOrthogonal(std::vector<Sprite*>::iterator iter);
		void drawParticles(std::vector<Sprite*>::iterator iter);
		void setbasicUniformLoc();
		void setparticleUniformLoc();
		void animationSetting();
		void cameraSetting();
		void MergeList();
		void FreeList();

		//void drawLine(bool isdraw);
	private:
		int drawStats;
		int particleDraw;
		bool isParticle;
		GLuint buffer, basicVAO/*, particleVAO*/;
		//GLuint particlebuffer;
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 hudmodel = glm::mat4(1.f);
		glm::mat4 particlemodel = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		Camera * pCamera;
		GLint uniformLocation[END];
		GLint particleLoc[PEND];
		GLSLProgram _basicProgram;
		GLSLProgram _particleProgram;

		Vertex vertexData[6];
		float time;
		float splashtime;

	};

	extern Graphics * GRAPHICS;
}