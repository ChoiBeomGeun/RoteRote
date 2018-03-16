/******************************************************************************/
/*!
\file   Application.cpp
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

Application Source file for handling Resolution and window
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma  once
#include <algorithm>
#include "Application.h"
#include "DebugUtil.h"
#include <stdio.h>
#include "Engine.h"
#include "Input.h"
#include "Graphics.h"
#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <fstream>
void copyFile(std::string  src, std::string dest);

using namespace TE;
bool RealExit = false;
Object * confirmationa;

namespace TE {


	Application * APP = nullptr;
}
Application::Application() : pWnd(nullptr), ResolutionNumber(2)
{
	std::ifstream ifile;

	char line[200]; // 한 줄씩 읽어서 임시로 저장할 공간

	ifile.open(".\\temp.txt");  // 파일 열기

	if (ifile.is_open())
	{
		while (ifile.getline(line, sizeof(line))) // 한 줄씩 읽어 처리를 시작한다.
		{
			std::cout << line << std::endl; // 내용 출력

			if (!strcmp(line, "1920 X 1080"))
			{
				_screenHeight = 1080;
				_screenWidth = 1920;

				_resolution = Resolution::_1920X1080;
			}
			if (!strcmp(line, "1600 X 900"))
			{
				_screenHeight = 900;
				_screenWidth = 1600;
				_resolution = Resolution::_1600X900;
			}
			if (!strcmp(line, "1280 X 960"))
			{
				_screenHeight = 960;
				_screenWidth = 1280;
				_resolution = Resolution::_1280X960;
			}
			if (!strcmp(line, "1024 X 768"))
			{
				_screenHeight = 768;
				_screenWidth = 1024;
				_resolution = Resolution::_1024X768;
			}
			if (!strcmp(line, "800 X 600"))
			{
				_screenHeight = 600;
				_screenWidth = 800;
				_resolution = Resolution::_800X600;
			}
			if (!strcmp(line, "1280 X 720"))
			{
				_screenWidth = 1280;
				_screenHeight = 720;
				_resolution = Resolution::_1280X720;
			}
			if (!strcmp(line, "True"))
				_isfull = true;
		}
	}

	ifile.close(); // 파일 닫기



	DEBUG_ASSERT(APP != nullptr, "No more than 1 instnace of Application system! Stupid!");
	APP = this;

}

Application::~Application()
{
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(pWnd);
	pWnd = nullptr;
	SDL_FreeCursor(cursor);
	SDL_Quit();


}

void Application::Initialize()
{

	//_resolution = Resolution::_1280x720;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		DEBUG_PRINT("SDL couldn't init! SD_Error : %s\n", SDL_GetError());
	}
	// Set up the screen
	pWnd = SDL_CreateWindow("game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		_screenWidth, _screenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (pWnd == NULL)
	{
		DEBUG_PRINT("SDL Window could not be created!");
		return;
	}


	//ImGui_ImplSdlGL3_Init(pWnd);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	//we are not going to use deprecated functions
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//
	glContext = SDL_GL_CreateContext(pWnd);


	//Check SDL_WINDOW_OPENGL or
	//Graphcis Driver support the minore GL version
	//Force GPU to use .exe file not onboard graphic ! 
	DEBUG_ASSERT(glContext == 0, "Createing openGL contextFailed!!!");
	if (!pWnd)
	{
		DEBUG_ASSERT(pWnd == nullptr, "Failed to create window\n");
	}
	else
		DEBUG_PRINT("Window is create\n");

	// check the OpenGL version
	std::printf(" *** OpenGL Version %s ****\n", glGetString(GL_VERSION));

	glClearColor(1.f, 1.f, 1.f, 1.f);

	glEnable(GL_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);

	if (_resolution == Resolution::_1280X960)
	{


		SDL_SetWindowSize(this->pWnd, 1280, 960);
		glViewport(0, 0, 1280, 960);

		std::cout << ResolutionNumber << '\n';
		SDL_SetWindowPosition(this->pWnd, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		SDL_SetWindowBordered(this->pWnd, SDL_TRUE);

	}
	if (_resolution == Resolution::_1920X1080)
	{


		SDL_SetWindowSize(this->pWnd, 1920, 1080);
		glViewport(0, 0, 1920, 1080);

		std::cout << ResolutionNumber << '\n';
		SDL_SetWindowPosition(this->pWnd, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		SDL_SetWindowBordered(this->pWnd, SDL_TRUE);

	}
	if (_resolution == Resolution::_800X600)
	{


		SDL_SetWindowSize(this->pWnd, 800, 600);
		glViewport(0, 0, 800, 600);

		std::cout << ResolutionNumber << '\n';
		SDL_SetWindowPosition(this->pWnd, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		SDL_SetWindowBordered(this->pWnd, SDL_TRUE);

	}
	if (_resolution == Resolution::_1280X720)
	{
		SDL_SetWindowSize(this->pWnd, 1280, 720);
		glViewport(0, 0, 1280, 720);
		std::cout << ResolutionNumber << '\n';
		SDL_SetWindowPosition(this->pWnd, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		SDL_SetWindowBordered(this->pWnd, SDL_TRUE);

	}

	if (_isfull)
		SDL_SetWindowFullscreen(pWnd, SDL_WINDOW_FULLSCREEN);


	SDL_GL_SetSwapInterval(1);
#ifndef _DEBUG
	int32_t cursorData[2] = { 0, 0 };
	cursor = SDL_CreateCursor((Uint8 *)cursorData, (Uint8 *)cursorData, 8, 8, 4, 4);
	SDL_SetCursor(cursor);
#endif
}

void Application::Update(float /*dt*/)
{
	Input::Reset();
	//Todo : PollEvents 

	PollEvents();

}

