#include "OGRPoint.h"

OGRPoint::OGRPoint(){}

char* OGRPoint::GetGeometryType()
{
	return "OGRPoint";
}

MBR OGRPoint::GetMBR()
{
	return make_pair(point, point);
}

Point& OGRPoint::GetPoint()
{
	return point;
}

void OGRPoint::Copy(OGRPoint& obj)
{
	point = obj.point;
}

bool OGRPoint::Equals(OGRPoint& obj)
{
	return Point_equal(point,obj.point);
}

void OGRPoint::ImportFromWkt(string& filepath)
{
	freopen(filepath.c_str(), "r", stdin);
	char buffer[LINEMAX];
	vector<string> word;
	const char * delim = " ()";
	char * c;
	cin.clear();
	cin.getline(buffer, LINEMAX);
	c = strtok(buffer, delim);
	//字符串分割，得到"Point"，x,y三个子串
	while (c)
	{
		word.push_back(c);
		c = strtok(NULL, delim);
	}
	point.first = stod(word[1]);
	point.second = stod(word[2]);
	fclose(stdin);
}

void OGRPoint::ExportToWkt(string& filepath)
{
	freopen(filepath.c_str(), "w", stdout);
	cout << "Point (" << point.first << " " << point.second << ")" << endl;
	fclose(stdout);
}