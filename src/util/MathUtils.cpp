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

Range::Range() : from(0.0f), to(0.0f)
{}

Range::Range(float _len) : from(0.0f), to(_len)
{ assert(isValid()); }

Range::Range(float _from, float _to) : from(_from), to(_to)
{ assert(isValid()); }

bool Range::isValid() const
{ return (to >= from); }

bool Range::isEmpty() const
{ return (to == from); }

float Range::middle() const
{ return (to - from) / 2.0f; }

float Range::random() const
{ return from + (to - from) * (rand() % 64) / 64.0f; }

//************************************************************************************************************************
