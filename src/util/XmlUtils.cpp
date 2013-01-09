// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: xml parsing wrappers

#include "XmlUtils.h"

//************************************************************************************************************************

namespace 
{

template <typename RESULT>
RESULT readAttr(CL_DomElement node, const CL_String &one, const CL_String &two)
{
	return RESULT(node.get_attribute_float(one), node.get_attribute_float(two));
}

}

//************************************************************************************************************************

CL_Sizef readSize(CL_DomElement node)
{ return readAttr<CL_Sizef>(node, "width", "height"); }

CL_Pointf readPos(CL_DomElement node)
{ return readAttr<CL_Pointf>(node, "x", "y"); }

//************************************************************************************************************************
