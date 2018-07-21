#include "OGRGeometry.h"

OGRGeometry::OGRGeometry()
{
	id = count;
	count++;
}

int OGRGeometry::GetID()
{
	return id;
}