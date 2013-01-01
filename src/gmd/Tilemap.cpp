// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: class for keeping tilemap info

#include "Tilemap.h"

//************************************************************************************************************************

namespace 
{

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
void checkLines(const Tilemap &map, CL_Pointf ptOne, float length, CL_Pointf delta, TileTest &test)
{
	// precalc some useful deltas and parameters which differ for positive
	// and negative directions of movement:

	const int moveSg  = (y(delta) > 0) ? +1 : -1;
	const int rounder = (y(delta) > 0) ? +0 : -1;
	const int offset  = 1 + rounder;
	
	// checking against the coordinate lines. consider the code as a checking
	// against latitudes, and swap x and y, when checking longitudes:

	for (float line = int(y(ptOne)) + offset; between(y(ptOne), y(ptOne) + y(delta), line); line += moveSg)
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
			TileDesc tile = map.getTile(coords[0], coords[1]);
			if (tile.flags & tf_Blocking)
			{ 
				CL_Pointf result;
				x(result) = inter;
				y(result) = line - moveSg * 0.02f;

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

void checkHorizontal(const Tilemap &map, CL_Pointf ptOne, float length, CL_Pointf delta, TileTest &test)
{ 
	return checkLines<th_Horizontal, getY, getX, doSwap>(map, ptOne, length, delta, test); 
}

void checkVertical(const Tilemap &map, CL_Pointf ptOne, float length, CL_Pointf delta, TileTest &test)
{ 
	return checkLines<th_Vertical, getX, getY, noSwap>(map, ptOne, length, delta, test); 
}

}

//************************************************************************************************************************

Tilemap::Tilemap(int dimX, int dimY, int size)
: m_dimX(dimX), m_dimY(dimY), m_size(float(size))
{
	m_tiles.resize(dimX * dimY);

	int seed = 0;
	for (auto it = m_tiles.begin(); it != m_tiles.end(); ++ it)
	{ it->flags = (std::rand()) % 12 ? tf_Empty : tf_Blocking; }
}

TileDesc Tilemap::getTile(int x, int y) const
{
	static const TileDesc defTile = { tf_Blocking, 0, 0 };

	if (x <= 0 || x >= m_dimX) { return defTile; }
	if (y <= 0 || y >= m_dimY) { return defTile; }
	
	return m_tiles[m_dimX * y + x]; 
}

TileProxy Tilemap::getProxy(int id) const
{
	return m_proxies[id]; 
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

void Tilemap::pushProxy(CL_String name, int count)
{ 
	for (int no = 0; no < count; ++ no)
	{
		TileProxy proxy = { no, name, CL_Sprite() };
		m_proxies.push_back(proxy);
	}
}

// movement constraints:

TileTest Tilemap::checkMove(CL_Rectf rect, CL_Pointf delta) const
{
	TileTest test = { th_None, TileDesc(), delta };

	if (delta.x != 0.0f) // check horizontal movement
	{
		CL_Pointf ptTop = (delta.x > 0) ? rect.get_top_right() : rect.get_top_left();
		checkHorizontal(*this, ptTop, rect.get_height(), test.delta, test);
	}

	if (delta.y != 0.0f) // check vertical movement
	{
		CL_Pointf ptLef = (delta.y > 0) ? rect.get_bottom_left()  : rect.get_top_left();
		checkVertical(*this, ptLef, rect.get_width(), test.delta, test);
	}

	return test;
}

// rendering:

void Tilemap::render(CL_GraphicContext &gc, CL_Sprite &brick)
{
	// render tilemap:
	const int tilesInX = 3 + m_window.width  / m_size;
	const int tilesInY = 3 + m_window.height / m_size;

	const int startX = (int)m_offset.x - tilesInX / 2;
	const int startY = (int)m_offset.y - tilesInY / 2;

	for (int tileX = startX; tileX < startX + tilesInX; ++ tileX)
	for (int tileY = startY; tileY < startY + tilesInY; ++ tileY)
	{
		if (getTile(tileX, tileY).flags & tf_Blocking)
		{
			// draw shade and original sprite:
			CL_Pointf pt = toScreen(CL_Pointf(tileX, tileY));
			brick.draw(gc, pt.x, pt.y);
		}
	}
}

//************************************************************************************************************************
