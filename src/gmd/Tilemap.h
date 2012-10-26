// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: class for keeping tilemap info

#ifndef _Tilemap_h_
#define _Tilemap_h_

#include <ClanLib/core.h>

//************************************************************************************************************************

class Tilemap
{
public:
	explicit Tilemap(int dimX, int dimY);

	bool getTile(int x, int y, bool colorize = false) const;
	bool isColor(int x, int y) const;
	void resetColor();

	// Checks the rect, trying to move by "delta" offset within
	// the tilemap. Returns the actual available offset: 
	
	CL_Pointf checkMove(CL_Rectf rc, CL_Pointf delta, int &hit) const;

private:
	typedef float& (*FnGet) (CL_Pointf&);
	typedef  void  (*FnSwp) (int&, int&);

	CL_Pointf checkLines(CL_Pointf ptOne, CL_Pointf delta, float length, FnGet x, FnGet y, FnSwp sw) const;

private:
	const int m_dimX;
	const int m_dimY;
	const float m_size;

	mutable std::vector<std::pair<bool, bool>> m_tiles;
};

//************************************************************************************************************************

#endif
