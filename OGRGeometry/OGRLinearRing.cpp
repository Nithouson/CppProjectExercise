#include "OGRLinearRing.h"

char* OGRLinearRing::GetGeometryType()
{
	return "OGRLinearRing";
}

double OGRLinearRing::GetLength()
{
	//与LineString不同，需加上首末点连线，故重新定义
	double l = 0;
	for (int i = 0; i < points.size() - 1; i++)
	{
		l += Point_dist(points[i], points[i + 1]);
	}
	l += Point_dist(points[points.size() - 1], points[0]);
	return l;
}


bool OGRLinearRing::Equals(OGRLinearRing& obj)
{
	return Ring_equal(points, obj.points);
}