#pragma once
#include "OGRGeometryCollection.h"

class OGRMultiLineString :public OGRGeometryCollection
{
	vector<Line> lines;
public:
	char* GetGeometryType();
	Line& GetGeometry(int);
	int GetNumGeometries();
	MBR GetMBR();
	double GetLength();

	void Copy(OGRMultiLineString&);
	bool Equals(OGRMultiLineString&);
	void AddGeometry(Line&);
	void UpdateGeometry(int,Line&);
	void RemoveGeometry(int);
	
};