void Application::SwapWindow(void)
{

	SDL_GL_SwapWindow(pWnd);

}

glm::vec3  Application::ConvertToNDC(const glm::vec3 & position)
{
	glm::vec3  vec3;
	float viewW = APP->_screenWidth*0.5f;
	float viewH = APP->_screenHeight*0.5f;
	float worldW = APP->_screenWidth *0.5f;
	float worldH = APP->_screenHeight *0.5f;
	vec3.x = ((position.x - viewW)*worldW) / viewW;
	vec3.y = (position.y - viewH)*(-worldH) / viewH;
	return vec3;
}

glm::vec3 TE::Application::ConvertToWorld(glm::vec3 mouseCoords)
{
	// invert y direction
	mouseCoords.x = APP->_screenWidth *.5f - mouseCoords.x;
	mouseCoords.y = mouseCoords.y - APP->_screenHeight *.5f;
	float scale = CAMERA->_zFar / CAMERA->cameraPos.z;
	mouseCoords /= scale;
	mouseCoords.z = 0;

	glm::mat4 model;
	model = glm::translate(model, mouseCoords);
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.f, 0.f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	mouseCoords = glm::unProject(mouseCoords, model * CAMERA->view, CAMERA->projection, glm::vec4(0.0f, 0.0f, (float)APP->_screenWidth, (float)APP->_screenHeight));
	mouseCoords.z = 130.f;
	return mouseCoords;
}

void Application::PollEvents(void)
{

	while (SDL_PollEvent(&event))
	{
		if (event.window.event == SDL_WINDOWEVENT_CLOSE)
		{
			SDL_RestoreWindow(getWindow());
		}
		if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
		{
			//windowIsActive = true;
		}

		if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {

			windowIsActive = false;
		}
		PollWindowEvent(event);
		PollKeyEvent(event);
		PollMouseEvent(event);
		ImGui_ImplSdlGL3_ProcessEvent(&event);
	}
}

void Application::PollWindowEvent(SDL_Event & currEvent)
{

	std::string name;
	//std::str
	std::string filename;
	std::string filetype;
	std::size_t found;
	std::size_t found2;
	std::string desname;


	switch (currEvent.type)
	{
	case SDL_QUIT:
		if (confirmationa == NULL) {
			confirmationa = FACTORY->CreateHUD(glm::vec3(0, 0, 0), glm::vec3(1.5, 0.7, 0));
			confirmationa->GetComponent<Sprite>()->texture_load("Sure.png");
		}
		RealExit = true;

		//ENGINE->Quit();
		break;
	case SDL_DROPFILE:
		name = currEvent.drop.file;
		found = name.find_last_of("/\\");
		filename = name.substr(found + 1);
		found2 = filename.find_last_of(".");
		filetype = filename.substr(found2 + 1);

		if (filetype == "png") {
			TCHAR buffer[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, buffer);

			desname = buffer;
			desname += "\\texture\\" + filename;
			CopyFile(name.c_str(), desname.c_str(), TRUE);
			ENGINE->mVsTexturenamelist.push_back(filename);

			//	copyFile(name, desname);
		}
		else if (filetype == "mp3") {
			TCHAR buffer[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, buffer);
			free(buffer);
			desname = buffer;
			desname += "\\sounds\\" + filename;
			CopyFile(name.c_str(), desname.c_str(), TRUE);
			ENGINE->mVsSoundnamelist.push_back(filename);
			//	copyFile(name, desname);
		}
		else {
			DEBUG_ASSERT(true, "ThumbUp Engine only supports png,mp3 files");

		}
		break;
	default:
		break;
	}



}

