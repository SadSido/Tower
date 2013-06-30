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

	virtual bool onDetected (MonsterEntity * owner, const LevelCtx &ctx) = 0;
	virtual bool onTouched  (MonsterEntity * owner, const LevelCtx &ctx) = 0;
};

struct DamagePolicy
{
	typedef std::shared_ptr<DamagePolicy> Ref;

	explicit DamagePolicy() {}
	virtual ~DamagePolicy() {}

	virtual bool onDamage (MonsterEntity * owner, const LevelCtx &ctx) = 0;
};

//************************************************************************************************************************

// I GUESS WE SHOULD SEPARATE MONSTER'S HEALTH AND PLAYER'S HEALTH. NO NEED TO MOVE IT TO BASE ENTITY...

class MonsterEntity : public Entity
{
public:
	enum States
	{ state_Emerge, state_Move, state_Wait, state_Vanish, state_Count };

	// state management:
	void enterMoveState(CL_Pointf vel, CL_Pointf acc);
	void enterWaitState();
	void enterVanishState();

	// many getters:
	float getSpeed() const
	{ return m_speed; }

	float getDamage() const
	{ return m_damage; }
	
	bool isRecovering() const
	{ return m_recover > 0.0f; }

	// dealing damage:
	float applyDamage(float ammount);

private:
	// c-tors and d-tors:
	explicit MonsterEntity(const CL_DomNodeList &plist);
	Entity::Ref clone();

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

	// minor helpers:
	bool outsideArea  (const LevelCtx &ctx) const;
	bool detectPlayer (const LevelCtx &ctx) const;
	bool touchPlayer  (const LevelCtx &ctx) const;
	bool touchSword   (const LevelCtx &ctx) const;
	void enterState   (int state);

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
	float m_health;
	float m_recover;
	float m_detect;

	MovingPolicy::Ref m_mpolicy;
	AttackPolicy::Ref m_apolicy;
	DamagePolicy::Ref m_dpolicy;

public:
	// actual factory for creating monsters. Specify template
	// parameters to get various behavior combinations:

	template<typename MPOLICY, typename APOLICY, typename DPOLICY>
	static Entity::Ref create(const CL_DomNodeList &plist)
	{
		auto monster = new MonsterEntity(plist);

		monster->m_mpolicy = MovingPolicy::Ref(new MPOLICY());
		monster->m_apolicy = AttackPolicy::Ref(new APOLICY());
		monster->m_dpolicy = DamagePolicy::Ref(new DPOLICY());

		return Entity::Ref(monster);
	}
};

//************************************************************************************************************************

#endif
