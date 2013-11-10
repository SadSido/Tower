// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: class for keeping tilemap info

#include "Tilemap.h"
#include "LevelScene.h"
#include <assert.h>

//************************************************************************************************************************

namespace 
{

// range clamping:

float clamp(float value, float vmin, float vmax)
{ return max(min(value, vmax), vmin); }

// collision test helpers:

bool between(float a, float b, float test)
{ return (a > b) ? (test < a) && (test > b) : (test < b) && (test > a); }

// coordinates getters:

typedef float& (*FnGet)(CL_Pointf&);

float& getX (CL_Pointf &point) 
{ return point.x; }

float& getY (CL_Pointf &point) 
{ return point.y; }

// coordinates swappers:

typedef void (*FnSwap)(int&, int&);

void doSwap(int& one, int& two) 
{ std::swap(one, two); }

void noSwap(int& one, int& two) 
{ return; }

// generalized version of line checker:

template<TileHit type, FnGet x, FnGet y, FnSwap sw>
void checkLines(const Tilemap &map, CL_Pointf ptOne, float length, CL_Pointf delta, TileChecker checker, TileTest &test)
{
	// precalc some useful deltas and parameters which differ for positive
	// and negative directions of movement:

	const int moveSg  = (y(delta) > 0) ? +1 : -1;
	const int rounder = (y(delta) > 0) ? +0 : -1;
	const int offset  = 1 + rounder;
	
	// checking against the coordinate lines. consider the code as a checking
	// against latitudes, and swap x and y, when checking longitudes:

	int start = int(y(ptOne)) + offset;
	for (float line = float(start); between(y(ptOne), y(ptOne) + y(delta), line); line += moveSg)
	{
		float inter = x(ptOne) + ((line - y(ptOne)) * x(delta)) / y(delta);

		int lineCol = (int)(inter);
		int lineEnd = (int)(inter + length);
		int lineRow = (int)(line) + rounder;

		for (int colNo = lineCol; colNo <= lineEnd; ++ colNo)
		{
			// coordinates may require swapping:
			int coords [] = {colNo, lineRow};
			sw(coords[0], coords[1]);

			// finally, request the tilemap:
			const TileDesc &tile = map.getTile(coords[0], coords[1]);
			if (checker(tile))
			{ 
				CL_Pointf result;
				x(result) = inter;
				y(result) = line - moveSg * 0.015f;

				// fill in the collision info:
				test.tile  = tile;
				test.delta = result - ptOne; 
				test.type  = type;
				
				return;
			}
		}
	}
}

// wrappers for readability:

void checkHorizontal(const Tilemap &map, CL_Pointf ptOne, float length, CL_Pointf delta, TileChecker checker, TileTest &test)
{ 
	return checkLines<th_Horizontal, getY, getX, doSwap>(map, ptOne, length, delta, checker, test); 
}

void checkVertical(const Tilemap &map, CL_Pointf ptOne, float length, CL_Pointf delta, TileChecker checker, TileTest &test)
{ 
	return checkLines<th_Vertical, getX, getY, noSwap>(map, ptOne, length, delta, checker, test); 
}

// layer getters:

typedef int (*FnLayer)(const TileDesc &desc);

int backLayer(const TileDesc &desc)
{ return desc.backID; }

int deckLayer(const TileDesc &desc)
{ return desc.deckID; }

int foreLayer(const TileDesc &desc)
{ return desc.foreID; }

// generalized version of layer rendering:

template <FnLayer layer>
void renderLayer(Tilemap &map, const LevelCtx &ctx, CL_Pointf offset, CL_Sizef wndSize, float tileSize)
{
	const int tilesInX = (int)(wndSize.width  / tileSize) + 3;
	const int tilesInY = (int)(wndSize.height / tileSize) + 3;

	const int startX = (int)offset.x - tilesInX / 2;
	const int startY = (int)offset.y - tilesInY / 2;

	// screen coordinate of the top-left tile:
	const CL_Pointf topLeft = CL_Pointf((float)startX, (float)startY);
	const CL_Pointf anchor  = map.toScreen(topLeft);

	// rendering in "ints" results in seamless tiling:
	for (int tileY = startY, pointY = (int)anchor.y; tileY < startY + tilesInY; ++ tileY, pointY += (int)tileSize)
	for (int tileX = startX, pointX = (int)anchor.x; tileX < startX + tilesInX; ++ tileX, pointX += (int)tileSize)
	{
		if (int proxyID = layer(map.getTile(tileX, tileY)))
		{
			TileProxy proxy = map.getProxy(proxyID, ctx);
			proxy.sprite.set_frame(proxy.frame);
			proxy.sprite.draw(ctx.gc, pointX, pointY);
		}
	}
}

}

