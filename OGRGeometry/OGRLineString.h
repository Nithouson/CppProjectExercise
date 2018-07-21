#pragma once
#include "OGRGeometry.h"

class OGRLineString :public OGRGeometry
{
protected:
	Line points;
public:
	char* GetGeometryType();
	double GetLength();
	MBR GetMBR();
	Line& GetPoints();

	void Copy(OGRLineString&);
	bool Equals(OGRLineString&);
};