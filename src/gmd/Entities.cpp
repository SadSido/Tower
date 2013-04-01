// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: base class for every entity in the game

#include "Entities.h"
#include <assert.h>

//************************************************************************************************************************

// c-tor and d-tor:

Entity::Entity()
: m_spriteNo(0), m_uploaded(false), m_facing(false)
{
}

Entity::~Entity()
{}

Entity::Ref Entity::clone()
{ return Entity::Ref(); }

// entity lifecycle (not so virtual now):

bool Entity::doUpdate (const LevelCtx &ctx, float secs)
{
	if (!m_uploaded) { m_uploaded = true; upload(ctx); }
	return update(ctx, secs);
}

bool Entity::doRender (const LevelCtx &ctx)
{
	if (!m_uploaded) { m_uploaded = true; upload(ctx); }
	return render(ctx);
}

void Entity::doNotify(const LevelCtx &ctx, Notify code)
{
	notify(ctx, code);
}

// base entity does not react on notifications and loads nothing:

void Entity::notify(const LevelCtx &ctx, Notify code)
{}

void Entity::upload(const LevelCtx &ctx)
{}

// handling the set of sprites:

void Entity::setSpriteNo(int no)
{
	assert(no < getSpriteCount());
	m_spriteNo = no;
}

int Entity::getSpriteNo() const
{ return m_spriteNo; }

int Entity::getSpriteCount() const
{ return (int)m_sprites.size(); }

CL_Sprite & Entity::getSprite()
{
	assert(m_spriteNo < getSpriteCount());
	return m_sprites[m_spriteNo];
}

Entity::SpriteVec & Entity::getSprites()
{
	return m_sprites;
}

//************************************************************************************************************************
