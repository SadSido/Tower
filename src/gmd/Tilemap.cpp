// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: class for keeping tilemap info

#include "Tilemap.h"

//************************************************************************************************************************

namespace 
{

// tiledesc struct helpers:

TileDesc GetDefDesc()
{
	TileDesc def; def.flags = tf_Blocking;
	return def;
}

// collision test helpers:

bool between(float a, float b, float test)
{ return (a > b) ? (test < a) && (test > b) : (test < b) && (test > a); }

float& getX (CL_Pointf &point) { return point.x; }
float& getY (CL_Pointf &point) { return point.y; }

void doSwap(int& one, int& two) { std::swap(one, two); }
void noSwap(int& one, int& two) {}

}

//************************************************************************************************************************

Tilemap::Tilemap(int dimX, int dimY)
: m_dimX(dimX), m_dimY(dimY), m_size(64)
{
	m_tiles.resize(dimX * dimY);

	int seed = 0;
	for (auto it = m_tiles.begin(); it != m_tiles.end(); ++ it)
	{ it->flags = (std::rand()) % 20 ? tf_Empty : tf_Blocking; }
}

TileDesc Tilemap::getTile(int x, int y) const
{
	if (x <= 0 || x >= m_dimX) { return GetDefDesc(); }
	if (y <= 0 || y >= m_dimY) { return GetDefDesc(); }
	
	return m_tiles[m_dimX * y + x]; 
}

TileTest Tilemap::checkMove(CL_Rectf rect, CL_Pointf delta) const
{
	TileTest test = { th_None, TileDesc(), delta };

	if (delta.x != 0.0f) // check horizontal movement
	{
		CL_Pointf ptTop = (delta.x > 0) ? rect.get_top_right() : rect.get_top_left();
		checkLines(test, ptTop, test.delta, rect.get_height(), th_Horizontal, getY, getX, doSwap);
	}

	if (delta.y != 0.0f) // check vertical movement
	{
		CL_Pointf ptLef = (delta.y > 0) ? rect.get_bottom_left()  : rect.get_top_left();
		checkLines(test, ptLef, test.delta, rect.get_width(), th_Vertical, getX, getY, noSwap);
	}

	return test;
}

void Tilemap::checkLines(TileTest &test, CL_Pointf ptOne, CL_Pointf delta, float length, TileHit type, FnGet x, FnGet y, FnSwp sw) const
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
			TileDesc tile = getTile(coords[0], coords[1]);
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

//************************************************************************************************************************
