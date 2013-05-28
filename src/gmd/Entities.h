// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: base class for every entity in the game

#ifndef _Entities_h_
#define _Entities_h_

#include "../sys/Notify.h"
#include "Hitmap.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/sound.h>
#include <list>

//************************************************************************************************************************

struct LevelCtx;

//************************************************************************************************************************

class Entity
{
public:
	typedef std::shared_ptr<Entity> Ref;
	typedef std::vector<CL_Sprite> SpriteVec;
	typedef std::vector<Hitmap> HitmapVec;
	typedef std::vector<CL_SoundBuffer> SoundVec;

	// c-tor, d-tor and cloning:

	explicit Entity();
	virtual  Entity::Ref clone();
	virtual ~Entity();

	// public interface:

	bool doUpdate (const LevelCtx &ctx, float secs);
	bool doRender (const LevelCtx &ctx);
	void doNotify (const LevelCtx &ctx, Notify code);
	void doDamage (const LevelCtx &ctx, float ammount);

	// member querries:

	CL_Rectf getRect() const
	{ return m_rect; }

	CL_Pointf getCenter() const
	{ return m_rect.get_center(); }

	CL_Pointf getVel() const
	{ return m_vel; }

	CL_Pointf getAcc() const
	{ return m_acc; }

	float getFacing() const
	{ return m_facing ? +1.0f : -1.0f; }

	bool isRecovering() const
	{ return m_recover > 0.0f; }

	// member assign:

	void setPos(CL_Pointf pos)
	{ m_rect = CL_Rectf(pos, m_rect.get_size()); }

	void setSize(CL_Sizef size)
	{ m_rect = CL_Rectf(m_rect.get_top_left(), size); }

	void setVel(CL_Pointf vel)
	{ m_vel = vel; }

	void setAcc(CL_Pointf acc)
	{ m_acc = acc; }

	void setFacing()
	{ m_facing = (m_vel.x) ? (m_vel.x > 0.0f) : m_facing; }

	// recovering utils:

	CL_Colorf getRecoverColor() const;

protected:

	// virtual interface:

	virtual bool update (const LevelCtx &ctx, float secs) = 0;
	virtual bool render (const LevelCtx &ctx) = 0;
	virtual void notify (const LevelCtx &ctx, Notify code);
	virtual void upload (const LevelCtx &ctx);
	virtual void damage (const LevelCtx &ctx, float ammount);

	// state nums:

	int  getStateNo() const;
	void setStateNo(int no);

	// sprites & hitmaps & sounds:

	CL_Sprite & getSprite();
	SpriteVec & getSprites();
	Hitmap    & getHitmap();
	HitmapVec & getHitmaps();
	SoundVec  & getSounds();

protected:

	CL_Rectf  m_rect;
	CL_Pointf m_vel;
	CL_Pointf m_acc;

	bool m_facing;
	bool m_uploaded;
	int  m_stateNo;
	
	float m_health;
	float m_recover;

	SpriteVec m_sprites;
	HitmapVec m_hitmaps;
	SoundVec  m_sounds;
};

//************************************************************************************************************************

struct Entities : public std::list<Entity::Ref>
{
	typedef std::shared_ptr<Entities> Ref;
};

//************************************************************************************************************************

#endif
