#pragma once
#include "OGRGeometry.h"

class OGRPoint :public OGRGeometry
{
	Point point;
public:
	OGRPoint();
	char* GetGeometryType();
	Point& GetPoint();
	MBR GetMBR();
	void Copy(OGRPoint&);
	bool Equals(OGRPoint&);

	virtual void ImportFromWkt(string&);
	virtual void ExportToWkt(string&);
};