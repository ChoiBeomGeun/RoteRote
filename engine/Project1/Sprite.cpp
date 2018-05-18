/**
\file        Sprite.cpp
\author      HyunJun Yoo
\par         email: hyunjun306@gmail.com
\par         course: CS225
\date        12/16/2017
\brief

This is where textures are loaded, sort sprite by depth.
and store sprite info to sprite vector

All content 2017 DigiPen (USA) Corporation, all rights reserved.
*/


#include "Sprite.h"
#include "Graphics.h"
#include <algorithm>
#include "DebugUtil.h"
#include "SOIL.h"
#include "Object.h"
#include <string>
#include <algorithm>
#define JSON_FILE "jsonFile.json"
#define JSON_FILE_WRITE "jsonFileWrite.json"
using namespace TE;


Sprite::Sprite() : Component(ComponentType::CT_SPRITE)
{

	GRAPHICS->SpriteList.push_back(this);
	//m_BaseGraphicsList.push_back(this);
}

Sprite::~Sprite()
{
	GRAPHICS->SpriteList.erase(std::find(GRAPHICS->SpriteList.begin(), GRAPHICS->SpriteList.end(), this));
	

}

void Sprite::Initialize()
{
	_sortType = SortType::FRONT_TO_BACK;
	isRotating = false;
}


GLuint  Sprite::texture_load(std::string filepath)
{
	char * Userinfo;
	size_t len = filepath.size();
	_dupenv_s(&Userinfo, &len, "USERPROFILE");

	std::string saveLevel = filepath;
#ifdef _DEBUG
	filepath = ".\\texture.\\" + filepath;
#else
	filepath = Userinfo;
	filepath += "/Documents/RoteRote/texture/" + saveLevel;
#endif
	free(Userinfo);
	mTexutureDir = saveLevel;

	m_TextureID = SOIL_load_OGL_texture(filepath.c_str(), SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MULTIPLY_ALPHA | SOIL_FLAG_INVERT_Y);

	glActiveTexture(GL_TEXTURE0);
	/*glBindTexture(GL_TEXTURE_2D, m_TextureID);*/

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_REPEAT);

	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	return m_TextureID;
}

GLuint Sprite::find_texture_id(std::string filepath)
{
	std::string temp = ".\\texture.\\";
	temp += filepath;
	for(auto texID : GRAPHICS->m_textureMap)
	{
		if ((texID.first == temp))
			return texID.second;
	}
	return false;
}

void TE::Sprite::SetTextureID(std::string filepath)
{
	
}

void Sprite::LoadAllSprites()
{
	if (GRAPHICS->m_textureMap.empty())
	{
		for (auto texID : ENGINE->mVsTexturenamelist)
		{
			std::string textureDir = texID;
			textureDir = std::string(".\\texture.\\") + textureDir;
			GLuint tempTextureID = SOIL_load_OGL_texture(textureDir.c_str(), SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MULTIPLY_ALPHA | SOIL_FLAG_INVERT_Y);
			glActiveTexture(GL_TEXTURE0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_REPEAT);
			GRAPHICS->m_textureMap[textureDir] = tempTextureID;
		}
	}
}

void Sprite::UnLoadAllSprites()
{
	for (auto texID : GRAPHICS->m_textureMap)
	{
		//std::cout << "texID->pTextureID : " << texID->pTexureID << '\n';
		glDeleteTextures(1, &texID.second);
	}
}

void Sprite::ChangeColor(float r, float g, float b, float a)
{
	if ((r < 0 || r> 255.0f) || (g < 0 || g> 255.0f) || (b < 0 || b> 255.0f) || (a < 0 || a> 255.0f))
		DEBUG_PRINT("number is out of bound\n");


	Color[0] = r / 255.f;
	Color[1] = g / 255.f;
	Color[2] = b / 255.f;
	Color[3] = a / 255.f;
}

void Sprite::ChangeBackgroundColor(float r, float g, float b, float a)
{
	glClearColor(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

void Sprite::framebuffer_size_callback(int width, int height)
{
	glViewport(0, 0, width, height);
}

void TE::Sprite::sortSprites(SortType sortType /*= SortType::FRONT_TO_BACK*/)
{
	switch (sortType)
	{
	case SortType::BACK_TO_FRONT:
		std::stable_sort(GRAPHICS->SpriteList.begin(), GRAPHICS->SpriteList.end(), compareBackToFront);
		break;
	case SortType::FRONT_TO_BACK:
		std::stable_sort(GRAPHICS->SpriteList.begin(), GRAPHICS->SpriteList.end(), compareFrontToBack);
		break;
	case SortType::TEXTURE:
		std::stable_sort(GRAPHICS->SpriteList.begin(), GRAPHICS->SpriteList.end(), compareTextureID);
		break;


	}

}

bool TE::Sprite::compareBackToFront(Sprite *a, Sprite* b)
{
	return (a->depth > b->depth);
}

bool TE::Sprite::compareFrontToBack(Sprite * a, Sprite * b)
{
	return (a->depth < b->depth);
}

bool TE::Sprite::compareTextureID(Sprite * a, Sprite * b)
{
	return (a->m_TextureID< b->m_TextureID);
}







