// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: class for keeping tilemap info

#ifndef _Tilemap_h_
#define _Tilemap_h_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

struct LevelCtx;

//************************************************************************************************************************

// single tile decsriptor:

enum TileFlags
{
	tf_Empty    = 0,
	tf_Blocking = 1 << 0,
	tf_Stair    = 1 << 1,
};

struct TileDesc
{
	int flags;
	int backID;
	int deckID;
	int foreID;
};

struct TileProxy
{
	int frame;
	CL_String name;
	CL_Sprite sprite;
};

// collision test results:

enum TileHit
{
	th_None,
	th_Vertical,
	th_Horizontal,
};

struct TileTest
{
	TileHit    type;
	TileDesc   tile;
	CL_Pointf  delta;
};

// pre-defined collision checkers:

typedef bool (*TileChecker)(const TileDesc &desc);

inline bool anyBlocking(const TileDesc &desc)
{ return (desc.flags & tf_Blocking) > 0; }

inline bool anyStairs(const TileDesc &desc)
{ return (desc.flags & tf_Stair) > 0; }

inline bool isBlocking(const TileDesc &desc)
{ return (desc.flags == tf_Blocking); }

inline bool topStairs(const TileDesc &desc)
{ return (desc.flags == (tf_Blocking | tf_Stair)); }

//************************************************************************************************************************

class Tilemap
{
public:
	typedef std::shared_ptr<Tilemap> Ref;
	explicit Tilemap(int dimX, int dimY, int size);

	// Gets the tile descriptor for target coordinates. Wrong
	// coords will return the default descriptor:

	TileDesc  getTile  (int x, int y) const;
	TileDesc  getTile  (CL_Pointf pt) const;
	TileProxy getProxy (int id, const LevelCtx &ctx);

	// Checks the rect, trying to move by "delta" offset within
	// the tilemap. Returns the actual available offset: 
	
	TileTest checkMove(CL_Rectf rect, CL_Pointf delta, TileChecker checker) const;

	// Sets the camera offset for rendering this tilemap:

	void offset (CL_Pointf pt);
	void window (CL_Sizef sz);

	// coordinates processing:

	CL_Pointf toScreen (CL_Pointf point) const;
	CL_Rectf  toScreen (CL_Rectf rect) const;
	CL_Pointf toTilespace (CL_Pointf pt) const;

	// filling the tilemap data (while loading):

	void pushDesc  (TileDesc desc);
	void pushProxy (const CL_String &name, int count);
	void pushImage (const CL_String &name);

	// rendering:
	void renderImage(const LevelCtx &ctx);
	void renderForeground(const LevelCtx &ctx);
	void renderDecoration(const LevelCtx &ctx);
	void renderBackground(const LevelCtx &ctx);

private:
	const float m_size;
	const int m_dimX;
	const int m_dimY;

	CL_Sizef  m_window;
	CL_Pointf m_offset;
	CL_Sizef  m_offmin;
	
	CL_String m_bgName;
	CL_Sprite m_bgSprite;

	std::vector<TileDesc>  m_tiles;
	std::vector<TileProxy> m_proxies;
};

//************************************************************************************************************************

#endif