void Application::PollKeyEvent(SDL_Event & currEvent)
{
	if (RealExit) {

		if (Input::IsTriggered(SDL_SCANCODE_Y)) {
			FACTORY->Destroy(confirmationa);
			FACTORY->Update(0);
			ENGINE->Quit();
		}
		if (Input::IsTriggered(SDL_SCANCODE_N)) {
			RealExit = false;
			FACTORY->Destroy(confirmationa);
			FACTORY->Update(0);

		}
	}

	switch (currEvent.type)
	{
	case SDL_KEYDOWN:
		Input::SetKeyPressed(currEvent.key.keysym.scancode, SDL_KEYDOWN);
		break;
	case SDL_KEYUP:
		Input::SetKeyPressed(currEvent.key.keysym.scancode, SDL_KEYUP);
		break;
	default:
		break;
	}

}
/**
* \brief
*/
void TE::Application::PollMouseEvent(SDL_Event & currentEvent)
{
	switch (currentEvent.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		Input::SetMousePressed(currentEvent.button.button, SDL_MOUSEBUTTONDOWN);
		break;
	case SDL_MOUSEBUTTONUP:
		Input::SetMousePressed(currentEvent.button.button, SDL_MOUSEBUTTONUP);
		break;
	default:
		break;
	}
}

void Application::ChangeScreenSize(SDL_Window * window, Resolution resolution)
{
	_resolution = resolution;
	if (_isfull)
	{
		switch (resolution)
		{


			//case Resolution::_1280x720:
			//	// 16:9
			//	SDL_SetWindowSize(window, 1280, 720);
			//	glViewport(0, 0, 1280, 720);
			//	ResolutionNumber = resolution;
			//	std::cout << ResolutionNumber << '\n';
			//	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
			//	SDL_SetWindowBordered(window, SDL_TRUE);
			//	CAMERA->angle = 45.f;
			//	break;
			//case Resolution::_1280x960:
			//	//4:3
			//	SDL_SetWindowSize(window, 1280, 960);
			//	glViewport(0, 0, 1280, 960);
			//	ResolutionNumber = resolution;
			//	std::cout << ResolutionNumber << '\n';
			//	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
			//	SDL_SetWindowBordered(window, SDL_TRUE);
			//	CAMERA->angle = 45.f;
			//	break;

			//case Resolution::_1920x1080:
			//	SDL_SetWindowSize(window, 1920, 1080);
			//	glViewport(0, 0, 1920, 1080);
			//	ResolutionNumber = resolution;
			//	std::cout << ResolutionNumber << '\n';
			//	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
			//	SDL_SetWindowBordered(window, SDL_TRUE);
			//	CAMERA->angle = 45.f;
			//	break;
			//default:
			//	break;

		}
	}
	else
	{
		switch (resolution)
		{
			//case Resolution::_960x720:
			//	// 0 16:9
			//	SDL_SetWindowSize(window, 960, 720);
			//	glViewport(0, 0, 960, 720);
			//	ResolutionNumber = resolution;
			//	std::cout << ResolutionNumber << '\n';
			//	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
			//	CAMERA->angle = 45.f;
			//	break;

			//case Resolution::_1280x720:
			//	// 16:9
			//	SDL_SetWindowSize(window, 1280, 720);
			//	glViewport(0, 0, 1280, 720);
			//	ResolutionNumber = resolution;
			//	std::cout << ResolutionNumber << '\n';
			//	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
			//	CAMERA->angle = 45.f;
			//	break;
			//case Resolution::_1280x960:
			//	//4:3
			//	SDL_SetWindowSize(window, 1280, 960);
			//	glViewport(0, 0, 1280, 960);
			//	ResolutionNumber = resolution;
			//	std::cout << ResolutionNumber << '\n';
			//	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
			//	CAMERA->angle = 45.f;
			//	break;

			//case Resolution::_1920x1080:
			//	SDL_SetWindowSize(window, 1920, 1080);
			//	glViewport(0, 0, 1920, 1080);
			//	ResolutionNumber = resolution;
			//	std::cout << ResolutionNumber << '\n';
			//	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
			//	CAMERA->angle = 45.f;
			//	break;
			//default:
			//	break;

		}
	}
}

void Application::toggle_fullscreen(SDL_Window * window, bool isfull)
{
	// if the screen is windowed
	_isfull = isfull;
	if (isfull)
	{
		// set the screen to fullscreen
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
		//glViewport(0, 0, APP->_screenWidth, APP->_screenHeight);
		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}
	else
	{
		SDL_SetWindowFullscreen(window, SDL_WINDOW_MINIMIZED);
		//glViewport(0, 0, APP->_screenWidth, APP->_screenHeight);
		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}

}
void copyFile(std::string src, std::string dest)
{
	FILE* f_src;
	FILE* f_dest;
	f_src = fopen(src.c_str(), "r");
	f_dest = fopen(dest.c_str(), "w");
	int tmp;
	while (!feof(f_src))
	{
		tmp = fgetc(f_src);
		fputc(tmp, f_dest);
	}
	fclose(f_src);
	fclose(f_dest);
}