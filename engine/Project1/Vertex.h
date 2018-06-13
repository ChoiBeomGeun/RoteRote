/******************************************************************************/
/*!
\file        Vertex.h
\author      HyunJun Yoo
\par         email: hyunjun306@gmail.com
\par         course: GAM250
\date        12/11/2017

Vertex.h contains position, color, UV.
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include <glew.h>
namespace TE {
	struct Position { // Position is the type
		float x;
		float y;
		float z;
	}; // position is the name of this particular instance

	   struct Color {
	   	GLubyte r;
	   	GLubyte g;
	   	GLubyte b;
	   	GLubyte a;
	   };

	struct Uv {
		float u;
		float v;
	};

	// the vertex definition
	struct Vertex
	{
		// this is the position struct. when you store a struct or class
		// inside of another struct or class , it is called composition. This is 
		// layed out exactly the same in memory as if we had a float position[2],
		// but doing it this way makes more sense
		// this is exaclty same as  Position position 
		Position position;

		// 4 byes for r g b a color.
		Color color;

		// UV texture coordinates.
		Uv uv;

		void setPosition(float x, float y, float z) {
			position.x = x;
			position.y = y;
			position.z = z;
		}

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
		}

		void setUV(float u, float v)
		{
			uv.u = u;
			uv.v = v;
		}
		
	};
}