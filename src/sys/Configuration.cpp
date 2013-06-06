// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: class for providing access to game config

#include "Configuration.h"

//************************************************************************************************************************

Configuration::Configuration()
{
}

// assigned controls:

int Configuration::keyUp() const
{ return CL_KEY_W; }

int Configuration::keyDown() const
{ return CL_KEY_S; }

int Configuration::keyLeft() const
{ return CL_KEY_A; }

int Configuration::keyRight() const
{ return CL_KEY_D; }

int Configuration::keyPause() const
{ return CL_KEY_ESCAPE; }

int Configuration::mouseLeft() const
{ return CL_MOUSE_LEFT; }

int Configuration::mouseRight() const
{ return CL_MOUSE_RIGHT; }


//************************************************************************************************************************

