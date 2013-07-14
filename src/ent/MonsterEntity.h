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
	virtual void onDetected (MonsterEntity * owner, const LevelCtx &ctx) = 0;
	virtual void onCollided (MonsterEntity * owner, const LevelCtx &ctx, const TileTest &test) = 0;
};

struct AttackPolicy
{
	typedef std::shared_ptr<AttackPolicy> Ref;
		
	explicit AttackPolicy() {}
	virtual ~AttackPolicy() {}

	virtual bool onInRange (MonsterEntity * owner, const LevelCtx &ctx) = 0;
	virtual bool onTouched (MonsterEntity * owner, const LevelCtx &ctx) = 0;
};

struct DamagePolicy
{
	typedef std::shared_ptr<DamagePolicy> Ref;

	explicit DamagePolicy() {}
	virtual ~DamagePolicy() {}

	virtual bool onDamage (MonsterEntity * owner, const LevelCtx &ctx) = 0;
};

//************************************************************************************************************************

enum StatesMask
{
	mask_Emerge = 1 << 0,
	mask_Move   = 1 << 1,
	mask_Wait   = 1 << 2,
	mask_Strike = 1 << 3,
	mask_Shoot  = 1 << 4,
	mask_Recoil = 1 << 5,
	mask_Reload = 1 << 6,
	mask_Vanish = 1 << 7,
};

//************************************************************************************************************************

class MonsterEntity : public Entity
{
public:
	enum States
	{ state_Emerge, state_Move, state_Wait, state_Strike, state_Shoot, state_Recoil, state_Reload, state_Vanish, state_Count };

	// state management:
	void enterMoveState(CL_Pointf vel, CL_Pointf acc);
	void enterWaitState();
	void enterReloadState();
	void enterVanishState();
	void enterStrikeState();
	void enterShootState();

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
	explicit MonsterEntity(const CL_DomNodeList &plist, long statesMask);
	Entity::Ref clone();

	// virtual entity interface:
	virtual bool update (const LevelCtx &ctx, float secs);
	virtual bool render (const LevelCtx &ctx);
	virtual void upload (const LevelCtx &ctx);

	// per-state updates:
	void update_Emerge (const LevelCtx &ctx);
	void update_Move   (const LevelCtx &ctx);
	void update_Wait   (const LevelCtx &ctx, float secs);
	void update_Reload (const LevelCtx &ctx, float secs);
	void update_Strike (const LevelCtx &ctx);
	void update_Shoot  (const LevelCtx &ctx);
	void update_Recoil (const LevelCtx &ctx);
	void update_Vanish (const LevelCtx &ctx);

	// handling damage:
	bool checkDamage   (const LevelCtx &ctx);

	// minor helpers:
	bool outsideArea  (const LevelCtx &ctx, float distance) const;
	bool detectPlayer (const LevelCtx &ctx, float distance) const;
	bool touchPlayer  (const LevelCtx &ctx) const;
	bool touchSword   (const LevelCtx &ctx) const;
	void enterState   (int state);
	bool hasState     (int state);

private:
	bool m_alive;
	long m_statesMask;
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
	float m_range;
	float m_reload;

	MovingPolicy::Ref m_mpolicy;
	AttackPolicy::Ref m_apolicy;
	DamagePolicy::Ref m_dpolicy;

public:
	// actual factory for creating monsters. Specify template
	// parameters to get various behavior combinations:

	template<typename MPOLICY, typename APOLICY, typename DPOLICY>
	static Entity::Ref create(const CL_DomNodeList &plist, long statesMask)
	{
		auto monster = new MonsterEntity(plist, statesMask);

		monster->m_mpolicy = MovingPolicy::Ref(new MPOLICY());
		monster->m_apolicy = AttackPolicy::Ref(new APOLICY());
		monster->m_dpolicy = DamagePolicy::Ref(new DPOLICY());

		return Entity::Ref(monster);
	}
};

//************************************************************************************************************************

#endif
