#pragma once
#include "OGRGeometryCollection.h"

class OGRMultiPoint :public OGRGeometryCollection
{
	vector<Point> points;
public:
	char* GetGeometryType();
	MBR GetMBR();
	void Copy(OGRMultiPoint&);
	bool Equals(OGRMultiPoint&);

	void AddGeometry(Point&);
	void UpdateGeometry(int,Point&);
	void RemoveGeometry(int);
	Point& GetGeometry(int);
	int GetNumGeometries();

	virtual void ImportFromWkt(string&);
	virtual void ExportToWkt(string&);
	
};