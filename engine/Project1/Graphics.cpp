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
bool moving;

std::vector<std::string> namelist;
namespace TE {
	Graphics * GRAPHICS = nullptr;
}
using namespace TE;



Graphics::Graphics()
{

	DEBUG_ASSERT(GRAPHICS != nullptr, "Graphics are no more 2");
	GRAPHICS = this;
}
void TE::Graphics::initShader()
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


	_colorProgram.compileShaders(path + ".vert", path + ".frag");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");

	_colorProgram.linkShaders();
}


void Graphics::Initialize(void)
{
	moving = true;
	pCamera = new Camera;
	CAMERA = pCamera;
	isHud = 0;

	isMapEditorOn = false;
	drawGrid = false;
	//loading OPENGL Funciton
	DEBUG_ASSERT(glewInit() != GLEW_OK, "Initializing GLEW is failed\n");

	// initialize Shader
	initShader();

	if (buffer == 0)
		glGenBuffers(1, &buffer);

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

	// unbind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0); // 0 mean unbind

	drawAttributes();
	CAMERA->cameraPos = eye;
	CAMERA->cameraTarget = target;
	CAMERA->cameraUp = up;

	//_spriteBatch.init();
	uniformLocation[TEXTURE] = _colorProgram.getUniformLocation("texturing");
	uniformLocation[SAMPLER] = _colorProgram.getUniformLocation("texture2D");
	uniformLocation[FLIPX] = _colorProgram.getUniformLocation("flipx");
	uniformLocation[ISJUMPING] = _colorProgram.getUniformLocation("isJumping");
	//uniformLocation[FLIPOBJ] = _colorProgram.getUniformLocation("flipObj");
	uniformLocation[ISANIMATION] = _colorProgram.getUniformLocation("isAnimation");
	uniformLocation[ANIMATIONX] = _colorProgram.getUniformLocation("animationx");
	uniformLocation[TIME] = _colorProgram.getUniformLocation("timer");
	uniformLocation[COLOR] = _colorProgram.getUniformLocation("colorOffset");
	uniformLocation[MODEL] = _colorProgram.getUniformLocation("model");
	uniformLocation[VIEW] = _colorProgram.getUniformLocation("view");
	uniformLocation[PROJ] = _colorProgram.getUniformLocation("projection");
	uniformLocation[ISHUD] = _colorProgram.getUniformLocation("isHud");
	uniformLocation[HUDMODEL] = _colorProgram.getUniformLocation("hudmodel");

#ifdef _DEBUG
	ImGui_ImplSdlGL3_Init(APP->pWnd);
	ImVec4 clear_color = ImColor(114, 144, 154);
#endif
	time = 0.f;
	splashtime = 0.f;
	//   ImGuiIO& io = ImGui::GetIO();
}


void TE::Graphics::drawAttributes()
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
	glDrawArrays(GL_TRIANGLES, 0, 6); // drawing 6 vertices 

									  // disable the vertex attrib array. this is not optional
	glDisableVertexAttribArray(0);

	// unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0); // 0 mean unbind
}

// ALL systems are updated every frame
void Graphics::Update(float dt)
{

	_colorProgram.use();
	Sprite::sortSprites(SortType::FRONT_TO_BACK);
	glm::mat4 view;
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.f, 1.f, 1.f, .5f);

	time += dt;
	splashtime += dt;

	glUniform1i(uniformLocation[TEXTURE], 1/*True*/);
	glUniform1i(uniformLocation[SAMPLER], 0);

	glm::mat4 model;
	glm::mat4 hudmodel = glm::mat4(1.f);


	if (CAMERA->isCentered)
		CAMERA->lookat(CAMERA->cameraPos, CAMERA->cameraTarget, CAMERA->cameraUp);
	else
	{
		if (FACTORY->GetPlayer())
		{
			CAMERA->cameraPos = FACTORY->GetPlayer()->transform->position;
			CAMERA->cameraPos.z = 999.f;
			CAMERA->lookat(CAMERA->cameraPos, CAMERA->cameraTarget, CAMERA->cameraUp);
		}
	}
	view = CAMERA->view;

	glUniformMatrix4fv(uniformLocation[VIEW], 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(uniformLocation[PROJ], 1, GL_FALSE, &CAMERA->projection[0][0]);


	for (std::vector<Sprite*>::iterator it = SpriteList.begin();
		it != SpriteList.end(); ++it) {
		glBindTexture(GL_TEXTURE_2D, (*it)->TextureId);

		auto aniIter = AnimationList.begin();
		for (aniIter; aniIter != AnimationList.end(); ++aniIter)
		{
			if ((*aniIter)->pOwner->GetComponent(CT_ANIMATION) != nullptr) {
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
						else
						{

						}
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
		if ((*it)->isPerspective)
		{
			if ((*it)->pOwner->GetComponent(CT_ANIMATION) != nullptr) {
				glUniform1i(uniformLocation[ISANIMATION], 1);
			}
			isHud = 0;
			CAMERA->proj();
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3((*it)->pTransform->position.x, (*it)->pTransform->position.y, (*it)->pTransform->position.z));
			model = glm::scale(model, glm::vec3((*it)->pTransform->scale.x, (*it)->pTransform->scale.y, (*it)->pTransform->scale.z));
			model = glm::rotate(model, glm::radians((*it)->pTransform->angle), (*it)->pTransform->rotation);
		}
		else
		{
			isHud = 1;
			hudmodel = glm::mat4(1.0f);
			hudmodel = glm::translate(hudmodel, (*it)->pTransform->position);
			hudmodel = glm::scale(hudmodel, (*it)->pTransform->scale);
		}

		glUniformMatrix4fv(uniformLocation[HUDMODEL], 1, GL_FALSE, &hudmodel[0][0]);
		glUniform1i(uniformLocation[ISHUD], isHud);
		glUniform4f(uniformLocation[COLOR], (*it)->Color[0], (*it)->Color[1], (*it)->Color[2], (*it)->Color[3]);
		glUniformMatrix4fv(uniformLocation[MODEL], 1, GL_FALSE, &model[0][0]);

		glPushAttrib(GL_CURRENT_BIT);
		if (isMapEditorOn)
		{
			if (drawGrid)
			{
				glDrawArrays(GL_LINE, 0, 2);
				//DEBUG_PRINT("lines\n");
			}

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		if (!isMapEditorOn)
		{
			glDrawArrays(GL_TRIANGLES, 0, 6);
			//DEBUG_PRINT("normal\n");
		}
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
	_colorProgram.unuse();
	delete pCamera;
	pCamera = NULL;



#ifdef _DEBUG
	ImguiFree();
	ImGui_ImplSdlGL3_Shutdown();
#endif
}