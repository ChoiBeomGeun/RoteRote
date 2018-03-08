/**
\file        Graphics.cpp
\author      HyunJun Yoo
\par         email: hyunjun306@gmail.com
\par         course: CS225
\date        12/16/2017
\brief

Graphics.cpp,
This is where it get all sprite and animation data to redner objects
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/

#include "Graphics.h"
#include <algorithm>

static glm::vec3 eye = { 0,0,999 };
static glm::vec3 target{ 0,0,-1 };
static glm::vec3 up{ 0,1,0 };
//bool moving;

namespace TE {
	Graphics * GRAPHICS = nullptr;
}
using namespace TE;

Graphics::Graphics()
{
	DEBUG_ASSERT(GRAPHICS != nullptr, "Graphics are no more 2");
	GRAPHICS = this;
}
void TE::Graphics::initbasicShader()
{
	std::string path = "TEGraphics";
	char * Userinfo;
	size_t len = path.size();
	_dupenv_s(&Userinfo, &len, "USERPROFILE");

	std::string saveLevel = path;
#ifdef _DEBUG
	path = ".\\shaders.\\" + path;
#else
	path = Userinfo;
	path += "/Documents/RoteRote/shaders/" + saveLevel;
#endif
	free(Userinfo);
	_basicProgram.compileShaders(path + ".vert", path + ".frag");
	_basicProgram.addAttribute("vertexPosition");
	_basicProgram.addAttribute("vertexColor");
	_basicProgram.addAttribute("vertexUV");

	_basicProgram.linkShaders();
}

void TE::Graphics::initparticleShader()
{
	std::string path = "TEParticle";
	char * Userinfo;
	size_t len = path.size();
	_dupenv_s(&Userinfo, &len, "USERPROFILE");

	std::string saveLevel = path;
#ifdef _DEBUG
	path = ".\\shaders.\\" + path;
#else
	path = Userinfo;
	path += "/Documents/RoteRote/shaders/" + saveLevel;
#endif
	free(Userinfo);

	_particleProgram.compileShaders(path + ".vert", path + ".frag");
	_particleProgram.addAttribute("verParticlePos");
	_particleProgram.addAttribute("verParticleCol");
	_particleProgram.addAttribute("verParticleUV");

	_particleProgram.linkShaders();
}


void Graphics::Initialize(void)
{
	isParticle = false;
	moving = true;
	pCamera = new Camera;
	CAMERA = pCamera;
	drawStats = 0;

	isMapEditorOn = false;
	drawGrid = false;
	//loading OPENGL Funciton
	DEBUG_ASSERT(glewInit() != GLEW_OK, "Initializing GLEW is failed\n");

	// initialize Shader
	initbasicShader();


	if (buffer == 0)
		glGenBuffers(1, &buffer);
	if (basicVAO == 0)
		glGenVertexArrays(1, &basicVAO);
	/*if (particleVAO == 0)
	glGenBuffers(1, &particleVAO);*/


	// first triangle
	vertexData[0].setPosition(-0.5f, 0.5f, 0.0f);
	vertexData[0].setColor(255, 255, 255, 255);
	vertexData[0].setUV(0.0f, 1.0f); // 0,1 // flip 1,1

	vertexData[1].setPosition(0.5f, 0.5f, 0.0f);
	vertexData[1].setColor(255, 255, 255, 255);
	vertexData[1].setUV(1.0f, 1.0f); // 1, 1 // flip 0, 1

	vertexData[2].setPosition(0.5f, -0.5f, 0.0f);
	vertexData[2].setColor(255, 255, 255, 255);
	vertexData[2].setUV(1.0f, 0.0f); // 1, 0 // flip 0, 0

									 // second triangle
	vertexData[3].setPosition(0.5f, -0.5f, 0.0f);
	vertexData[3].setColor(255, 255, 255, 255);
	vertexData[3].setUV(1.0f, 0.0f); // 1, 0 // flip 0,0

	vertexData[4].setPosition(-0.5f, -0.5f, 0.0f);
	vertexData[4].setColor(255, 255, 255, 255);
	vertexData[4].setUV(0.0f, 0.0f); // 0,0 // flip 1,0

	vertexData[5].setPosition(-0.5f, 0.5f, 0.0f);
	vertexData[5].setColor(255, 255, 255, 255);
	vertexData[5].setUV(0.0f, 1.0f); // 0, 1 // flip 1, 1


									 //create opengl buffer object

									 // tell opengl to bind our vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	// upload the data to the GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glBindVertexArray(basicVAO);
	//glBindVertexArray(particleVAO);
	// unbind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0); // 0 mean unbind

	drawbasic_attributes();
	CAMERA->cameraPos = eye;
	CAMERA->cameraTarget = target;
	CAMERA->cameraUp = up;


	setbasicUniformLoc();
	initparticleShader();
	drawparticle_attributes();
	setparticleUniformLoc();

