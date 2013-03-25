// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: base class for a flying passievely patrolling monster

#ifndef _FlyingPatrol_h_
#define _FlyingPatrol_h_

#include "../gmd/Entities.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

struct LevelCtx;

//************************************************************************************************************************

class FlyingPatrol : public Entity
{
public:
	explicit FlyingPatrol(CL_Pointf pos, CL_Sizef size);
	virtual  Entity::Ref clone();

private:
	#pragma region enums

	enum Sprites
	{
		spr_Emerge,
		spr_Move,
		spr_Vanish,
		spr_Count,
	};
	
	#pragma endregion

private:
	// missile entity:
	Entity::Ref m_missile;

	// sprites & rendering:
	float m_facing;

	// virtual entity interface:
	virtual bool update (const LevelCtx &ctx, float secs);
	virtual bool render (const LevelCtx &ctx);

	// per-state updates:
	void enterState    (Sprites num);
	void update_Emerge (const LevelCtx &ctx);
	void update_Move   (const LevelCtx &ctx);
	void update_Vanish (const LevelCtx &ctx);
};

//************************************************************************************************************************

#endif
