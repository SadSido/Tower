// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: base class for every entity in the game

#ifndef _Entities_h_
#define _Entities_h_

#include "../sys/Notify.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <list>

//************************************************************************************************************************

struct LevelCtx;

//************************************************************************************************************************

class Entity
{
public:
	typedef std::shared_ptr<Entity> Ref;
	typedef std::vector<CL_Sprite> SpriteVec;

	// c-tor, d-tor and cloning:

	explicit Entity();
	virtual  Entity::Ref clone();
	virtual ~Entity();

	// public interface:

	bool doUpdate (const LevelCtx &ctx, float secs);
	bool doRender (const LevelCtx &ctx);
	void doNotify (const LevelCtx &ctx, Notify code);

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

protected:

	// virtual interface:

	virtual bool update (const LevelCtx &ctx, float secs) = 0;
	virtual bool render (const LevelCtx &ctx) = 0;
	virtual void notify (const LevelCtx &ctx, Notify code);
	virtual void upload (const LevelCtx &ctx);

	// sprites nums:

	int  getSpriteNo() const;
	int  getSpriteCount() const;
	void setSpriteNo(int no);

	// sprites data:

	CL_Sprite & getSprite();
	SpriteVec & getSprites();

protected:

	CL_Rectf  m_rect;
	CL_Pointf m_vel;
	CL_Pointf m_acc;

	bool m_facing;
	bool m_uploaded;
	int  m_spriteNo;

	SpriteVec m_sprites;
};

//************************************************************************************************************************

struct Entities : public std::list<Entity::Ref>
{
	typedef std::shared_ptr<Entities> Ref;
};

//************************************************************************************************************************

#endif
