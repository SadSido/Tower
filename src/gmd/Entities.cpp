// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: base class for every entity in the game

#include "Entities.h"
#include <assert.h>

//************************************************************************************************************************

// c-tor and d-tor:

Entity::Entity(CL_String type, CL_String name, int spriteCount)
: m_type(type), m_name(name), m_spriteNo(0)
{
	// allocate sprites array:
	if (spriteCount) { m_sprites.resize(spriteCount); }
}

Entity::~Entity()
{}

// base entity does not react on notifications:

void Entity::notify(const UpdateCtx &ctx, Notify code)
{}

// handling the set of sprites:

void Entity::setSpriteNo(int no)
{
	assert(no < m_sprites.size());
	m_spriteNo = no;
	
	// restart when switched:
	// getSprite().restart();
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