//************************************************************************************************************************

Tilemap::Tilemap(int dimX, int dimY, int size)
: m_dimX(dimX), m_dimY(dimY), m_size(float(size))
{
	// reserve tilespace:
	m_tiles.reserve(dimX * dimY);

	// init null proxy:
	m_proxies.push_back(TileProxy());
}

TileDesc Tilemap::getTile(int x, int y) const
{
	static const TileDesc defTile = { tf_Blocking, 0, 0, 0 };

	if (x < 0 || x >= m_dimX) { return defTile; }
	if (y < 0 || y >= m_dimY) { return defTile; }
	
	return m_tiles[m_dimX * y + x]; 
}

TileDesc Tilemap::getTile(CL_Pointf pt) const
{
	return getTile((int)pt.x, (int)pt.y);
}

TileProxy Tilemap::getProxy(int id, const LevelCtx &ctx)
{
	TileProxy &proxy = m_proxies[id];

	if (proxy.sprite.is_null())
	{ proxy.sprite = CL_Sprite(ctx.gc, proxy.name, &ctx.assets); }

	return proxy; 
}

// coordinates setting:

void Tilemap::offset (CL_Pointf pt) 
{ 
	m_offset.x = clamp(pt.x, m_offmin.width, (float)m_dimX - m_offmin.width);
	m_offset.y = clamp(pt.y, m_offmin.height, (float)m_dimY - m_offmin.height);
}

void Tilemap::window (CL_Sizef sz)  
{ 
	m_window = sz; 
	m_offmin = sz / (2.0f * m_size);
}


// coordinates conversion:

CL_Pointf Tilemap::toScreen(CL_Pointf pt) const	
{
	CL_Pointf center(m_window.width / 2, m_window.height / 2);
	return (pt - m_offset) * m_size + center;
}

CL_Rectf Tilemap::toScreen(CL_Rectf rect) const
{
	return CL_Rectf(toScreen(rect.get_top_left()), rect.get_size() * m_size);
}

CL_Pointf Tilemap::toTilespace(CL_Pointf pt) const
{
	CL_Pointf center(m_window.width / 2, m_window.height / 2);
	return (pt - center) / m_size + m_offset;
}

// pushing data into tilemap (while loading only):

void Tilemap::pushDesc(TileDesc desc)
{ 
	m_tiles.push_back(desc); 
}

void Tilemap::pushProxy(const CL_String &name, int count)
{ 
	for (int no = 0; no < count; ++ no)
	{
		TileProxy proxy = { no, name, CL_Sprite() };
		m_proxies.push_back(proxy);
	}
}

void Tilemap::pushImage(const CL_String &name)
{
	m_bgName = name;
}

// movement constraints:

TileTest Tilemap::checkMove(CL_Rectf rect, CL_Pointf delta, TileChecker checker) const
{
	TileTest test = { th_None, TileDesc(), delta };

	if (delta.x != 0.0f) // check horizontal movement
	{
		CL_Pointf ptTop = (delta.x > 0) ? rect.get_top_right() : rect.get_top_left();
		checkHorizontal(*this, ptTop, rect.get_height(), test.delta, checker, test);
	}

	if (delta.y != 0.0f) // check vertical movement
	{
		CL_Pointf ptLef = (delta.y > 0) ? rect.get_bottom_left()  : rect.get_top_left();
		checkVertical(*this, ptLef, rect.get_width(), test.delta, checker, test);
	}

	return test;
}

// rendering:

void Tilemap::renderImage(const LevelCtx &ctx)
{
	if (!m_bgName.empty())
	{
		if (m_bgSprite.is_null()) 
		{ m_bgSprite = CL_Sprite(ctx.gc, m_bgName, &ctx.assets); }
		
		assert(!m_bgSprite.is_null());
		m_bgSprite.draw(ctx.gc, CL_Rectf(ctx.gc.get_size())); 
	}
}

void Tilemap::renderBackground(const LevelCtx &ctx)
{
	// maybe static image:
	renderImage(ctx);

	// render background tiles:
	renderLayer<backLayer>(*this, ctx, m_offset, m_window, m_size);
}

void Tilemap::renderDecoration(const LevelCtx &ctx)
{
	// render foreground tiles:
	renderLayer<deckLayer>(*this, ctx, m_offset, m_window, m_size);
}

void Tilemap::renderForeground(const LevelCtx &ctx)
{
	// render foreground tiles:
	renderLayer<foreLayer>(*this, ctx, m_offset, m_window, m_size);
}

//************************************************************************************************************************
