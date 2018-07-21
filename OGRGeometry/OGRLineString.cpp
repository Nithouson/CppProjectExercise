#include "OGRLineString.h"

char* OGRLineString::GetGeometryType()
{
	return "OGRLineString";
}

Line& OGRLineString::GetPoints()
{
	return points;
}

MBR OGRLineString::GetMBR()
{
	double xmax = points[0].first, xmin = points[0].first;
	double ymax = points[0].second, ymin = points[0].second;
	for (int i = 1; i < points.size(); i++)
	{
		xmax = max(xmax, points[i].first);
		xmin = min(xmin, points[i].first);
		ymax = max(ymax, points[i].second);
		ymin = min(ymin, points[i].second);
	}
	return make_pair(make_pair(xmin, ymin), make_pair(xmax, ymax));
}

double OGRLineString::GetLength()
{
	double l=0;
	for (int i = 0; i < points.size() - 1; i++)
	{
		l += Point_dist(points[i], points[i + 1]);
	}
	return l;
}

void OGRLineString::Copy(OGRLineString& obj)
{
	points = obj.points;
}

bool OGRLineString::Equals(OGRLineString& obj)
{
	return Line_equal(points, obj.points);
}