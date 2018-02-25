/**
\file        TUMath.h
\author      KyungOok Park
\par         email: qkrruddn6680@gmail.com
\par         course: GAM200
\par	ThumbUp Engine
\date        12/12/2017
\brief
Helper function for Thumbup Engine
*/
#pragma once
namespace TUMath
{
	float  DegreeToRadian(float degrees);

	float  RadianToDegree(float radians);

	float GetRandomFloat(float floata, float floatb);

	int GetRandomInt(int randA, int randB);

	float  Clamp(float x, float low, float high);

	float  Wrap(float x, float low, float high);

	float  Min(float x, float y);

	float  Max(float x, float y);

	bool  IsInRange(float x, float low, float high);

	bool  IsFloatEqual(float x, float y);

	bool  IsPowerOf2(int x);

	int  GetNextPowerOf2(int x);

	int  Lerp(int start, int end, float time);

	char  Lerp(char start, char end, float time);

	float  Lerp(float start, float end, float time);

	int Round(float num);

	int GridRound(float num);

	const float  EPSILON = 0.00001f;

	const float  PI = 3.1415926535897932384626433832795f;

	const float  HALF_PI = (PI * .5f);

	const float  TWO_PI = (PI * 2.0f);


};
