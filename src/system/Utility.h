#pragma once

#include <cmath>

/*****************************************************************/
/*!
		\brief
				Helper function to generate a random number from a base
				and a range around the base
				(base - .5range) to (base + .5range)
		\param base
				the base of the random number
		\param range
				the range around the base number
		\return
				the random number that was generated
*/
/*****************************************************************/
static float floatRand(float base, float range)
{
		float zeroToOne = (float)(rand() / (RAND_MAX + 1.0));
		return base + (zeroToOne * range) - (range / 2);
}

