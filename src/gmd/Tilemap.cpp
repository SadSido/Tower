// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: class for keeping tilemap info

#include "Tilemap.h"

//************************************************************************************************************************

static bool between(float a, float b, float test)
{ return (a > b) ? (test < a) && (test > b) : (test < b) && (test > a); }

float& getX (CL_Pointf &point) { return point.x; }
float& getY (CL_Pointf &point) { return point.y; }

void doSwap(int& one, int& two) { std::swap(one, two); }
void noSwap(int& one, int& two) {}

//************************************************************************************************************************

Tilemap::Tilemap(int dimX, int dimY)
: m_dimX(dimX), m_dimY(dimY), m_size(64)
{
	m_tiles.resize(dimX * dimY);

	int seed = 0;
	for (auto it = m_tiles.begin(); it != m_tiles.end(); ++ it)
	{ 
		it->first = (std::rand()) % 20 == 0;
		it->second = false;
	}
}

bool Tilemap::getTile(int x, int y, bool colorize) const
{
	if (x <= 0 || y <= 0)
		return true;
	
	if (x < m_dimX && y < m_dimY)
	{ 
		if (colorize) m_tiles[m_dimX * y + x].second = true;
		return m_tiles[m_dimX * y + x].first; 
	}
	
	return true;
}

bool Tilemap::isColor(int x, int y) const
{
	if (x <= 0 || y <= 0)
		return false;
	
	if (x < m_dimX && y < m_dimY)
	{ 
		return m_tiles[m_dimX * y + x].second; 
	}
	
	return false;
}

void Tilemap::resetColor()
{
	for (auto it = m_tiles.begin(); it != m_tiles.end(); ++ it)
	{ 
		it->second = false;
	}
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


/*

CL_Pointf Tilemap::checkLats(CL_Pointf ptLef, CL_Pointf ptRig, CL_Pointf delta, bool &hit) const
{
	float length = ptRig.x - ptLef.x;
	float moveSg = (delta.y > 0) ? +1 : -1;	
	 int rounder = (delta.y > 0) ? +0 : -1;
	 int offset  = 1 + rounder;
	
	// checking against the latitude lines:

	for (float latLine = int(ptLef.y) + offset; between(ptLef.y, ptLef.y + delta.y, latLine); latLine += moveSg)
	{
		float interX = ptLef.x + ((latLine - ptLef.y) * delta.x) / delta.y;

		int latCol = (int)(interX);
		int latEnd = (int)(interX + length);
		int latRow = (int)(latLine) + rounder;

		for (int colNo = latCol; colNo <= latEnd; ++ colNo)
		{
			if (getTile(colNo, latRow, true))
			{ hit = true; return CL_Pointf(interX, latLine - moveSg * 0.02f) - ptLef; }
		}
	}

	// no collisions detected, full delta available:
	return delta;
}

CL_Pointf Tilemap::checkLons(CL_Pointf ptTop, CL_Pointf ptBot, CL_Pointf delta, bool &hit) const
{
	float length = ptBot.y - ptTop.y;
	float moveSg = (delta.x > 0) ? +1 : -1;
	 int rounder = (delta.x > 0) ? +0 : -1;
	 int offset  = 1 + rounder;
	
	// checking against the longitude lines:

	for (float lonLine = int(ptTop.x) + offset; between(ptTop.x, ptTop.x + delta.x, lonLine); lonLine += moveSg)
	{
		float interY = ptTop.y + ((lonLine - ptTop.x) * delta.y) / delta.x;

		int lonRow = (int)(interY);
		int lonEnd = (int)(interY + length);
		int lonCol = (int)(lonLine) + rounder;

		for (int rowNo = lonRow; rowNo <= lonEnd; ++ rowNo)
		{
			if (getTile(lonCol, rowNo, true))
			{ hit = true; return CL_Pointf(lonLine - moveSg * 0.02f, interY) - ptTop; }
		}
	}

	// no collisions detected, full delta available:
	return delta;
}
*/

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
			if (getTile(coords[0], coords[1], true))
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
