/******************************************************************************/
/*!
\file	TUMath.h
\author KyungOok Park
\par    email: qkrruddn6680@gmail.com
\par    course: GAM250
\par	ThumbUp Engine
\date   12/12/2017
\brief
Helper function for Thumbup Engine

All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "TUMath.h"
#include <random>
float TUMath::DegreeToRadian(float degrees)
{
	return (PI / 180) * degrees;
}

float TUMath::RadianToDegree(float radians)
{
	return (radians * 180.f) / PI;
}

float TUMath::GetRandomFloat(float floata, float floatb)
{
	float temp;
	temp = floata + static_cast<float> (rand()) / (static_cast<float> (RAND_MAX / (floatb - floata)));

	return temp;
}

int TUMath::GetRandomInt(int randA, int randB)
{
	int temp;
	temp = rand() % randB + randA;
	return temp;
}

float TUMath::Clamp(float x, float low, float high)
{
	if (x < low)
		return low;
	else if (x > high)
		return high;
	return x;
}

float TUMath::Wrap(float x, float low, float high)
{
	if (x < low)
		return high;
	else if (x > high)
		return low;
	return x;
}

float TUMath::Min(float x, float y)
{
	return (x > y) ? y : x;
}

float TUMath::Max(float x, float y)
{
	return (x < y) ? y : x;
}

bool TUMath::IsInRange(float x, float low, float high)
{
	return (x >= low) && (x <= high);
}

bool TUMath::IsPowerOf2(int x)
{
	return (x != 0) && ((x & (x - 1)) == 0); // bit operator
}

int TUMath::Lerp(int start, int end, float time)
{
	return int((1 - time)*start + time*end);
}

float TUMath::Lerp(float start, float end, float time)
{
	return (1 - time)*start + time*end;
}

int TUMath::Round(float num)
{
	int round_num = (int)(num + 0.5f);
	return round_num;
}


int TUMath::GridRound(float num)
{
	int round_num = (int)(num * 0.02f);

	round_num *= 50;

	return round_num;
}