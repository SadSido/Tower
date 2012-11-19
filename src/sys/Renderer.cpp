// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: class, incapsulating rendering operations

#include "Renderer.h"

//************************************************************************************************************************

Renderer::Renderer()
{
	// create a window:
	m_window = CL_DisplayWindow("Title", 800, 600, false);
}

void Renderer::preRender()
{
	getGC().clear();
}

void Renderer::postRender()
{
	getWindow().flip();
}

//************************************************************************************************************************

