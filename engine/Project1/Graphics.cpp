/******************************************************************************/
/*!
\file   Graphics.cpp
\author   HyunJun Yoo
\par   email: hyunjun306@gmail.com
\par   Class:GAM250
\par   ThumbUp Engine
\date   06/13/2017

Graphics.cpp,
This is where it get all sprite and animation data to redner objects
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/


#include "Graphics.h"
#include <algorithm>

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
	path = ".\\shaders.\\" + path;
	_basicProgram.compileShaders(path + ".vert", path + ".frag");
	_basicProgram.addAttribute("vertexPosition");
	_basicProgram.addAttribute("vertexColor");
	_basicProgram.addAttribute("vertexUV");
	_basicProgram.linkShaders();
}

void TE::Graphics::initparticleShader()
{
	std::string path = "TEParticle";
	std::string saveLevel = path;
	path = ".\\shaders.\\" + path;
	_particleProgram.compileShaders(path + ".vert", path + ".frag");
	_particleProgram.addAttribute("verParticlePos");
	_particleProgram.addAttribute("verParticleCol");
	_particleProgram.addAttribute("verParticleUV");
	_particleProgram.linkShaders();
}


void Graphics::Initialize(void)
{
	isParticle = false;

	pCamera = new Camera();
	CAMERA = pCamera;
	drawStats = 0;

	//loading OPENGL Funciton
	DEBUG_ASSERT(glewInit() != GLEW_OK, "Initializing GLEW is failed\n");

	// initialize Shader
	initbasicShader();

	if (buffer == 0)
		glGenBuffers(1, &buffer);
	if (basicVAO == 0)
		glGenVertexArrays(1, &basicVAO);

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
	// unbind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0); // 0 mean unbind

	drawbasic_attributes();
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

	Sprite::LoadAllSprites();


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
	//   glDrawArrays(GL_TRIANGLES, 0, 6); // drawing 6 vertices 


	//glEnableVertexAttribArray(0);// disable the vertex attrib array. this is not optional
	glDisableVertexAttribArray(0);

	// unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0); // 0 mean unbind
}

// ALL systems are updated every frame
void Graphics::Update(float dt)
{
	Sprite::sortSprites(SortType::FRONT_TO_BACK);

	glEnable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	time += dt;
	splashtime += dt;

	glUniform1i(uniformLocation[TEXTURE], 1);/*True*/
	glUniform1i(uniformLocation[SAMPLER], 0);
	glUniform1i(particleLoc[PTEXTURE], 1);
	glUniform1i(particleLoc[PSAMPLER], 0);

	view = CAMERA->view;
	CAMERA->update();
	for (std::vector<Sprite*>::iterator it = SpriteList.begin();
		it != SpriteList.end(); ++it) {
		glBindTexture(GL_TEXTURE_2D, (*it)->m_TextureID);
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
		//glPushAttrib(GL_CURRENT_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

#ifdef _DEBUG
	ImGuiUpdate();
#endif
	APP->SwapWindow();
	glDisable(GL_BLEND);
	glDepthMask(GL_FALSE);


}

void Graphics::framebuffer_size_callback(SDL_Window* window, int width, int height)
{
	window = window;
	glViewport(0, 0, width, height);
}

Graphics::~Graphics()
{
	for (auto texID : GRAPHICS->SpriteList)
	{
		glDeleteTextures(1, &texID->m_TextureID);
	}
	SpriteList.clear();
	AnimationList.clear();
	_basicProgram.unuse();
	_particleProgram.unuse();
	delete pCamera;
	pCamera = NULL;
#ifdef _DEBUG
	ImguiFree();
	ImGui::Shutdown();
	ImGui_ImplSdlGL3_Shutdown();
#endif
	glDeleteVertexArrays(1, &buffer);
	glDeleteBuffers(1, &basicVAO);

	Sprite::UnLoadAllSprites();
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
		model = glm::translate(model, glm::vec3((*iter)->pOwner->GetComponent<Transform>()->position));
		model = glm::rotate(model, glm::radians((*iter)->pOwner->GetComponent<Transform>()->angle), (*iter)->pOwner->GetComponent<Transform>()->rotation);
		model = glm::scale(model, glm::vec3((*iter)->pOwner->GetComponent<Transform>()->scale));

		if ((*iter)->pOwner->objectstyle == Objectstyle::Box)
			glUniform2f(uniformLocation[UV], (*iter)->pOwner->GetComponent<Transform>()->u_v.x, (*iter)->pOwner->GetComponent<Transform>()->u_v.y);
		else
			glUniform2f(uniformLocation[UV], (*iter)->pOwner->GetComponent<Transform>()->u_v.x, (*iter)->pOwner->GetComponent<Transform>()->u_v.y);

		glUniformMatrix4fv(uniformLocation[MODEL], 1, GL_FALSE, &model[0][0]);
	}
}

