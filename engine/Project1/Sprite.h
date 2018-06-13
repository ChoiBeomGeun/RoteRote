/******************************************************************************/
/*!
\file		Sprite.h
\author		HyunJun Yoo
\par		email: hyunjun306@gmail.com
\par		Class:GAM250
\par		ThumbUp Engine
\date		06/13/2018

Sprite component,
where sprite is defined.
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "Component.h"
#include "Transform.h"
#include  <string>
#include "glew.h"
#include "SDL.h"


enum class SortType
{
	NONE,
	FRONT_TO_BACK,
	BACK_TO_FRONT,
	TEXTURE
};


namespace TE {
	class Sprite : public Component
	{
	public:
		Sprite();
		~Sprite();
		virtual void Initialize() override;
		GLuint  texture_load(std::string filepath);
		GLuint m_TextureID;
		std::string mTexutureDir;
		static void LoadAllSprites();
		static void UnLoadAllSprites();
		static GLuint find_texture_id(std::string filepath);
		static void SetTextureID(std::string filepath);
		void ChangeAvalue(float a);
		void ChangeColor(float r, float g, float b, float a); // You can actually put numbers from 0~255.f 
		void ChangeBackgroundColor(float r, float g, float b, float a);


		void framebuffer_size_callback(int width, int height);
		bool isPerspective;
		bool isRotating;
		static void sortSprites(SortType sortType = SortType::FRONT_TO_BACK);
		static bool compareBackToFront(Sprite *a, Sprite* b);
		static bool compareFrontToBack(Sprite *a, Sprite* b);
		static bool compareTextureID(Sprite *a, Sprite* b);
		float depth;
		float Color[4]; ;
	private:
		SortType _sortType;

	};
}