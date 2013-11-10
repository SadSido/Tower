// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: xml parsing wrappers

#ifndef _XmlUtils_h_
#define _XmlUtils_h_

#include "MathUtils.h"
#include <ClanLib/core.h>

//************************************************************************************************************************

// read from attributes:

CL_Sizef  readSize  (CL_DomElement node);
CL_Pointf readPoint (CL_DomElement node);
CL_Rectf  readRect  (CL_DomElement node);
Range     readRange (CL_DomElement node);

// parse from one attribute:

CL_Sizef  readSize  (CL_DomElement node, const CL_String &attrName);
CL_Pointf readPoint (CL_DomElement node, const CL_String &attrName);
Range     readRange (CL_DomElement node, const CL_String &attrName);

//************************************************************************************************************************

#endif
