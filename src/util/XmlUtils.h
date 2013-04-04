// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: xml parsing wrappers

#ifndef _XmlUtils_h_
#define _XmlUtils_h_

#include <ClanLib/core.h>

//************************************************************************************************************************

CL_Sizef  readSize  (CL_DomElement node);
CL_Pointf readPoint (CL_DomElement node);
CL_Pointf readPoint (CL_DomElement node, CL_String attrName);
CL_Rectf  readRect  (CL_DomElement node);

//************************************************************************************************************************

#endif
