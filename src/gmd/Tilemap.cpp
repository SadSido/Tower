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
	{ it->flags = (std::rand()) % 20 ? tf_NoFlags : tf_Blocking; }
}

TileDesc Tilemap::getTile(int x, int y) const
{
	if (x <= 0 || x >= m_dimX) { return GetDefDesc(); }
	if (y <= 0 || y >= m_dimY) { return GetDefDesc(); }
	
	return m_tiles[m_dimX * y + x]; 
}

CL_Pointf Tilemap::checkMove(CL_Rectf rc, CL_Pointf delta, int &hit) const
{
	
	if (delta.x != 0.0f) // check horizontal movement
	{
		CL_Pointf ptTop = (delta.x > 0) ? rc.get_top_right()    : rc.get_top_left();
		CL_Pointf ptRes = checkLines(ptTop, delta, rc.get_height(), getY, getX, doSwap);
		
		hit = (ptRes == delta) ? hit : 1;
		delta = ptRes;
	}

	if (delta.y != 0.0f) // check vertical movement
	{
		CL_Pointf ptLef = (delta.y > 0) ? rc.get_bottom_left()  : rc.get_top_left();
		CL_Pointf ptRes = checkLines(ptLef, delta, rc.get_width(), getX, getY, noSwap);

		hit = (ptRes == delta) ? hit : 2;
		delta = ptRes;
	}

	return delta;
}

CL_Pointf Tilemap::checkLines(CL_Pointf ptOne, CL_Pointf delta, float length, FnGet x, FnGet y, FnSwp sw) const
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

				// return the correct delta:
				return result - ptOne; 
			}
		}
	}

	// no collisions detected, full delta available:
	return delta;
}

//************************************************************************************************************************
