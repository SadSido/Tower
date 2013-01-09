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

	explicit Entity(CL_String type, CL_String name);
	virtual ~Entity();

	// virtual interface:

	virtual bool update(const UpdateCtx &ctx, float secs) = 0;
	virtual bool render(const RenderCtx &ctx) = 0;

	// member querries:

	CL_Rectf getRect() const
	{ return m_rect; }

	CL_Pointf getVel() const
	{ return m_vel; }

	CL_Pointf getAcc() const
	{ return m_acc; }

	CL_String getName() const
	{ return m_name; }

	CL_String getType() const
	{ return m_type; }

	// member assign:

	void setPos(CL_Pointf pos)
	{ m_rect = CL_Rectf(pos, m_rect.get_size()); }

	void setSize(CL_Sizef size)
	{ m_rect = CL_Rectf(m_rect.get_top_left(), size); }

	void setVel(CL_Pointf vel)
	{ m_vel = vel; }

	void setAcc(CL_Pointf acc)
	{ m_acc = acc; }

protected:
	CL_Rectf  m_rect;
	CL_Pointf m_vel;
	CL_Pointf m_acc;

	const CL_String m_name;
	const CL_String m_type;
};

//************************************************************************************************************************

struct Entities : public std::list<Entity::Ref>
{
	typedef std::shared_ptr<Entities> Ref;
};

//************************************************************************************************************************

#endif
