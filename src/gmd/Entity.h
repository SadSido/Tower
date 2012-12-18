// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: base class for every entity in the game

#ifndef _Entity_h_
#define _Entity_h_

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

	// position querries:

	CL_Rectf getRect() const
	{ return m_rect; }

	CL_Pointf getVel() const
	{ return m_vel; }

	CL_Pointf getAcc() const
	{ return m_acc; }

protected:
	CL_Rectf  m_rect;
	CL_Pointf m_vel;
	CL_Pointf m_acc;
};

//************************************************************************************************************************

typedef std::list<Entity::Ref> Entities;

//************************************************************************************************************************

#endif
