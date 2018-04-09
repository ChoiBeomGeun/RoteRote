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
	//SOIL_free_image_data();

	GRAPHICS->SpriteList.erase(std::find(GRAPHICS->SpriteList.begin(), GRAPHICS->SpriteList.end(), this));
	for (auto texID : GRAPHICS->SpriteList)
	{
		//std::cout << "texID->pTextureID : " << texID->pTexureID << '\n';
		glDeleteTextures(1, &texID->m_TextureID);
	}
	/*for (auto map: m_textureMap)
	{
	m_textureMap.clear();
	}*/
}

void Sprite::Initialize()
{

	/*pTransform =this->GetOwner()->GetComponent<Transform>();
	if (pTransform == nullptr)
	{
	DEBUG_PRINT("Sprite Component Init Fail");

	}*/
	//_isPressed = false;
	_sortType = SortType::FRONT_TO_BACK;
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

	/*if (GRAPHICS->m_textureMap.empty())
	{
	GRAPHICS->m_textureMap[mTexutureDir] = m_TextureID;
	}
	else
	{
	auto it = GRAPHICS->m_textureMap.find(mTexutureDir);
	for (auto texture : GRAPHICS->m_textureMap)
	{
	if (it == GRAPHICS->m_textureMap.find(texture.first))
	return texture.second;
	else
	{
	m_TextureID = SOIL_load_OGL_texture(filepath.c_str(), SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MULTIPLY_ALPHA | SOIL_FLAG_INVERT_Y);
	GRAPHICS->m_textureMap[mTexutureDir] = m_TextureID;
	}
	}
	}*/



	glActiveTexture(GL_TEXTURE0);
	/*glBindTexture(GL_TEXTURE_2D, m_TextureID);*/

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_REPEAT);

	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	return m_TextureID;
}

void Sprite::LoadAllSprites(std::string path)
{
	char * Userinfo;
	size_t len = path.size();
	_dupenv_s(&Userinfo, &len, "USERPROFILE");

	std::string saveLevel = path;
	//std::string imgpath;
#ifdef _DEBUG
	path = ".\\texture.\\" + path;
	//imgpath = ".\\texture.\\";
#else
	path = Userinfo;
	path += "/Documents/RoteRote/texture/" + saveLevel;
#endif
	free(Userinfo);


	Jsonclass file;
	std::string object = "Images";
	file.ReadFile(path);

	for (int i = 1; i < file.mRoot.get("NumberOfImages", false).asInt() + 1; i++)
	{
		std::string textureDir = file.mRoot.get(object + std::to_string(i), false).get("TextureDir", false).asString();
		textureDir = std::string(".\\texture.\\") + textureDir;
		GLuint tempTextureID = SOIL_load_OGL_texture(textureDir.c_str(), SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MULTIPLY_ALPHA | SOIL_FLAG_INVERT_Y);
		GRAPHICS->m_textureMap[textureDir] = tempTextureID;
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







