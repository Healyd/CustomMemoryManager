#pragma once
#ifndef util
#define util

#include <stdlib.h>
#include <random>
static float getRangedRandom(float min, float max)
{
	int r = rand();

	float r1 = (float)r / (float)0x7fff;

	r1 *= (max - min);

	r1 += min;

	return r1;
}
static int getRangedRandom(int min, int max)
{
	int r = rand();

	float r2 = (float)r / (float)0x7fff;

	r2 *= ((float)max - (float)min + 0.99f);
	int r1 = (int)r2;

	r1 += min;

	return r1;

}
#endif // !util