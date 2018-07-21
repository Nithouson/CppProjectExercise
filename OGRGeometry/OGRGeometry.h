#pragma once
#include "global.h"

class OGRGeometry
{
	static int count;//计数变量，用于分配ID
protected:
	int id;
public:
	OGRGeometry();//实现ID分配
	int GetID();
	virtual void ImportFromWkt(string&)=0;
	virtual void ExportToWkt(string&)=0;
};


