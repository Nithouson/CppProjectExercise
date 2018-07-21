#pragma once
#include "OGRLineString.h"

class OGRLinearRing:public OGRLineString
{
public:
	char* GetGeometryType();
	double GetLength();
	bool Equals(OGRLinearRing&);
};