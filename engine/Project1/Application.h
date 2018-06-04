/******************************************************************************/
/*!
\file   Application.h
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM200
\par    ThumbUp Engine
\date   2017/11/29

Application header file for handling Resolution and window
All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once


#include <SDL.h>
#include "System.h"
//#include "vector3.h"
#include <vector>
#include <glm/glm.hpp>


namespace TE {
	enum Resolution
	{


		_800X600 = 1,
		_1024X768,
		_1280X720,
		_1280X960,
		_1680X1050,
		_1600X900,
		_1920X1080
	};

	class Application : public Systems
	{
		friend class Object;
	public:
		Application();
		~Application();

		void Initialize(void) override;
		void Update(float dt) override;
		void SwapWindow(void);
		void ResizeAllObjects();
		//Todo : add other features
		//Resolution size
		//Window Size
		//Get and Set Window Size
		//FullScreen
		int _screenWidth;			// 
		int _screenHeight;
		const Uint8 *keyboard_state_array;
		// 
		SDL_Event event;	// Events from ths OS
		SDL_Window * pWnd; // window data 
		SDL_GLContext glContext;
		glm::vec3  mousePos;

		glm::vec3  ConvertToNDC(const glm::vec3 & position);
		glm::vec3 ConvertToWorld(glm::vec3 mouseCoords);
		void SetMousePosition(SDL_Event & currEvent);
		void PollEvents(void);
		void PollWindowEvent(SDL_Event & currEvent);
		void PollKeyEvent(SDL_Event & currEvent);
		void PollMouseEvent(SDL_Event & currentEvent);
		void getCurrentMonitorSize();
		/*
		*	return windowID: pWnd;
		*
		*/

		Resolution _resolution;

		SDL_Window* getWindow() { return pWnd; }
		SDL_Cursor *cursor;
		/*
		*		Changes to fixed resolution size
		*		To do: select screen size. and give options
		*		960x720 = 1,
		*		_1024x576,
		*		_1280x720,
		*		_1280x960,
		*		_1440x1080, // 4:3
		*		_1920x1080
		*
		*/
		//void ChangeScreenSize(SDL_Window * window, Resolution resolution);
		void toggle_fullscreen(SDL_Window * window, bool isfull);
		bool b_Win;
		bool b_Lose;
		bool _isfull;
		bool windowminimized = false;
		bool windowIsActive = false;
		bool IsKeyBoardAvailable = true;
		unsigned ResolutionNumber = 0;
	};
	extern Application * APP;


}//namespace TEend