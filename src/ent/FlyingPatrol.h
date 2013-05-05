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

class BasePatrol : public Entity
{
public:
	explicit BasePatrol(const CL_DomNodeList &plist);

private:
	// virtual entity interface:
	virtual bool update (const LevelCtx &ctx, float secs);
	virtual bool render (const LevelCtx &ctx);
	virtual void upload (const LevelCtx &ctx);

	// per-state updates:
	void enterState    (int state, CL_Pointf vel);
	void update_Emerge (const LevelCtx &ctx);
	void update_Move   (const LevelCtx &ctx);
	void update_Wait   (const LevelCtx &ctx, float secs);
	void update_Vanish (const LevelCtx &ctx);

	// handling damage stuff:
	bool checkDamage (const LevelCtx &ctx);
	void checkPlayer (const LevelCtx &ctx);

protected:
	// destination point selection:
	virtual void setNextPos() = 0;
	virtual bool reachedPos() = 0;

protected:
	bool m_alive;

	CL_Pointf m_basePos;
	CL_Pointf m_nextPos;
	CL_String m_prefix;

	float m_speed;
	float m_distance;
	float m_waittime;
	float m_towait;
	float m_damage;
};

//************************************************************************************************************************

class FlyingPatrol : public BasePatrol
{
public:
	explicit FlyingPatrol(const CL_DomNodeList &plist);
	virtual FlyingPatrol::Ref clone();

private:
	// destination point selection:
	virtual void setNextPos();
	virtual bool reachedPos();
};

class GroundPatrol : public BasePatrol
{
public:
	explicit GroundPatrol(const CL_DomNodeList &plist);
	virtual GroundPatrol::Ref clone();

private:
	// destination point selection:
	virtual void setNextPos();
	virtual bool reachedPos();
};

//************************************************************************************************************************

#endif
