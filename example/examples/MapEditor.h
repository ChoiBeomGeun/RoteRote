/******************************************************************************/
/*!
\file   MapEditor.h
\author KyungOok Park
\par    email: qkrruddn6680@gmail.com
\par    course: GAM250
\par	ThumbUp Engine
\date   12/12/2017
\brief
MapEditor state

Load Level and Edit it.
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "BaseStage.h"
#include "EditorData.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL.h>

class MapEditor :public BaseStage
{
public:
	MapEditor();
	~MapEditor();
	void Load(void);
	void Init(void);
	void Update(float dt);
	void Free(void);
	void Unload(void);

private:
	glm::vec3 mouse;
	glm::vec3 OBJ_scale;
	int x = 0;
	int y = 0;
};