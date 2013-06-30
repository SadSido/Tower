// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: custom classes for managing entities data

#ifndef _EntityData_h_
#define _EntityData_h_

#include "Hitmap.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/sound.h>
#include <vector>

//************************************************************************************************************************

class SpriteVec : public std::vector<CL_Sprite>
{
public:
	SpriteVec ();
	SpriteVec (const SpriteVec &other);
};

//************************************************************************************************************************

class HitmapVec : public std::vector<Hitmap>
{};

//************************************************************************************************************************

class SoundVec : public std::vector<CL_SoundBuffer>
{};

//************************************************************************************************************************

#endif