#ifdef _DEBUG
	ImGui_ImplSdlGL3_Init(APP->pWnd);
	ImVec4 clear_color = ImColor(114, 144, 154);
#endif
	time = 0.f;
	splashtime = 0.f;
	//   ImGuiIO& io = ImGui::GetIO();
}


void TE::Graphics::drawbasic_attributes()
{
	// bind the buffer object
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// the opengl that we want to use the first 
	// attribute array. we only need one array right
	// now since we are only using position
	glEnableVertexAttribArray(0);

	// this is the position attribute pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	//// this is the color attribute pointer
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	// this is the UV attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	// draw the 6 vertices to the screen
	//glDrawArrays(GL_TRIANGLES, 0, 6); // drawing 6 vertices 

	// disable the vertex attrib array. this is not optional
	glDisableVertexAttribArray(0);
	//glEnableVertexAttribArray(2);
	// unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0); // 0 mean unbind
}

void Graphics::drawparticle_attributes()
{
	// bind the buffer object
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// thee opengl that we want to use the first 
	// attribute array. we only need one array right
	// now since we are only using position
	glEnableVertexAttribArray(0);

	// this is the position attribute pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	//// this is the color attribute pointer
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	// this is the UV attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	// draw the 6 vertices to the screen
	//	glDrawArrays(GL_TRIANGLES, 0, 6); // drawing 6 vertices 


	//glEnableVertexAttribArray(0);// disable the vertex attrib array. this is not optional
	glDisableVertexAttribArray(0);

	// unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0); // 0 mean unbind
}



