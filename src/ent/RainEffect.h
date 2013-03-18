// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: effect to show the neverending rain

#ifndef _RainEffect_h_
#define _RainEffect_h_

#include "../gmd/LevelScene.h"
#include "../gmd/Entities.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

class RainEffect : public Entity
{
	struct Raindrop
	{
		CL_Pointf pos;
		CL_Pointf vel;
		float life;
	};

public:
	// common constructor for the entities:
	explicit RainEffect(CL_String name, const CL_DomNodeList &props);

private:
	// virtual entity interface:
	virtual bool update(const LevelCtx &ctx, float secs);
	virtual bool render(const LevelCtx &ctx);

	// properties:
	float m_interval;
	float m_tospawn;
	float m_lifetime;
	
	std::list<Raindrop> m_drops;
};

//************************************************************************************************************************

#endif
