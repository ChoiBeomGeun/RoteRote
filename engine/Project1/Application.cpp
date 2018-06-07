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
/* GIMP RGBA C-Source image dump (player4_kIe_icon.c) */

/* GIMP RGBA C-Source image dump (player4_kIe_icon.c) */
void WindowIconSetting(SDL_Window * sWnd);
namespace TE {


	Application * APP = nullptr;
}
Application::Application() : pWnd(nullptr), ResolutionNumber(2)
{
	std::string  path;
	std::ifstream ifile;

	char line[200]; // 한 줄씩 읽어서 임시로 저장할 공간

	
#ifdef _DEBUG
	ifile.open(".\\temp.ini");
#else


	char * Userinfo;
	size_t len = path.size();
	_dupenv_s(&Userinfo, &len, "USERPROFILE");

	path = Userinfo;
	path += "/Documents/RoteRote/temp.ini";


	ifile.open(path);  // 파일 열기

	free(Userinfo);

#endif
	
	
	if (ifile.is_open())
	{
		while (ifile.getline(line, sizeof(line))) // 한 줄씩 읽어 처리를 시작한다.
		{
			std::cout << line << std::endl; // 내용 출력

			if (!strcmp(line, "1680 X 1050"))
			{
				_screenWidth = 1680;
				_screenHeight = 1050;
			}
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





	

#ifdef _DEBUG
#else

	if (remove(path.c_str()) != 0)
		perror("Error deleting file");
	else
		puts("File successfully deleted");
	
#endif
;




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
	pWnd = SDL_CreateWindow("RoteRote", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		_screenWidth, _screenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (pWnd == NULL)
	{
		DEBUG_PRINT("SDL Window could not be created!");
		return;
	}

	getCurrentMonitorSize();
	//ImGui_ImplSdlGL3_Init(pWnd);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
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

	glClearColor(0.f, 0.f, 0.f, 0.f);

	

	SDL_SetWindowSize(this->pWnd, _screenWidth, _screenHeight);
	glViewport(0, 0, _screenWidth, _screenHeight);

	SDL_SetWindowPosition(this->pWnd, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_SetWindowBordered(this->pWnd, SDL_TRUE);
	/*if (_resolution == Resolution::_1280X960)
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
*/
	WindowIconSetting(this->pWnd);
	

#ifndef _DEBUG
		SDL_SetWindowFullscreen(pWnd, SDL_WINDOW_FULLSCREEN);
#endif // DEBUG

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

    SDL_GL_SetSwapInterval(1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SwapWindow(pWnd);

}

void TE::Application::ResizeAllObjects()
{
	if (_screenWidth != 1280 && _screenHeight != 720)
	{
		for (auto p : FACTORY->ObjectIDMap)
		{
			if (!p.second->HasComponent<Emitter>())
			{
				float xscale = APP->_screenWidth * p.second->GetComponent<Transform>()->scale.x / 1280;
				float yscale = APP->_screenHeight * p.second->GetComponent<Transform>()->scale.y / 720;

				if (p.second->objectstyle == Objectstyle::Button)
					p.second->GetComponent<Transform>()->scale = glm::vec3(xscale, xscale, 0);
				else
					p.second->GetComponent<Transform>()->scale = glm::vec3(xscale, yscale, 0);
			}
		}
	}
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
	float scale = CAMERA->far_distance()/ CAMERA->cameraPos.z;
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
			confirmationa->GetComponent<Sprite>()->m_TextureID = Sprite::find_texture_id("Sure.png");
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

	if (IsKeyBoardAvailable) {

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

void TE::Application::getCurrentMonitorSize()
{
	glm::vec2 screensize;
	SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
	{
		SDL_Log("SDL_GETDESKTOPDISPLAYMODE failed: %s", SDL_GetError());
		
	}
	screensize.x = dm.w;
	screensize.y = dm.h;
	std::cout << dm.w << "    " << dm.h << std::endl;
	//SDL_GetRendererOutputSize(r)
}

//void Application::ChangeScreenSize(SDL_Window * window, Resolution resolution)
//{
//	_resolution = resolution;
//	if (_isfull)
//	{
//		switch (resolution)
//		{
//
//
//			//case Resolution::_1280x720:
//			//	// 16:9
//			//	SDL_SetWindowSize(window, 1280, 720);
//			//	glViewport(0, 0, 1280, 720);
//			//	ResolutionNumber = resolution;
//			//	std::cout << ResolutionNumber << '\n';
//			//	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
//			//	SDL_SetWindowBordered(window, SDL_TRUE);
//			//	CAMERA->angle = 45.f;
//			//	break;
//			//case Resolution::_1280x960:
//			//	//4:3
//			//	SDL_SetWindowSize(window, 1280, 960);
//			//	glViewport(0, 0, 1280, 960);
//			//	ResolutionNumber = resolution;
//			//	std::cout << ResolutionNumber << '\n';
//			//	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
//			//	SDL_SetWindowBordered(window, SDL_TRUE);
//			//	CAMERA->angle = 45.f;
//			//	break;
//
//			//case Resolution::_1920x1080:
//			//	SDL_SetWindowSize(window, 1920, 1080);
//			//	glViewport(0, 0, 1920, 1080);
//			//	ResolutionNumber = resolution;
//			//	std::cout << ResolutionNumber << '\n';
//			//	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
//			//	SDL_SetWindowBordered(window, SDL_TRUE);
//			//	CAMERA->angle = 45.f;
//			//	break;
//			//default:
//			//	break;
//
//		}
//	}
//	else
//	{
//		switch (resolution)
//		{
//			//case Resolution::_960x720:
//			//	// 0 16:9
//			//	SDL_SetWindowSize(window, 960, 720);
//			//	glViewport(0, 0, 960, 720);
//			//	ResolutionNumber = resolution;
//			//	std::cout << ResolutionNumber << '\n';
//			//	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
//			//	CAMERA->angle = 45.f;
//			//	break;
//
//			//case Resolution::_1280x720:
//			//	// 16:9
//			//	SDL_SetWindowSize(window, 1280, 720);
//			//	glViewport(0, 0, 1280, 720);
//			//	ResolutionNumber = resolution;
//			//	std::cout << ResolutionNumber << '\n';
//			//	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
//			//	CAMERA->angle = 45.f;
//			//	break;
//			//case Resolution::_1280x960:
//			//	//4:3
//			//	SDL_SetWindowSize(window, 1280, 960);
//			//	glViewport(0, 0, 1280, 960);
//			//	ResolutionNumber = resolution;
//			//	std::cout << ResolutionNumber << '\n';
//			//	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
//			//	CAMERA->angle = 45.f;
//			//	break;
//
//			//case Resolution::_1920x1080:
//			//	SDL_SetWindowSize(window, 1920, 1080);
//			//	glViewport(0, 0, 1920, 1080);
//			//	ResolutionNumber = resolution;
//			//	std::cout << ResolutionNumber << '\n';
//			//	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
//			//	CAMERA->angle = 45.f;
//			//	break;
//			//default:
//			//	break;
//
//		}
//	}
//}

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

void WindowIconSetting(SDL_Window * sWnd)
{
	static const struct {
		unsigned int 	 width;
		unsigned int 	 height;
		unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */
		unsigned char	 pixel_data[16 * 16 * 4 + 1];
	} my_icon = {
		16, 16, 4,
		"\234\234\234\377kkk\377ooo\377ooo\377ooo\377ooo\377ooo\377ooo\377ooo\377"
		"ooo\377ooo\377ooo\377ooo\377ooo\377ooo\377\266\266\266\377RRR\377\000\000\000\377"
		"\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000"
		"\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\201\201\201\377"
		"RRR\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000"
		"\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377"
		"\201\201\201\377RRR\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377"
		"\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000"
		"\000\000\377\000\000\000\377\201\201\201\377RRR\377\000\000\000\377\000\000\000\377\000\000\000\377\000"
		"\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000"
		"\000\377\000\000\000\377\000\000\000\377\000\000\000\377\201\201\201\377RRR\377\000\000\000\377\000\000"
		"\000\377\010\010\010\377\060\060\060\377EEE\377\040\040\040\377\002\002\002\377\000\000\000\377"
		"\000\000\000\377\000\000\000\377\023\023\023\377\063\063\063\377$$$\377\005\005\005\377\201\201\201"
		"\377RRR\377\000\000\000\377\003\003\003\377PPP\377\311\311\311\377\351\351\351\377\253"
		"\253\253\377)))\377\012\012\012\377\014\014\014\377\033\033\033\377\214\214\215\377"
		"\331\331\331\377\304\304\304\377LLL\377\204\204\204\377RRR\377\000\000\000\377"
		"\024\024\024\377\250\250\250\377\377\377\377\377\275\275\275\377\272\272\272"
		"\377lll\377\067\067\067\377???\377UUU\377\356\356\356\377\374\374\374\377\271"
		"\271\271\377\225\225\225\377\234\234\234\377RRR\377\000\000\000\377\027\027\027\377"
		"\260\260\260\377\377\377\377\377\243\243\243\377\234\234\234\377ttt\377<"
		"<<\377FFF\377ccc\377\367\367\367\377\367\367\367\377vvv\377\207\207\207\377"
		"\245\245\245\377RRR\377\000\000\000\377\006\006\006\377eee\377\341\341\341\377\367\367"
		"\367\377\301\301\301\377\065\065\065\377\027\027\027\377\035\035\035\377\063\064\064"
		"\377\303\303\303\377\375\375\375\377\350\350\350\377uuu\377\213\213\213\377"
		"RRR\377\000\000\000\377\000\000\000\377\020\020\020\377KKK\377fff\377\064\065\065\377\004\004\004"
		"\377\012\012\012\377*''\377,%%\377\070\070\070\377ttt\377\\]]\377\026\026\026\377"
		"\201\201\201\377RRR\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\001\001\001\377"
		"\000\000\000\377\002\002\002\377!##\377c,,\377d\061\061\377\000\000\000\377\004\004\004\377\002\002\002\377"
		"\000\000\000\377\201\201\201\377RRR\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377"
		"\000\000\000\377\000\000\000\377\010\010\010\377\034\034\034\377\232\066\066\377v\067\067\377\000"
		"\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\201\201\201\377RRR\377\000\000\000\377\000"
		"\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\004\004\004\377\040\"\"\377t,,"
		"\377n\064\064\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\201\201\201\377S"
		"SS\377\000\000\000\377\001\001\001\377\001\001\001\377\001\001\001\377\001\001\001\377\001\001\001\377\001\001\001"
		"\377\022\023\023\377\065,,\377\066''\377\000\001\001\377\001\001\001\377\001\001\001\377\002\002\002\377"
		"\202\202\202\377\255\255\255\377\207\207\207\377\210\210\210\377\210\210"
		"\210\377\210\210\210\377\210\210\210\377\210\210\210\377\210\210\210\377"
		"\207\207\207\377\216\217\217\377\215\216\216\377\210\210\210\377\210\210"
		"\210\377\210\210\210\377\210\210\210\377\302\302\302\377",
	};


	Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	int shift = (my_icon.bytes_per_pixel == 3) ? 8 : 0;
	rmask = 0xff000000 >> shift;
	gmask = 0x00ff0000 >> shift;
	bmask = 0x0000ff00 >> shift;
	amask = 0x000000ff >> shift;
#else // little endian, like x86
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = (my_icon.bytes_per_pixel == 3) ? 0 : 0xff000000;
#endif
	SDL_Surface* icon = SDL_CreateRGBSurfaceFrom((void*)my_icon.pixel_data, my_icon.width,
		my_icon.height, my_icon.bytes_per_pixel * 8, my_icon.bytes_per_pixel*my_icon.width,
		rmask, gmask, bmask, amask);

	//SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(pixels, 16, 16, 16, 16 * 2, 0x0f00, 0x00f0, 0x000f, 0xf000);

	// The icon is attached to the window pointer
	SDL_SetWindowIcon(sWnd, icon);
	// ...and the surface containing the icon pixel data is no longer required.
	SDL_FreeSurface(icon);
}