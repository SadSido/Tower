// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: base class for every entity in the game

#include "Entities.h"
#include <assert.h>

//************************************************************************************************************************

// c-tor and d-tor:

Entity::Entity()
: m_stateNo(0), m_uploaded(false), m_facing(false)
{}

Entity::~Entity()
{}

Entity::Ref Entity::clone()
{ assert(false); return Entity::Ref(); }

// entity lifecycle (not so virtual now):

bool Entity::doUpdate (const LevelCtx &ctx, float secs)
{
	// upload once:
	doUpload(ctx);

	// actual update:
	return update(ctx, secs);
}

bool Entity::doRender (const LevelCtx &ctx)
{
	// upload once:
	doUpload(ctx);

	// actual render:
	return render(ctx);
}

void Entity::doNotify(const LevelCtx &ctx, Notify code)
{ 
	notify(ctx, code); 
}

void Entity::doUpload(const LevelCtx &ctx)
{ 
	if (!m_uploaded) { m_uploaded = true; upload(ctx); }
}

// base entity does not react on notifications and loads nothing:

void Entity::notify(const LevelCtx &ctx, Notify code)
{}

void Entity::upload(const LevelCtx &ctx)
{}

// state numbers:

void Entity::setStateNo(int no)
{ m_stateNo = no; }

int Entity::getStateNo() const
{ return m_stateNo; }

// handling the set of sprites & hitmaps & sounds:

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

CL_SoundBuffer & Entity::getSound()
{
	assert(m_stateNo < (int)m_sounds.size());
	return m_sounds[m_stateNo];
}

Entity::SoundVec & Entity::getSounds()
{ return m_sounds; }

//************************************************************************************************************************
