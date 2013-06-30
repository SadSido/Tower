// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: some math and geometry shortcuts

#ifndef _MathUtils_h_
#define _MathUtils_h_

#include <ClanLib/core.h>

//************************************************************************************************************************

// interpolators for "time" to return zero, one at maximum, then back to zero:

float alphaInterpolateLinear    (const float time, const float lifefreq);
float alphaInterpolateParabolic (const float time, const float lifefreq);

//************************************************************************************************************************

// handy class to keep mischellaneous ranges:

struct Range
{
	// constructors:
	explicit Range();
	explicit Range(float _len);
	explicit Range(float _from, float _to);

	// utilities:
	bool isValid() const;
	bool isEmpty() const;
	
	float middle() const;
	float random() const;

	// properties:
	float from;
	float to;
};

//************************************************************************************************************************

// some angle calculation shortcuts:

inline float degToRad(float deg)
{ return deg * 0.0174533f; }

inline float radToDeg(float rad)
{ return rad * 57.295755f; }

//************************************************************************************************************************

CL_Pointf getRandomVec(const Range &direction, const Range &amplitude);

//************************************************************************************************************************

#endif