// ALL systems are updated every frame
void Graphics::Update(float dt)
{
	Sprite::sortSprites(SortType::FRONT_TO_BACK);

	glClearColor(1.f, 1.f, 1.f, .5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	time += dt;
	splashtime += dt;

	glUniform1i(uniformLocation[TEXTURE], 1);/*True*/
	glUniform1i(uniformLocation[SAMPLER], 0);
	glUniform1i(particleLoc[PTEXTURE], 1);
	glUniform1i(particleLoc[PSAMPLER], 0);
	cameraSetting();

	view = CAMERA->view;
	for (std::vector<Sprite*>::iterator it = SpriteList.begin();
		it != SpriteList.end(); ++it) {
		glBindTexture(GL_TEXTURE_2D, (*it)->TextureId);
		animationSetting();
		_basicProgram.use();

		if ((*it)->isPerspective)
			drawPerspective(it);
		else if (!(*it)->isPerspective)
			drawOrthogonal(it);

		drawParticles(it);
		glUniformMatrix4fv(uniformLocation[VIEW], 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(uniformLocation[PROJ], 1, GL_FALSE, &CAMERA->projection[0][0]);
		glUniform4f(uniformLocation[COLOR], (*it)->Color[0], (*it)->Color[1], (*it)->Color[2], (*it)->Color[3]);
		glUniform1i(uniformLocation[DRAWINGSTATUS], drawStats);
		glPushAttrib(GL_CURRENT_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

#ifdef _DEBUG
	ImGuiUpdate();
#endif
	APP->SwapWindow();

}


void Graphics::framebuffer_size_callback(SDL_Window* window, int width, int height)
{
	window = window;
	glViewport(0, 0, width, height);
}




Graphics::~Graphics()
{
	_basicProgram.unuse();
	_particleProgram.unuse();
	delete pCamera;
	pCamera = NULL;
#ifdef _DEBUG
	ImguiFree();
	ImGui_ImplSdlGL3_Shutdown();
#endif
}


void TE::Graphics::drawPerspective(std::vector<Sprite*>::iterator iter)
{
	if (!(*iter)->pOwner->HasComponent<Emitter>())
	{

		if ((*iter)->pOwner->HasComponent<Animation>()) {
			glUniform1i(uniformLocation[ISANIMATION], 1);
		}

		drawStats = 0;
		CAMERA->proj();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3((*iter)->pTransform->position.x, (*iter)->pTransform->position.y, (*iter)->pTransform->position.z));
		model = glm::rotate(model, glm::radians((*iter)->pTransform->angle), (*iter)->pTransform->rotation);
		model = glm::scale(model, glm::vec3((*iter)->pTransform->scale.x, (*iter)->pTransform->scale.y, (*iter)->pTransform->scale.z));
		glUniform1i(particleLoc[PSTATS], drawStats);
		//glUniform1i(uniformLocation[PTEXTURE], 0);
		glUniformMatrix4fv(uniformLocation[MODEL], 1, GL_FALSE, &model[0][0]);
	}
}

void TE::Graphics::drawOrthogonal(std::vector<Sprite*>::iterator iter)
{
	if (!(*iter)->pOwner->HasComponent<Emitter>())
	{
		//_basicProgram.use();
		//glBindTexture(GL_TEXTURE_2D, (*iter)->TextureId);

		drawStats = 1;
		hudmodel = glm::mat4(1.0f);
		hudmodel = glm::translate(hudmodel, (*iter)->pTransform->position);
		hudmodel = glm::scale(hudmodel, (*iter)->pTransform->scale);
		glUniform1i(particleLoc[PSTATS], drawStats);
		//glUniform1i(uniformLocation[PTEXTURE], 0);
		glUniformMatrix4fv(uniformLocation[HUDMODEL], 1, GL_FALSE, &hudmodel[0][0]);
		//_basicProgram.unuse();
	}
}

void TE::Graphics::drawParticles(std::vector<Sprite*>::iterator iter)
{
	if ((*iter)->pOwner->HasComponent<Emitter>())
	{
		_particleProgram.use();

		for (auto p : PARTICLEMANAGER->m_EmitterList)
		{
			glBindTexture(GL_TEXTURE_2D, p->m_textureID);
			for (int i = 0; i<p->capacity; ++i)
			{
				drawStats = 3;
				particlemodel = glm::mat4(1.0f);
				CAMERA->proj();
				particlemodel = glm::translate(particlemodel, p->pParticles[i].pos);
				particlemodel = glm::rotate(particlemodel, glm::radians((*iter)->pTransform->angle), (*iter)->pTransform->rotation);
				particlemodel = glm::scale(particlemodel, glm::vec3(p->pParticles[i].scale));
				glUniform4f(particleLoc[PCOLOR], p->pParticles[i].color[0], p->pParticles[i].color[1],
					p->pParticles[i].color[2], p->pParticles[i].color[3]);
				glUniformMatrix4fv(particleLoc[PARTICLEMODEL], 1, GL_FALSE, &particlemodel[0][0]);
				glUniformMatrix4fv(particleLoc[PARTICLEVIEW], 1, GL_FALSE, &view[0][0]);
				glUniformMatrix4fv(particleLoc[PARTICLEPROJ], 1, GL_FALSE, &CAMERA->projection[0][0]);
				//glUniform1i(particleLoc[PTEXTURE], 1);
				glUniform1i(particleLoc[PSTATS], drawStats);
				glPushAttrib(GL_CURRENT_BIT);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
	}
}

void TE::Graphics::setbasicUniformLoc()
{

	uniformLocation[TEXTURE] = _basicProgram.getUniformLocation("texturing");
	uniformLocation[SAMPLER] = _basicProgram.getUniformLocation("texture2D");
	uniformLocation[COLOR] = _basicProgram.getUniformLocation("colorOffset");
	uniformLocation[MODEL] = _basicProgram.getUniformLocation("model");
	uniformLocation[VIEW] = _basicProgram.getUniformLocation("view");
	uniformLocation[PROJ] = _basicProgram.getUniformLocation("projection");
	uniformLocation[DRAWINGSTATUS] = _basicProgram.getUniformLocation("drawingStatus");
	uniformLocation[HUDMODEL] = _basicProgram.getUniformLocation("hudmodel");
	uniformLocation[ISANIMATION] = _basicProgram.getUniformLocation("isAnimation");
	uniformLocation[FLIPX] = _basicProgram.getUniformLocation("flipx");
	uniformLocation[ISJUMPING] = _basicProgram.getUniformLocation("isJumping");
	uniformLocation[ANIMATIONX] = _basicProgram.getUniformLocation("animationx");
	uniformLocation[TIME] = _basicProgram.getUniformLocation("timer");


}

void TE::Graphics::setparticleUniformLoc()
{
	particleLoc[PARTICLEMODEL] = _particleProgram.getUniformLocation("particlemodel");
	particleLoc[PARTICLEVIEW] = _particleProgram.getUniformLocation("particleview");
	particleLoc[PARTICLEPROJ] = _particleProgram.getUniformLocation("particleprojection");
	particleLoc[PTEXTURE] = _particleProgram.getUniformLocation("ptexturing");
	particleLoc[PSAMPLER] = _particleProgram.getUniformLocation("ptexture2D");
	particleLoc[PCOLOR] = _particleProgram.getUniformLocation("pcolorOffset");
	particleLoc[PSTATS] = _particleProgram.getUniformLocation("particleStats");
}


void TE::Graphics::animationSetting()
{
	auto aniIter = AnimationList.begin();
	for (aniIter; aniIter != AnimationList.end(); ++aniIter)
	{
		if ((*aniIter)->pOwner->HasComponent<Animation>()) {
			if ((*aniIter)->pOwner->objectstyle == Player)
			{
				glUniform1i(uniformLocation[ISANIMATION], 1);
				glUniform1f(uniformLocation[ANIMATIONX], /*aniX*/(*aniIter)->getFrame());
				glUniform1f(uniformLocation[TIME], (*aniIter)->getTime()/*1.0f / 6*/);
				if (time >= .125f)
				{
					// when key is pressed
					if ((*aniIter)->isPressed())
					{
						(*aniIter)->IterateFrame((*aniIter)->getTime());
					}
					if ((*aniIter)->isJumping)
					{
						(*aniIter)->setFrame(0.0f);
						glUniform1i(uniformLocation[ISJUMPING], 1);
					}
					else
					{
						glUniform1i(uniformLocation[ISJUMPING], 0);
					}

					if ((*aniIter)->_isOnWall)
					{
						(*aniIter)->setFrame(.75f);
					}
					else {}
					time = 0.0f;
					//std::cout << (*aniIter)->getFrame() << '\n';
				}



				// if player is flipped it should face other direction
				if ((*aniIter)->isFlippedX())
					glUniform1i(uniformLocation[FLIPX], 1);

				if (!(*aniIter)->isFlippedX())
					glUniform1i(uniformLocation[FLIPX], 0);

			}
			else
			{
				if (splashtime >= .1f)
				{
					glUniform1i(uniformLocation[ISANIMATION], 1);
					glUniform1f(uniformLocation[ANIMATIONX], /*aniX*/(*aniIter)->getFrame());
					glUniform1f(uniformLocation[TIME], (*aniIter)->getTime()/*1.0f / 6*/);
					(*aniIter)->IterateSplash((*aniIter)->getTime());
					splashtime = 0.0f;
					if ((*aniIter)->getFrame() >= .95f)
						(*aniIter)->setFrame(0.96875f);
				}
			}
		}
		if ((*aniIter)->getFrame() >= 1.0f)
			(*aniIter)->setFrame(0.0f);
	}
	glUniform1i(uniformLocation[ISANIMATION], 0);
}

void TE::Graphics::cameraSetting()
{
	if (CAMERA->isCentered)
		CAMERA->lookat(CAMERA->cameraPos, CAMERA->cameraTarget, CAMERA->cameraUp);
	else
	{
		if (FACTORY->GetPlayer())
		{
			if (!CAMERA->IsCameraShaking)
				CAMERA->cameraPos = FACTORY->GetPlayer()->GetComponent<Transform>()->position;

			CAMERA->cameraPos.z = 999.f;
			CAMERA->lookat(CAMERA->cameraPos, CAMERA->cameraTarget, CAMERA->cameraUp);
		}
	}
}
