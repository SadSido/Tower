// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: base class for every entity in the game

#include "Entities.h"
#include <assert.h>

//************************************************************************************************************************

static const float s_recoverTime = 0.3f;

//************************************************************************************************************************

// c-tor and d-tor:

Entity::Entity()
: m_stateNo(0), m_uploaded(false), m_facing(false), m_health(1.0f), m_recover(0.0f)
{}

Entity::~Entity()
{}

Entity::Ref Entity::clone()
{ assert(false); return Entity::Ref(); }

// entity lifecycle (not so virtual now):

bool Entity::doUpdate (const LevelCtx &ctx, float secs)
{
	// perform upload if necessary:
	if (!m_uploaded) { m_uploaded = true; upload(ctx); }

	// autodecrement recover time:
	if (m_recover) { m_recover  = max(m_recover - secs, 0.0f); }

	// actual update:
	return update(ctx, secs);
}

bool Entity::doRender (const LevelCtx &ctx)
{
	// perform upload if necessary:
	if (!m_uploaded) { m_uploaded = true; upload(ctx); }

	// actual render:
	return render(ctx);
}

void Entity::doNotify(const LevelCtx &ctx, Notify code)
{ notify(ctx, code); }

void Entity::doDamage(const LevelCtx &ctx, float ammount)
{ damage(ctx, ammount); }

// base entity does not react on notifications and loads nothing:

void Entity::notify(const LevelCtx &ctx, Notify code)
{}

void Entity::upload(const LevelCtx &ctx)
{}

void Entity::damage(const LevelCtx &ctx, float ammount)
{
	if (m_recover == 0.0f)
	{ 
		m_health  = max(m_health - ammount, 0.0f);
		m_recover = (m_health) ? s_recoverTime : 0.0f;
	}
}

// state numbers:

void Entity::setStateNo(int no)
{ m_stateNo = no; }

int Entity::getStateNo() const
{ return m_stateNo; }

// handling the set of sprites & hitmaps:

CL_Sprite & Entity::getSprite()
{
	assert(m_stateNo < (int)m_sprites.size());
	return m_sprites[m_stateNo];
}

Entity::SpriteVec & Entity::getSprites()
{ return m_sprites; }

Hitmap & Entity::getHitmap()
{
	assert(m_stateNo < (int)m_hitmaps.size());
	return m_hitmaps[m_stateNo];
}

Entity::HitmapVec & Entity::getHitmaps()
{ return m_hitmaps; }

//************************************************************************************************************************
