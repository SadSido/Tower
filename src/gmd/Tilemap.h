// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: class for keeping tilemap info

#ifndef _Tilemap_h_
#define _Tilemap_h_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

struct UpdateCtx;
struct RenderCtx;

//************************************************************************************************************************

// set of options for the tile:

enum TileHit
{
	th_None,
	th_Vertical,
	th_Horizontal,
};

// single tile decsriptor:

enum TileFlags
{
	tf_Empty    = 0,
	tf_Blocking = 1 << 0,
};

struct TileDesc
{
	int flags;
	int backID;
	int foreID;
};

struct TileProxy
{
	int frame;
	CL_String name;
	CL_Sprite sprite;
};

// collision test result:

struct TileTest
{
	TileHit    type;
	TileDesc   tile;
	CL_Pointf  delta;
};

//************************************************************************************************************************

class Tilemap
{
public:
	typedef std::shared_ptr<Tilemap> Ref;
	explicit Tilemap(int dimX, int dimY, int size);

	// Gets the tile descriptor for target coordinates. Wrong
	// coords will return the default descriptor:

	TileDesc  getTile  (int x, int y) const;
	TileProxy getProxy (int id, RenderCtx &ctx);

	// Checks the rect, trying to move by "delta" offset within
	// the tilemap. Returns the actual available offset: 
	
	TileTest checkMove(CL_Rectf rect, CL_Pointf delta) const;

	// Sets the camera offset for rendering this tilemap:

	void offset (CL_Pointf pt);
	void window (CL_Sizef sz);

	// coordinates processing:

	CL_Pointf toScreen (CL_Pointf point) const;
	CL_Rectf  toScreen (CL_Rectf rect) const;
	CL_Pointf toTilespace (CL_Pointf pt) const;

	// filling the tilemap data (while loading):

	void pushDesc  (TileDesc desc);
	void pushProxy (CL_String name, int count);

	// rendering:
	void render(RenderCtx ctx);

private:
	const float m_size;
	const int m_dimX;
	const int m_dimY;

	CL_Sizef  m_window;
	CL_Pointf m_offset;

	std::vector<TileDesc>  m_tiles;
	std::vector<TileProxy> m_proxies;
};

//************************************************************************************************************************

#endif
