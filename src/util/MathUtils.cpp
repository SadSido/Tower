// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: some math and geometry shortcuts

#include "MathUtils.h"
#include <assert.h>

//************************************************************************************************************************

float alphaInterpolateLinear(const float time, const float lifefreq)
{
	return 1.0f - abs(2.0f * time * lifefreq - 1.0f);
}

float alphaInterpolateParabolic(const float time, const float lifefreq)
{
	return 4.0f * lifefreq * time * (1.0f - lifefreq * time);
}


//************************************************************************************************************************
