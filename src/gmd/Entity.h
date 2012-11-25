// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: base class for every entity in the game

#ifndef _Entity_h_
#define _Entity_h_

#include "TileTest.h"
#include <ClanLib/core.h>

//************************************************************************************************************************

class Entity
{
public:
	explicit Entity(CL_Pointf pos, CL_Sizef size);
	virtual ~Entity();

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

#endif