void TE::Graphics::drawOrthogonal(std::vector<Sprite*>::iterator iter)
{
	if (!(*iter)->pOwner->HasComponent<Emitter>())
	{
		CAMERA->unproj();

		drawStats = 1;
		hudmodel = glm::mat4(1.0f);
		hudmodel = glm::translate(hudmodel, (*iter)->pOwner->GetComponent<Transform>()->position);
		if ((*iter)->isRotating)
			hudmodel = glm::rotate(hudmodel, glm::radians((*iter)->pOwner->GetComponent<Transform>()->angle), (*iter)->pOwner->GetComponent<Transform>()->rotation);

		hudmodel = glm::scale(hudmodel, (*iter)->pOwner->GetComponent<Transform>()->scale);
		glUniformMatrix4fv(uniformLocation[HUDMODEL], 1, GL_FALSE, &hudmodel[0][0]);
	}
}

void TE::Graphics::drawParticles(std::vector<Sprite*>::iterator iter)
{
	if ((*iter)->pOwner->HasComponent<Emitter>())
	{
		_particleProgram.use();

		for (auto p : PARTICLEMANAGER->m_EmitterList)
		{
			glBindTexture(GL_TEXTURE_2D, p->pOwner->GetComponent<Sprite>()->m_TextureID);
			if (p->isOn)
			{
				if (p->pParticles)
					for (int i = 0; i < p->capacity; ++i)
					{
						drawStats = 3;
						particlemodel = glm::mat4(1.0f);
						CAMERA->proj();
						particlemodel = glm::translate(particlemodel, p->pParticles[i].pos);
						particlemodel = glm::rotate(particlemodel, glm::radians(p->pParticles[i].angle), (*iter)->pOwner->GetComponent<Transform>()->rotation);
						particlemodel = glm::scale(particlemodel, p->pParticles[i].scale);
						glUniform4f(particleLoc[PCOLOR], p->pParticles[i].color[0], p->pParticles[i].color[1],
							p->pParticles[i].color[2], p->pParticles[i].color[3]);
						glUniformMatrix4fv(particleLoc[PARTICLEMODEL], 1, GL_FALSE, &particlemodel[0][0]);
						glUniformMatrix4fv(particleLoc[PARTICLEVIEW], 1, GL_FALSE, &view[0][0]);
						glUniformMatrix4fv(particleLoc[PARTICLEPROJ], 1, GL_FALSE, &CAMERA->projection[0][0]);
						glUniform1i(particleLoc[PSTATS], drawStats);
						//glPushAttrib(GL_CURRENT_BIT);
						glDrawArrays(GL_TRIANGLES, 0, 6);
					}
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
	uniformLocation[UV] = _basicProgram.getUniformLocation("uv");


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
				glUniform1f(uniformLocation[ANIMATIONX], (*aniIter)->getFrame());
				glUniform1f(uniformLocation[TIME], (*aniIter)->getTime());
				if (time >= (*aniIter)->getTime())
				{
					// when key is pressed
					if ((*aniIter)->isPressed())
					{
						(*aniIter)->IterateFrame((*aniIter)->getTime(), (*aniIter)->_limit_frame);
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
						(*aniIter)->setFrame(.8f);
					}
					else {}
					time = 0.0f;
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
					glUniform1f(uniformLocation[ANIMATIONX], (*aniIter)->getFrame());
					glUniform1f(uniformLocation[TIME], (*aniIter)->getTime());
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