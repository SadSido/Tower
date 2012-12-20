// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: base class for every entity in the game

#ifndef _Entities_h_
#define _Entities_h_

#include <ClanLib/core.h>
#include <list>

//************************************************************************************************************************

struct UpdateCtx;
struct RenderCtx;

//************************************************************************************************************************

class Entity
{
public:
	typedef std::shared_ptr<Entity> Ref;

	// c-tor and d-tor:

	explicit Entity(CL_Pointf pos, CL_Sizef size);
	virtual ~Entity();

	// virtual interface:

	virtual bool update(const UpdateCtx &ctx, unsigned int msecs) = 0;
	virtual bool render(const RenderCtx &ctx) = 0;

	// member querries:

	CL_Rectf getRect() const
	{ return m_rect; }

	CL_Pointf getVel() const
	{ return m_vel; }

	CL_Pointf getAcc() const
	{ return m_acc; }

	// member assign:

	void setPos(CL_Pointf pos)
	{ m_rect = CL_Rectf(pos, m_rect.get_size()); }

	void setVel(CL_Pointf vel)
	{ m_vel = vel; }

	void setAcc(CL_Pointf acc)
	{ m_acc = acc; }

protected:
	CL_Rectf  m_rect;
	CL_Pointf m_vel;
	CL_Pointf m_acc;
};

//************************************************************************************************************************

struct Entities : public std::list<Entity::Ref>
{
	typedef std::shared_ptr<Entities> Ref;
};

//************************************************************************************************************************

#endif
