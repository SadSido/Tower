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
	explicit FlyingPatrol(const CL_DomNodeList &plist);
	virtual  Entity::Ref clone();

protected:
	#pragma region enums

	enum Sprites
	{
		spr_Emerge,
		spr_Move,
		spr_Wait,
		spr_Vanish,
		spr_Count,
	};
	
	#pragma endregion

private:
	// virtual entity interface:
	virtual bool update (const LevelCtx &ctx, float secs);
	virtual bool render (const LevelCtx &ctx);
	virtual void upload (const LevelCtx &ctx);

	// per-state updates:
	void enterState    (Sprites num, CL_Pointf vel);
	void update_Emerge (const LevelCtx &ctx);
	void update_Move   (const LevelCtx &ctx);
	void update_Wait   (const LevelCtx &ctx, float secs);
	void update_Vanish (const LevelCtx &ctx);

private:
	bool m_alive;

	CL_Pointf m_basePos;
	CL_Pointf m_nextPos;
	CL_String m_prefix;

	float m_speed;
	float m_distance;
	float m_waittime;
	float m_towait;

	// minor helpers:
	void setNextPos();
	bool reachedPos();
};

//************************************************************************************************************************

#endif
