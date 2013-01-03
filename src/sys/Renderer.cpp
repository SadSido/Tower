// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: class, incapsulating rendering operations

#include "Renderer.h"

//************************************************************************************************************************

Renderer::Renderer()
{
	// create a window:
	m_window = CL_DisplayWindow("Title", 1024, 768, false);
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

