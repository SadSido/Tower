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

//************************************************************************************************************************

// I GUESS WE SHOULD SEPARATE MONSTER'S HEALTH AND PLAYER'S HEALTH. NO NEED TO MOVE IT TO BASE ENTITY...

class MonsterEntity : public Entity
{
public:
	// policies define monsters behavior: flying 
	// or still, melee or shooting missiles, etc:

	struct MovingPolicy
	{
		typedef std::shared_ptr<MovingPolicy> Ref;
		
		explicit MovingPolicy() {}
		virtual ~MovingPolicy() {}
	};

	struct DetectPolicy
	{
		typedef std::shared_ptr<MovingPolicy> Ref;
		
		explicit DetectPolicy() {}
		virtual ~DetectPolicy() {}
	};

public:
	// c-tors and d-tors:
	explicit MonsterEntity(const CL_DomNodeList &plist);

	// state management:
	void enterState(int state, CL_Pointf vel);

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

	// handling damage stuff:
	bool checkDamage (const LevelCtx &ctx);
	void checkPlayer (const LevelCtx &ctx);

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
	DetectPolicy::Ref m_dpolicy;
};

//************************************************************************************************************************

#endif
