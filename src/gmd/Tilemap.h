// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: class for keeping tilemap info

#ifndef _Tilemap_h_
#define _Tilemap_h_

#include <ClanLib/core.h>

//************************************************************************************************************************

// set of options for the tile:

enum TileFlags
{
	tf_NoFlags  = 0,
	tf_Blocking = 1 << 0,
};

enum TileHit
{
	th_None,
	th_Vertical,
	th_Horizontal,
};

// single tile decsriptor:

struct TileDesc
{
	int flags;
};

// collision test result:

struct TileTest
{
	TileHit   type;
	TileDesc  desc;

	CL_Pointf offset;
	CL_Pointf origin;
};

//************************************************************************************************************************

class Tilemap
{
public:
	explicit Tilemap(int dimX, int dimY);

	// Gets the tile descriptor for target coordinates. Wrong
	// coords will return the default descriptor:

	TileDesc getTile(int x, int y) const;

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

	std::vector<TileDesc> m_tiles;
};

//************************************************************************************************************************

#endif
