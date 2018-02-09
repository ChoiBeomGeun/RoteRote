
/**
\file        Sprite.h
\author      HyunJun Yoo
\par         email: hyunjun306@gmail.com
\par         course: CS225
\date        12/16/2017
\brief

Sprite component,
where sprite is defined.


All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once

#include "Component.h"
#include "GameLogic.h"
//#include "vector3.h"
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

		GLuint  texture_load( std::string filepath);
		
		
		Transform * pTransform;
		GLuint pTexureID;
		std::string mTexutureDir;
		void ChangeColor(float r, float g, float b, float a); // You can actually put numbers from 0~255.f 
		void ChangeBackgroundColor(float r, float g, float b, float a);
		void framebuffer_size_callback( int width, int height);
		bool isPerspective;
		float depth;
		static void sortSprites(SortType sortType = SortType::FRONT_TO_BACK);
		static bool compareBackToFront(Sprite *a, Sprite* b);
		static bool compareFrontToBack(Sprite *a, Sprite* b);
		static bool compareTextureID(Sprite *a, Sprite* b);
		
		float Color[4]={255,255,255,255};
		GLint TextureId;
	private:
		SortType _sortType;

	};
}