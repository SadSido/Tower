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

#endif
