#pragma once
#include "OGRGeometry.h"
#include "OGRPoint.h"
#include "OGRLineString.h"
class OGRPoint;
class OGRLineString;

class OGRPolygon :public OGRGeometry
{
	vector<Line> rings;
public:
	char* GetGeometryType();
	MBR GetMBR();
	vector<Line>& GetRings();
	double GetPerimeter();
	double GetArea();

	void Copy(OGRPolygon&);
	bool Equals(OGRPolygon&);
	bool Contains(OGRPoint&);
	bool Contains(OGRLineString&);
	bool Contains(OGRPolygon&);

	virtual void ImportFromWkt(string&);
	virtual void ExportToWkt(string&);
};