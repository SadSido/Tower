// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: base class for the game monsters

#ifndef _MonsterEntity_h_
#define _MonsterEntity_h_

#include "../gmd/Entities.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <memory>

//************************************************************************************************************************

struct LevelCtx;
struct TileTest;
class MonsterEntity;

//************************************************************************************************************************

// policies are the basic interfaces for monster behavior. Every
// monster can be customized by assigning a set of policies:

struct MovingPolicy
{
	typedef std::shared_ptr<MovingPolicy> Ref;
		
	explicit MovingPolicy() {}
	virtual ~MovingPolicy() {}

	virtual void onStarted  (MonsterEntity * owner, const LevelCtx &ctx) = 0;
	virtual void onReached  (MonsterEntity * owner, const LevelCtx &ctx) = 0;
	virtual void onWaited   (MonsterEntity * owner, const LevelCtx &ctx) = 0;
	virtual void onCollided (MonsterEntity * owner, const LevelCtx &ctx, const TileTest &test) = 0;
};

struct AttackPolicy
{
	typedef std::shared_ptr<AttackPolicy> Ref;
		
	explicit AttackPolicy() {}
	virtual ~AttackPolicy() {}
};

//************************************************************************************************************************

// I GUESS WE SHOULD SEPARATE MONSTER'S HEALTH AND PLAYER'S HEALTH. NO NEED TO MOVE IT TO BASE ENTITY...

class MonsterEntity : public Entity
{
public:
	enum States
	{ state_Emerge, state_Move, state_Wait, state_Vanish, state_Count };

	// c-tors and d-tors:
	explicit MonsterEntity(const CL_DomNodeList &plist);

	// state management:
	void enterMoveState(CL_Pointf vel, CL_Pointf acc);
	void enterWaitState();

	// many getters:
	float getSpeed() const
	{ return m_speed; }

private:
	// virtual entity interface:
	virtual bool update (const LevelCtx &ctx, float secs);
	virtual bool render (const LevelCtx &ctx);
	virtual void upload (const LevelCtx &ctx);

	// per-state updates:
	void update_Emerge (const LevelCtx &ctx);
	void update_Move   (const LevelCtx &ctx);
	void update_Wait   (const LevelCtx &ctx, float secs);
	void update_Vanish (const LevelCtx &ctx);

	// handling damage:
	bool checkDamage (const LevelCtx &ctx);
	void checkPlayer (const LevelCtx &ctx);

	// minor helpers:
	bool outsideArea();
	void enterState(int state);

private:
	bool m_alive;
	CL_String m_prefix;

	CL_Pointf m_basePos;
	CL_Pointf m_nextPos;

	float m_speed;
	float m_areal;
	float m_waittime;
	float m_towait;
	float m_damage;
//	float m_health;

	MovingPolicy::Ref m_mpolicy;
	AttackPolicy::Ref m_apolicy;
};

//************************************************************************************************************************

#endif
