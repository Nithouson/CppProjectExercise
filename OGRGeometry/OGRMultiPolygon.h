#pragma once
#include "OGRGeometryCollection.h"
#include "OGRPoint.h"
#include "OGRLineString.h"
#include "OGRPolygon.h"
class OGRPoint;
class OGRLineString;
class OGRPolygon;

class OGRMultiPolygon :public OGRGeometryCollection
{
	vector<vector<Line> > polygons;
public:
	char* GetGeometryType();
	MBR GetMBR();
	double GetPerimeter();
	double GetArea();

	void Copy(OGRMultiPolygon&);
	bool Equals(OGRMultiPolygon&);
	bool Contains(OGRPoint&);
	bool Contains(OGRLineString&);
	bool Contains(OGRPolygon&);

	void AddGeometry(vector<Line>&);
	void UpdateGeometry(int,vector<Line>&);
	void RemoveGeometry(int);
	vector<Line>& GetGeometry(int);
	int GetNumGeometries();

	virtual void ImportFromWkt(string&);
	virtual void ExportToWkt(string&);
};