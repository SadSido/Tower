// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: base class for every entity in the game

#include "Entities.h"
#include <assert.h>

//************************************************************************************************************************

// c-tor and d-tor:

Entity::Entity(CL_String type, CL_String name)
: m_type(type), m_name(name), m_spriteNo(0), m_uploaded(false)
{
}

Entity::~Entity()
{}

// entity lifecycle (not so virtual now):

bool Entity::doUpdate (const LevelCtx &ctx, float secs, int msecs)
{
	if (!m_uploaded) { m_uploaded = true; upload(ctx); }
	return update(ctx, secs, msecs);
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
	assert(no < m_sprites.size());
	m_spriteNo = no;
}

int Entity::getSpriteNo()
{
	return m_spriteNo;
}

CL_Sprite & Entity::getSprite()
{
	assert(m_spriteNo < m_sprites.size());
	return m_sprites[m_spriteNo];
}

//************************************************************************************************************************
